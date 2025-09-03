#!/usr/bin/env python3
"""
SOLSTICE – SCAD interface (PySide6) - VERSION CORRIGÉE

- Tabs: Live, History (plots), Config, Diagnostics
- Reads Arduino SCAD frames:
  DATA,<ts>,<t_PTC1>,<t_PTC2>,<t_cuve1>,<t_cuve2>,<enc1>,<enc2>,<pv_volts>
- Live cards for temps, encoders, PV voltage, simple temperature estimate
- History tab plots temperatures and PV voltage (requires pyqtgraph)
- Optional OPC UA export of live frames
- Source status: port, lines/sec, last frame age, timestamp origin
- Mock source available for testing

Usage:
    python3 -m pip install --upgrade pip
    python3 -m pip install PySide6 pyserial pyqtgraph opcua
    python solstice_scad_fixed.py
"""

import sys, time, random, csv
from collections import deque
from typing import Optional
from dataclasses import dataclass
from datetime import datetime, timezone

from PySide6.QtCore import Qt, QThread, Signal, Slot, QTimer, QObject
from PySide6.QtWidgets import (
    QApplication,
    QMainWindow,
    QWidget,
    QTabWidget,
    QVBoxLayout,
    QHBoxLayout,
    QGridLayout,
    QLabel,
    QPushButton,
    QComboBox,
    QGroupBox,
    QFormLayout,
    QTextEdit,
    QSizePolicy,
    QFileDialog,
    QLineEdit,
)

# Optional: plotting
try:
    import pyqtgraph as pg
except Exception:
    pg = None

# Optional: pyserial
try:
    import serial
    from serial.tools import list_ports
except Exception:
    serial = None
    list_ports = None

# Optional: OPC UA
try:
    from opcua import Client
except Exception:
    Client = None


# -------------------------
# Data model
# -------------------------
@dataclass
class Frame:
    ts: datetime
    t_PTC1: float
    t_PTC2: float
    t_cuve1: float
    t_cuve2: float
    encoder1: int
    encoder2: int
    pv_volts: float

    @property
    def ts_epoch(self) -> float:
        if self.ts.tzinfo is None:
            return self.ts.timestamp()
        return self.ts.astimezone(timezone.utc).timestamp()


def _parse_timestamp(ts_raw: str) -> datetime:
    """Parse timestamp from Arduino format: YYYY-MM-DD HH:MM:SS"""
    s = ts_raw.strip()
    try:
        return datetime.strptime(s, "%Y-%m-%d %H:%M:%S")
    except ValueError:
        try:
            if s.endswith("Z"):
                s = s[:-1] + "+00:00"
            return datetime.fromisoformat(s)
        except ValueError:
            return datetime.now()


def parse_frame(line: str) -> Frame:
    """Parse Arduino data frame: DATA,timestamp,temp1,temp2,temp3,temp4,enc1,enc2,pv"""
    s = line.strip()
    if not s.startswith("DATA,"):
        raise ValueError("Line does not start with DATA")
    parts = s.split(",")
    if len(parts) < 9:
        raise ValueError(f"Expected 9 fields, got {len(parts)} → {parts}")
    _, ts_raw, te, tp, c1, c2, e1, e2, pv = parts[:9]
    ts = _parse_timestamp(ts_raw)
    return Frame(
        ts=ts,
        t_PTC1=float(te),
        t_PTC2=float(tp),
        t_cuve1=float(c1),
        t_cuve2=float(c2),
        encoder1=int(e1),
        encoder2=int(e2),
        pv_volts=float(pv),
    )


# -------------------------
# Reader threads
# -------------------------
class BaseReader(QThread):
    line_received = Signal(str)
    opened = Signal(str)
    closed = Signal()
    error = Signal(str)

    def stop(self):
        self._stop = True


