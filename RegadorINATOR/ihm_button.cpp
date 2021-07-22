#include <Arduino.h>

#include "definicoes_sistema.h"
#include "ihm_button.h"

InterfaceHomemMaquinaButton::InterfaceHomemMaquinaButton()
{
}

/************************
 InterfaceHomemMaquinaSerial::LED
  LEDs para debug
  entradas
    inteiro: recebe o número da porta
  saidas
    nenhuma
*************************/
void InterfaceHomemMaquinaButton::LED(int porta)
{
    // Desliga os LEDs
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW); 

    // Liga o especificado
    digitalWrite(porta,HIGH);
}

  /************************
   button
  Percebe o acionamento do botão
  entradas
    inteiro: recebe número da porta
  saídas
    nenhuma
  *************************/
int InterfaceHomemMaquinaButton::button(int porta)
{
  if (digitalRead(porta) == LOW) {
    return  true;
  }
  else {
    return false;
  }
}
