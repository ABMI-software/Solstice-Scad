#pragma once 

class PumpController {
public:
  void begin();

  void setA(int pct);   // 0..100
  void setB(int pct);   // 0..100
  int  getA() const { return a_pct; }
  int  getB() const { return b_pct; }

  void stopAll();

private:
  int a_pct = 0, b_pct = 0;
  void applyA();
  void applyB();
};