class SerialReader(BaseReader):
    def __init__(self, port: str, baud: int = 9600, parent: Optional[QObject] = None):
        super().__init__(parent)
        self.port = port
        self.baud = baud
        self._stop = False

    def run(self):
        if serial is None:
            self.error.emit("pyserial not installed. Run: pip install pyserial")
            self.closed.emit()
            return
        try:
            ser = serial.Serial(self.port, self.baud, timeout=1)
            self.opened.emit(self.port)
            buf = b""
            while not self._stop:
                chunk = ser.read(256)
                if not chunk:
                    continue
                buf += chunk
                while b"\n" in buf:
                    line, buf = buf.split(b"\n", 1)
                    s = line.decode("utf-8", errors="ignore").strip("\r\n ")
                    if s:
                        self.line_received.emit(s)
            try:
                ser.close()
            except Exception:
                pass
        except Exception as e:
            self.error.emit(str(e))
        finally:
            self.closed.emit()


class MockReader(BaseReader):
    def __init__(self, parent: Optional[QObject] = None):
        super().__init__(parent)
        self._stop = False

    def run(self):
        self.opened.emit("MOCK")
        while not self._stop:
            now = datetime.now()
            timestamp = now.strftime("%Y-%m-%d %H:%M:%S")
            t_PTC1 = 24.0 + random.uniform(-0.2, 0.2)
            t_pan = 28.0 + random.uniform(-0.2, 0.2)
            c1 = 23.5 + random.uniform(-0.2, 0.2)
            c2 = 23.0 + random.uniform(-0.2, 0.2)
            e1 = random.randint(0, 100)
            e2 = random.randint(0, 100)
            pv = 1.5 + random.uniform(-0.05, 0.05)
            line = f"DATA,{timestamp},{t_PTC1:.2f},{t_pan:.2f},{c1:.2f},{c2:.2f},{e1},{e2},{pv:.3f}"
            self.line_received.emit(line)
            self.msleep(500)
        self.closed.emit()


# -------------------------
# UI helpers
# -------------------------
def mm(s: str) -> QLabel:
    q = QLabel(s)
    q.setAlignment(Qt.AlignCenter)
    q.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Preferred)
    q.setProperty("card", True)
    return q


