#include <Arduino.h>

#include "definicoes_sistema.h"
#include "sensor_umidade.h"

SensorUmidade::SensorUmidade()
{
}

  /************************
   Read
  Realiza a leitura do sensor
  entradas
    inteiro: recebe o número da porta
  saidas
    inteiro: valor lido em porcentagem
  *************************/
int SensorUmidade::Read(int porta)
{
    int umidade = analogRead(porta) - 300;  // reduzindo o intevalo de 0 a 723
    if (umidade < 0){
      umidade = 0;  // retirando valores negativos
    }
    int umidPorcento = (int)((1-((float)umidade/723))*100) ;  // transformando em porcentagem
    Serial.print("Umidade");
    Serial.println(umidPorcento);
    return umidPorcento;   
}
