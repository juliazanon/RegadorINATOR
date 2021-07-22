#include <Arduino.h>

#include "definicoes_sistema.h"
#include "ponteH_bomba.h"

PonteHBomba::PonteHBomba() : acionado(false)
{
}

/************************
 SireneSerial::acionar
 Aciona ou desaciona a sirene
 entradas
   controle: TRUE:ligar FALSE:desligar
 saidas
   nenhuma
*************************/
void PonteHBomba::acionar(int controle,int porta)
{
    acionado = controle;
    if (controle){
      digitalWrite(porta,HIGH);
    }
    else {
      digitalWrite(porta,LOW);
    }
}