# -------------------------
# Main Window
# -------------------------

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("SOLSTICE – SCAD Live (Fixed)")
        self.resize(1100, 700)

        # runtime state
        self.reader = None
        self.lps_window = deque(maxlen=200)
        self.last_frame = None
        self.frames_log = []  # store all frames for saving
        self.opc_client = None
        self.opc_nodes = {}

        # history buffers for plots
        self.history_ts = deque(maxlen=500)
        self.history_t1 = deque(maxlen=500)
        self.history_t2 = deque(maxlen=500)
        self.history_c1 = deque(maxlen=500)
        self.history_c2 = deque(maxlen=500)
        self.history_pv = deque(maxlen=500)

        # tabs container
        self.tabs = QTabWidget()
        self.setCentralWidget(self.tabs)

        # tabs widgets
        self.live_tab = QWidget()
        self.history_tab = QWidget()
        self.config_tab = QWidget()
        self.diag_tab = QWidget()

        # build tabs
        self._build_live()
        self._build_history()
        self._build_diag()
        self._build_config()

        self.tabs.addTab(self.live_tab, "Live")
        self.tabs.addTab(self.history_tab, "History")
        self.tabs.addTab(self.config_tab, "Config")
        self.tabs.addTab(self.diag_tab, "Diagnostics")

        self.status_timer = QTimer(self)
        self.status_timer.timeout.connect(self._update_status)
        self.status_timer.start(500)
        self.refresh_ports()

        self._log("App ready. Choose a source in Config → Connect.")

    # ---------- UI ----------
    def _build_live(self):
        lay = QVBoxLayout()
        grid = QGridLayout()
        self.lbl_t_PTC1 = mm("PTC1 — °C\n–")
        self.lbl_t_pan = mm("PTC2 — °C\n–")
        self.lbl_cuve1 = mm("Cuve1 — °C\n–")
        self.lbl_cuve2 = mm("Cuve2 — °C\n–")
        grid.addWidget(self._wrap_card("PTC1", self.lbl_t_PTC1), 0, 0)
        grid.addWidget(self._wrap_card("PTC2", self.lbl_t_pan), 0, 1)
        grid.addWidget(self._wrap_card("Cuve1", self.lbl_cuve1), 1, 0)
        grid.addWidget(self._wrap_card("Cuve2", self.lbl_cuve2), 1, 1)

        extra = QGridLayout()
        self.lbl_enc1 = mm("Enc1 —\n–")
        self.lbl_enc2 = mm("Enc2 —\n–")
        self.lbl_pv = mm("PV — V\n–")
        extra.addWidget(self._wrap_card("Encoder 1", self.lbl_enc1), 0, 0)
        extra.addWidget(self._wrap_card("Encoder 2", self.lbl_enc2), 0, 1)
        extra.addWidget(self._wrap_card("PV Voltage", self.lbl_pv), 0, 2)

        status_box = QGroupBox("Source status")
        status_l = QFormLayout()
        self.lbl_src = QLabel("–")
        self.lbl_lps = QLabel("0.0")
        self.lbl_last = QLabel("–")
        self.lbl_time_origin = QLabel("–")
        self.lbl_estimate = QLabel("–")
        status_l.addRow("Source:", self.lbl_src)
        status_l.addRow("Lines/sec:", self.lbl_lps)
        status_l.addRow("Last frame age:", self.lbl_last)
        status_l.addRow("Timestamp origin:", self.lbl_time_origin)
        status_l.addRow("Temp. estimée:", self.lbl_estimate)
        status_box.setLayout(status_l)

        row = QHBoxLayout()
        row.addLayout(grid, 2)
        row.addLayout(extra, 1)
        row.addWidget(status_box, 1)

        lay.addLayout(row)
        lay.addStretch(1)
        self.live_tab.setLayout(lay)

    def _wrap_card(self, title: str, value_label: QLabel) -> QGroupBox:
        box = QGroupBox(title)
        l = QVBoxLayout()
        value_label.setObjectName("metric")
        l.addWidget(value_label)
        box.setLayout(l)
        return box

    def _build_history(self):
        lay = QVBoxLayout()
        if pg is None:
            lay.addWidget(QLabel("pyqtgraph not installed"))
        else:
            self.temp_plot = pg.PlotWidget(title="Temperatures (°C)")
            self.temp_plot.addLegend()
            self.curve_t1 = self.temp_plot.plot(pen='r', name="PTC1")
            self.curve_t2 = self.temp_plot.plot(pen='g', name="PTC2")
            self.curve_c1 = self.temp_plot.plot(pen='c', name="Cuve1")
            self.curve_c2 = self.temp_plot.plot(pen='m', name="Cuve2")
            self.pv_plot = pg.PlotWidget(title="PV Voltage (V)")
            self.curve_pv = self.pv_plot.plot(pen='y', name="PV")
            lay.addWidget(self.temp_plot)
            lay.addWidget(self.pv_plot)
        self.history_tab.setLayout(lay)

    def _build_config(self):
        lay = QVBoxLayout()
        src_box = QGroupBox("Data source")
        form = QFormLayout()
        self.cb_port = QComboBox()
        self.btn_refresh = QPushButton("Refresh ports")
        self.btn_connect = QPushButton("Connect")
        self.btn_disconnect = QPushButton("Disconnect")
        self.btn_disconnect.setEnabled(False)
        form.addRow("Port:", self.cb_port)
        row = QHBoxLayout()
        row.addWidget(self.btn_refresh)
        row.addWidget(self.btn_connect)
        row.addWidget(self.btn_disconnect)
        form.addRow(row)
        src_box.setLayout(form)
        lay.addWidget(src_box)

        save_box = QGroupBox("Logging")
        save_l = QHBoxLayout()
        self.btn_save = QPushButton("Save CSV")
        save_l.addWidget(self.btn_save)
        save_box.setLayout(save_l)
        lay.addWidget(save_box)

        opc_box = QGroupBox("OPC UA")
        opc_form = QFormLayout()
        self.le_opc = QLineEdit("opc.tcp://localhost:4840")
        self.btn_opc_connect = QPushButton("Connect OPC")
        self.btn_opc_disconnect = QPushButton("Disconnect OPC")
        self.btn_opc_disconnect.setEnabled(False)
        opc_form.addRow("Endpoint:", self.le_opc)
        opc_btn_row = QHBoxLayout()
        opc_btn_row.addWidget(self.btn_opc_connect)
        opc_btn_row.addWidget(self.btn_opc_disconnect)
        opc_form.addRow(opc_btn_row)
        opc_box.setLayout(opc_form)
        lay.addWidget(opc_box)

        lay.addStretch(1)
        self.config_tab.setLayout(lay)

        self.btn_refresh.clicked.connect(self.refresh_ports)
        self.btn_connect.clicked.connect(self.connect_source)
        self.btn_disconnect.clicked.connect(self.disconnect_source)
        self.btn_save.clicked.connect(self.save_csv)
        self.btn_opc_connect.clicked.connect(self.connect_opc)
        self.btn_opc_disconnect.clicked.connect(self.disconnect_opc)

        self.refresh_ports()

    def _build_diag(self):
        lay = QVBoxLayout()
        self.txt_diag = QTextEdit()
        self.txt_diag.setReadOnly(True)
        lay.addWidget(self.txt_diag)
        self.diag_tab.setLayout(lay)

    # ---------- Actions ----------
    @Slot()
    def refresh_ports(self):
        current = self.cb_port.currentText()
        self.cb_port.blockSignals(True)
        self.cb_port.clear()
        self.cb_port.addItem("Mock data")
        self.cb_port.addItem("/dev/serial0", userData="/dev/serial0")
        self.cb_port.addItem("/dev/ttyS0", userData="/dev/ttyS0")
        self.cb_port.addItem("/dev/ttyUSB0", userData="/dev/ttyUSB0")
        self.cb_port.addItem("/dev/ttyACM0", userData="/dev/ttyACM0")
        ports = []
        if list_ports is not None:
            try:
                ports = list(list_ports.comports())
            except Exception:
                ports = []
        for p in ports:
            display_name = f"{p.device} — {p.description}"
            self.cb_port.addItem(display_name, userData=p.device)
        idx = self.cb_port.findText(current)
        if idx >= 0:
            self.cb_port.setCurrentIndex(idx)
        self.cb_port.blockSignals(False)
        self._log(f"Ports refreshed. Found {len(ports)} serial port(s).")

    @Slot()
    def connect_source(self):
        if self.reader is not None:
            self._log("Already connected.")
            return
        chosen = self.cb_port.currentText()
        if chosen.startswith("Mock"):
            r = MockReader()
        else:
            port = self.cb_port.currentData() or chosen.split(" — ")[0]
            r = SerialReader(port=port, baud=9600)
        self.reader = r
        r.line_received.connect(self.on_line)
        r.opened.connect(self.on_opened)
        r.closed.connect(self.on_closed)
        r.error.connect(self.on_error)
        r.start()
        self.btn_connect.setEnabled(False)
        self.btn_disconnect.setEnabled(True)

    @Slot()
    def disconnect_source(self):
        r = self.reader
        if r is None:
            return
        try:
            r.stop()
            r.wait(1500)
        except Exception:
            pass
        self.reader = None
        self.btn_connect.setEnabled(True)
        self.btn_disconnect.setEnabled(False)
        self._log("Disconnected.")

    @Slot()
    def connect_opc(self):
        if Client is None:
            self._log("opcua not installed. Run: pip install opcua")
            return
        if self.opc_client is not None:
            self._log("OPC already connected.")
            return
        endpoint = self.le_opc.text().strip()
        try:
            client = Client(endpoint)
            client.connect()
            self.opc_client = client
            self.opc_nodes = {
                "t_PTC1": client.get_node("ns=2;s=PTC1"),
                "t_PTC2": client.get_node("ns=2;s=PTC2"),
                "t_cuve1": client.get_node("ns=2;s=Cuve1"),
                "t_cuve2": client.get_node("ns=2;s=Cuve2"),
                "pv_volts": client.get_node("ns=2;s=PV"),
            }
            self.btn_opc_connect.setEnabled(False)
            self.btn_opc_disconnect.setEnabled(True)
            self._log(f"Connected OPC UA at {endpoint}")
        except Exception as e:
            self._log(f"OPC connect failed: {e}")

    @Slot()
    def disconnect_opc(self):
        if self.opc_client is None:
            return
        try:
            self.opc_client.disconnect()
        except Exception:
            pass
        self.opc_client = None
        self.opc_nodes = {}
        self.btn_opc_connect.setEnabled(True)
        self.btn_opc_disconnect.setEnabled(False)
        self._log("OPC disconnected.")

    # ---------- Signals ----------
    @Slot(str)
    def on_opened(self, src: str):
        self._log(f"Connected to {src}.")
        self.lbl_src.setText(src)

    @Slot()
    def on_closed(self):
        self._log("Source closed.")
        self.lbl_src.setText("–")

    @Slot(str)
    def on_error(self, msg: str):
        self._log(f"ERROR: {msg}")

    @Slot(str)
    def on_line(self, line: str):
        now = time.time()
        self.lps_window.append(now)
        self._log(f"Raw: {line}")
        try:
            fr = parse_frame(line)
            self.last_frame = fr
            self.frames_log.append(fr)
            self._update_live(fr)
            self._update_history(fr)
            self._send_opc(fr)
            self._log(f"Parsed successfully: T1={fr.t_PTC1}°C, T2={fr.t_PTC2}°C")
        except Exception as e:
            self._log(f"Parse error: {line!r} → {e}")

    # ---------- Updates ----------
    def _update_live(self, fr: Frame):
        self.lbl_t_PTC1.setText(f"PTC1 — °C\n{fr.t_PTC1:.2f}")
        self.lbl_t_pan.setText(f"PTC2 — °C\n{fr.t_PTC2:.2f}")
        self.lbl_cuve1.setText(f"Cuve1 — °C\n{fr.t_cuve1:.2f}")
        self.lbl_cuve2.setText(f"Cuve2 — °C\n{fr.t_cuve2:.2f}")
        self.lbl_enc1.setText(f"Enc1 —\n{fr.encoder1}")
        self.lbl_enc2.setText(f"Enc2 —\n{fr.encoder2}")
        self.lbl_pv.setText(f"PV — V\n{fr.pv_volts:.3f}")
        origin = (
            "UTC"
            if (fr.ts.tzinfo and fr.ts.utcoffset() == timezone.utc.utcoffset(fr.ts))
            else ("Offset-aware" if fr.ts.tzinfo else "Local")
        )
        self.lbl_time_origin.setText(origin)
        est = (fr.t_PTC1 + fr.t_PTC2 + fr.t_cuve1 + fr.t_cuve2) / 4.0
        self.lbl_estimate.setText(f"{est:.2f} °C")

    def _update_history(self, fr: Frame):
        if pg is None:
            return
        ts = fr.ts_epoch
        self.history_ts.append(ts)
        self.history_t1.append(fr.t_PTC1)
        self.history_t2.append(fr.t_PTC2)
        self.history_c1.append(fr.t_cuve1)
        self.history_c2.append(fr.t_cuve2)
        self.history_pv.append(fr.pv_volts)
        self.curve_t1.setData(self.history_ts, self.history_t1)
        self.curve_t2.setData(self.history_ts, self.history_t2)
        self.curve_c1.setData(self.history_ts, self.history_c1)
        self.curve_c2.setData(self.history_ts, self.history_c2)
        self.curve_pv.setData(self.history_ts, self.history_pv)

    def _send_opc(self, fr: Frame):
        if self.opc_client is None:
            return
        try:
            for key, val in {
                "t_PTC1": fr.t_PTC1,
                "t_PTC2": fr.t_PTC2,
                "t_cuve1": fr.t_cuve1,
                "t_cuve2": fr.t_cuve2,
                "pv_volts": fr.pv_volts,
            }.items():
                node = self.opc_nodes.get(key)
                if node is not None:
                    node.set_value(val)
        except Exception as e:
            self._log(f"OPC send failed: {e}")

    def _update_status(self):
        now = time.time()
        while self.lps_window and now - self.lps_window[0] > 3.0:
            self.lps_window.popleft()
        lps = len(self.lps_window) / 3.0
        self.lbl_lps.setText(f"{lps:.2f}")
        if self.last_frame is None:
            self.lbl_last.setText("–")
        else:
            age = max(0.0, now - self.last_frame.ts_epoch)
            self.lbl_last.setText(f"{age:.1f} s")

    # ---------- Utils ----------
    def _log(self, msg: str):
        ts = datetime.now().strftime("%H:%M:%S")
        if hasattr(self, "txt_diag") and self.txt_diag is not None:
            self.txt_diag.append(f"[{ts}] {msg}")
        else:
            print(f"[{ts}] {msg}")

    @Slot()
    def save_csv(self):
        if not self.frames_log:
            self._log("No data to save.")
            return
        fname, _ = QFileDialog.getSaveFileName(self, "Save CSV", "data.csv", "CSV files (*.csv)")
        if not fname:
            return
        try:
            with open(fname, "w", newline="") as f:
                writer = csv.writer(f)
                writer.writerow(["timestamp","t_PTC1","t_PTC2","t_cuve1","t_cuve2","encoder1","encoder2","pv_volts"])
                for fr in self.frames_log:
                    writer.writerow([
                        fr.ts.isoformat(sep=" "),
                        f"{fr.t_PTC1:.2f}",
                        f"{fr.t_PTC2:.2f}",
                        f"{fr.t_cuve1:.2f}",
                        f"{fr.t_cuve2:.2f}",
                        fr.encoder1,
                        fr.encoder2,
                        f"{fr.pv_volts:.3f}",
                    ])
            self._log(f"Saved {len(self.frames_log)} rows to {fname}")
        except Exception as e:
            self._log(f"Save failed: {e}")


