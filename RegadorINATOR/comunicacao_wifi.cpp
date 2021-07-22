#include <Arduino.h>
#include <A2a.h>

#include "definicoes_sistema.h"
#include "comunicacao_wifi.h"

#define endereco 0x08

A2a arduinoMaster;

ComunicacaoWifi::ComunicacaoWifi()
{
}

/************************
   feedback
  Envia mensagem para a Central
  entradas
    int: umidade e tempo de rega
  saidas
    nenhuma
  *************************/
void ComunicacaoWifi::feedback(int umidade, int tempoRega)
{
  arduinoMaster.varWireWrite(0,highByte(umidade));
  arduinoMaster.varWireWrite(1,lowByte(umidade));  // envia umidade mínima default
  arduinoMaster.varWireWrite(2,highByte(tempoRega));
  arduinoMaster.varWireWrite(3,lowByte(tempoRega));  // envia tempo rega default
}
/************************
   mudaParametroUmidade
  Muda a variavel de umidade
  entradas
    nenhuma
  saidas
    umidade
  *************************/
int ComunicacaoWifi::mudaParametroUmidade()
{
  int UmidadeNova = arduinoMaster.varWireRead(4);
  Serial.print("Umidadenova ");
  Serial.println(UmidadeNova);
  return UmidadeNova; 
}
/************************
   mudaParametroTempo
  Muda a variavel de umidade
  entradas
    nenhuma
  saidas
    tempo de rega
  *************************/
int ComunicacaoWifi::mudaParametroTempo()
{
  int TempoNovo = arduinoMaster.varWireRead(5);
  Serial.print("tempo novo ");
  Serial.println(TempoNovo);
  return TempoNovo;
}
/************************
  confirmacaoEmparelhamento
  Muda do estado de emparelhamento para idle
  entradas
    nenhuma
  saidas
    true se for confirmado
  *************************/
bool ComunicacaoWifi::confirmacaoEmparelhamento()
{
  int Confirmacao = arduinoMaster.varWireRead(6);
  Serial.print("Confirmação");
  Serial.println(Confirmacao);
  return Confirmacao;
}
