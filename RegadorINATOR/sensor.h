#ifndef SENSOR_H_INCLUDED
#define SENSOR_H_INCLUDED

class Sensor {
  public:
  /************************
   Read
  Realiza a leitura do sensor
  entradas
    inteiro: recebe o número da porta
  saidas
    inteiro: valor lido em porcentagem
  *************************/
  virtual int Read(int porta) = 0;
};


#endif // SENSOR_H_INCLUDED