# -------------------------
# Style
# -------------------------
QSS = """
QMainWindow { background: #0f1115; }
QGroupBox { border: 1px solid #2a2f3a; border-radius: 10px; margin-top: 12px; color: #e5e9f0; font-weight: 600; }
QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 4px; }
QLabel { color: #d8dee9; font-size: 14px; }
QLabel#metric { font-size: 28px; font-weight: 700; padding: 10px 0; }
QTabWidget::pane { border: 1px solid #2a2f3a; border-radius: 8px; }
QTabBar::tab { background: #1a1e27; color: #cfd6e6; padding: 8px 14px; border: 1px solid #2a2f3a; border-bottom: none; border-top-left-radius: 8px; border-top-right-radius: 8px; }
QTabBar::tab:selected { background: #232838; color: #e9edf7; }
QPushButton { background: #232838; color: #e9edf7; border: 1px solid #2e3547; border-radius: 8px; padding: 8px 12px; }
QPushButton:hover { background: #2b3144; }
QComboBox { background: #1a1e27; color: #e9edf7; border: 1px solid #2a2f3a; border-radius: 6px; padding: 6px; }
QTextEdit { background: #0b0d12; color: #cfd6e6; border: 1px solid #2a2f3a; border-radius: 8px; }
"""


def main():
    app = QApplication(sys.argv)
    app.setStyleSheet(QSS)
    w = MainWindow()
    w.show()
    sys.exit(app.exec())


if __name__ == "__main__":
    main()
