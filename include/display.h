#pragma once

class NullDisplay {
public:
  NullDisplay(N2KCollector &n2kCollector, Modbus &modbus, WebServer &webServer) {};
  void nextPage() {};
  void begin() {};
  void update() {};

};
