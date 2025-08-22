#include <string>

class SerialReader {
public:
    SerialReader(const std::string& port, int baudRate);
    ~SerialReader();

    bool openPort();
    std::string readLine();
    void closePort();

private:
    int serialFd;
    std::string portName;
    int baudRate;
};
