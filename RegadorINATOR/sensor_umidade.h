#ifndef SENSOR_UMIDADE_H_INCLUDED
#define SENSOR_UMIDADE_H_INCLUDED

#include "sensor.h"

class SensorUmidade: public Sensor {
  public:
  SensorUmidade();
  int Read(int porta);
};

#endif // SENSOR_UMIDADE_H_INCLUDED
