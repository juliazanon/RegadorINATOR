/*
     CÓDIGO:  Q0589-Master
     AUTOR:   BrincandoComIdeias
     LINK:    https://www.youtube.com/brincandocomideias ; https://cursodearduino.net/ ; https://cursoderobotica.net
     COMPRE:  https://www.arducore.com.br/
     SKETCH:  Integrando Variaveis entre 2 Arduinos - Esp01 Master
     DATA:    26/01/2020
*/

// INCLUSÃO DE BIBLIOTECAS
#include <A2a.h>
#include "config.h"

// DEFINIÇÕES
#define endereco 0x08
#define tempoAtualizacao 10000

// INSTANCIANDO OBJETOS
AdafruitIO_Feed *umidademin = io.feed("umidademin");
AdafruitIO_Feed *temporega = io.feed("temporega");
AdafruitIO_Feed *umidademinat = io.feed("umidademinat");
AdafruitIO_Feed *temporegaat = io.feed("temporegaat");
AdafruitIO_Feed *confirma = io.feed("confirma");


A2a arduinoSlave;

// DECLARAÇÃO DE FUNÇÕES
void configuraMQTT();
void retornoUmidadeMin(AdafruitIO_Data *data);
void retornoTempoRega(AdafruitIO_Data *data);
void retornoConfirma(AdafruitIO_Data *data);
bool monitoraUmidadeAt();
bool monitoraTempodeRegaAt();

// DECLARAÇÃO DE VARIÁVEIS
unsigned long controleTempo = 0;
bool comandoRecebido = false;
unsigned int _umidademinat;
unsigned int _temporegaat;

void setup() {
  Serial.begin(9600);
  while (! Serial);

  configuraMQTT();

  arduinoSlave.begin(0, 2);

  Serial.println("Atualizando valor do Display de LED");
  
  umidademin->get();
  temporega->get();
  confirma->get();
  io.run();
  
  Serial.println("Fim Setup");
}

void loop() {
  io.run();

  //Monitora o sensor 
  if (millis() > controleTempo + tempoAtualizacao) {
    if (monitoraUmidadeAt()) {  // Precisa verificar cada 1
      controleTempo = millis();
      umidademinat->save(_umidademinat);
    }
    if (monitoraTempoRegaAt()) {
      controleTempo = millis();
      temporegaat->save(_temporegaat);
    }
  }
}

// IMPLEMENTO DE FUNÇÕES
void configuraMQTT() {
  Serial.print("Conectando ao Adafruit IO");
  io.connect();

  umidademin->onMessage(retornoUmidadeMin);
  temporega->onMessage(retornoTempoRega);
  confirma->onMessage(retornoConfirma);

  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println(io.statusText());
}

void retornoUmidadeMin(AdafruitIO_Data *data) {
  Serial.print("Controle Recebido <- ");  
  Serial.println(data->value());
  
  arduinoSlave.varWireWrite(endereco, 4, byte(data->toInt()));
}
void retornoTempoRega(AdafruitIO_Data *data) {
  Serial.print("Controle Recebido <- ");  
  Serial.println(data->value());
  
  arduinoSlave.varWireWrite(endereco, 5, byte(data->toInt()));
}
void retornoConfirma(AdafruitIO_Data *data) {
  Serial.print("Controle Recebido <- ");  
  Serial.println(data->value());
  
  arduinoSlave.varWireWrite(endereco, 6, byte(data->toInt()));
}

bool monitoraTempoRegaAt() {
  static int leituraAnt;

  byte byte1 = arduinoSlave.varWireRead(endereco, 0);
  byte byte2 = arduinoSlave.varWireRead(endereco, 1);

  unsigned int leitura = byte1 << 8 | byte2;

  if (leitura != leituraAnt) {

    _temporegaat = leitura;

    Serial.print("Temp: ");
    Serial.println(_temporegaat);

    leituraAnt = leitura;
    return true;
  } else {
    return false;
  }
}
bool monitoraUmidadeAt() {
  static int leituraAnt;

  byte byte1 = arduinoSlave.varWireRead(endereco, 2);
  byte byte2 = arduinoSlave.varWireRead(endereco, 3);

  unsigned int leitura = byte1 << 8 | byte2;

  if (leitura != leituraAnt) {

    _umidademinat = leitura;

    Serial.print("Umid: ");
    Serial.println(_umidademinat);

    leituraAnt = leitura;
    return true;
  } else {
    return false;
  }
}
