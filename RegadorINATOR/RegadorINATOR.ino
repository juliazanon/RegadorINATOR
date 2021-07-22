/*
    ALARME RESIDENCIAL
*/

#include "definicoes_sistema.h"
#include "comunicacao_wifi.h"
#include "ihm_button.h"
#include "sensor_umidade.h"
#include "ponteH_bomba.h"
#include "timer_interno.h"
#include "task_switcher.h"
#include <A2a.h>

#define endereco 0x08
#define TASK_INTERVAL1 500
#define TASK_INTERVAL2 500
#define ButtonGRE 2
#define ButtonYEL 3
#define ButtonRED 4
#define Bomba 5
#define LEDgre 8  
#define LEDyel 9
#define LEDred 10
#define SensorUmid A0

/***********************************************************************
 Componentes
 ***********************************************************************/
ComunicacaoWifi com;
PonteHBomba phb;
InterfaceHomemMaquinaButton ihm;
SensorUmidade sensor;
TimerInterno tmr;

/***********************************************************************
 Estaticos
 ***********************************************************************/
int codigoEvento = NENHUM_EVENTO;
int eventoInterno = NENHUM_EVENTO;
int estado = Idle;
int codigoAcao;
int acao_matrizTransicaoEstados[NUM_ESTADOS][NUM_EVENTOS];
int proximo_estado_matrizTransicaoEstados[NUM_ESTADOS][NUM_EVENTOS];
int UmidadeMin = 35;  //% Default
int TempoRega = 3;  //Default

/************************************************************************
 executarAcao
 Executa uma acao
 Parametros de entrada:
    (int) codigo da acao a ser executada
 Retorno: (int) codigo do evento interno ou NENHUM_EVENTO
*************************************************************************/
int executarAcao(int codigoAcao)
{
    int retval;

    retval = NENHUM_EVENTO;
    if (codigoAcao == NENHUMA_ACAO)
        return retval;

    switch(codigoAcao)
    {
    case A01:
        Serial.print("Celular emparelhado com o arduino.\n");
        ihm.LED(LEDgre);
        break;
    case A02:
        Serial.print("Iniciar emparelhamento com celular.\n");
        ihm.LED(LEDyel);
        com.feedback(UmidadeMin,TempoRega);
        break;
    case A03:
        Serial.print("Umidade abaixo da mínima, inicializar rega.\n");
        phb.acionar(true, Bomba);
        tmr.iniciar(true);
        ihm.LED(LEDgre);
        break;
    case A04:
        Serial.print("Finalizar rega e voltar pro Idle.\n");
        ihm.LED(LEDgre);
        break;
    case A05:
        Serial.print("Umidade não mudou, falha na rega.\n");
        ihm.LED(LEDred);
        break;
    case A06:
        Serial.print("Tempo de rega concluído.\n");
        phb.acionar(false, Bomba);
        tmr.iniciar(false);
        ihm.LED(LEDgre);
        delay(3);
        break;
    case A07:
        Serial.print("Rega cancelada.\n");
        phb.acionar(false, Bomba);
        tmr.iniciar(false);
        ihm.LED(LEDred);
        break;
    case A08:
        Serial.print("Falha resolvida, voltar para o Idle.\n");
        ihm.LED(LEDgre);
        break;
    case A09:
        Serial.print("Mudou a umidade mínima. \n");
        ihm.LED(LEDyel);
        UmidadeMin = com.mudaParametroUmidade();
        delay(2);
        com.feedback(UmidadeMin,TempoRega);
        ihm.LED(LEDgre);
        break;
    case A10:
        Serial.print("Mudou o tempo de rega. \n");
        ihm.LED(LEDyel);
        TempoRega= com.mudaParametroTempo();
        delay(2);
        com.feedback(UmidadeMin,TempoRega);
        ihm.LED(LEDgre);
        break;
    } // switch

    return retval;
} // executarAcao

/************************************************************************
 iniciaMaquina de Estados
 Carrega a maquina de estados
 Parametros de entrada: nenhum
 Retorno: nenhum
*************************************************************************/
void iniciaMaquinaEstados()
{
  int i;
  int j;

  for (i=0; i < NUM_ESTADOS; i++) {
    for (j=0; j < NUM_EVENTOS; j++) {
       acao_matrizTransicaoEstados[i][j] = NENHUMA_ACAO;
       proximo_estado_matrizTransicaoEstados[i][j] = i;
    }
  }
  proximo_estado_matrizTransicaoEstados[Emparelhamento][emparelhando] = Idle;
  acao_matrizTransicaoEstados[Emparelhamento][emparelhando] = A01;
  
  proximo_estado_matrizTransicaoEstados[Idle][iniciar_emparelhamento] = Emparelhamento;
  acao_matrizTransicaoEstados[Idle][iniciar_emparelhamento] = A02;

  proximo_estado_matrizTransicaoEstados[Idle][umidade_abaixo] = Rega;
  acao_matrizTransicaoEstados[Idle][umidade_abaixo] = A03;

  proximo_estado_matrizTransicaoEstados[Fim_Rega][umidade_acima] = Idle;
  acao_matrizTransicaoEstados[Fim_Rega][umidade_acima] = A04;

  proximo_estado_matrizTransicaoEstados[Fim_Rega][umidade_manteve] = Falha_Rega;
  acao_matrizTransicaoEstados[Fim_Rega][umidade_manteve] = A05;

  proximo_estado_matrizTransicaoEstados[Rega][timer_rega] = Fim_Rega;
  acao_matrizTransicaoEstados[Rega][timer_rega] = A06;

  proximo_estado_matrizTransicaoEstados[Rega][cancelar] = Falha_Rega;
  acao_matrizTransicaoEstados[Rega][cancelar] = A07;

  proximo_estado_matrizTransicaoEstados[Falha_Rega][finalizar_falha] = Idle;
  acao_matrizTransicaoEstados[Falha_Rega][finalizar_falha] = A08;

  proximo_estado_matrizTransicaoEstados[Idle][parametro_tempo] = Idle;
  acao_matrizTransicaoEstados[Idle][parametro_tempo] = A10;

  proximo_estado_matrizTransicaoEstados[Idle][parametro_umid] = Idle;
  acao_matrizTransicaoEstados[Idle][parametro_umid] = A09;


} // initStateMachine

/************************************************************************
 iniciaSistema
 Inicia o sistema ...
 Parametros de entrada: nenhum
 Retorno: nenhum
*************************************************************************/
void iniciaSistema()
{
   iniciaMaquinaEstados();
} // initSystem


/************************************************************************
Funções decodificar
retornam true se ocorreu o evento
e falso caso contrário
*************************************************************************/
int decodificarTimeout(){
  return tmr.timeout(TempoRega);
}
int decodificarButtonGreen(){
  return ihm.button(ButtonGRE);  // Botão verde, numero da porta
}
int decodificarButtonYellow(){
  return ihm.button(ButtonYEL);
}
int decodificarButtonRed(){
  return ihm.button(ButtonRED);
}
int decodificarSensorAcima(){
  if (estado == Fim_Rega){
    if(sensor.Read(SensorUmid) > (UmidadeMin + 5) ){  // Tolerancia de 5%
      return true;
    }
    else {
      return false;
    } 
  }
  else{
    return false;
  }
}
int decodificarSensorAbaixo(){
  if (estado == Idle){
    if(sensor.Read(SensorUmid) < UmidadeMin){  // Chama sensor e compara com umidade mínima
      return true;
    }
    else {
      return false;
    } 
  }
  else{
    return false;
  }
}
int decodificarSensorManteve(){
  if (estado == Fim_Rega){
    if(sensor.Read(SensorUmid) < (UmidadeMin + 5) ){  // Tolerancia de 5%
      return true;
    }
    else {
      return false;
    } 
  }
  else{
    return false;
  }
}
int decodificarEmparelhar(){
  if (estado == Emparelhamento){
    return com.confirmacaoEmparelhamento();
  }
}
int decodificarParametroUmidade(){
  if (estado == Idle){
    if (com.mudaParametroUmidade() != UmidadeMin){
      return true;
    }
    else{
      return false; 
    }
  }
}
int decodificarParametroTempo(){
  if (estado == Idle){
    if (com.mudaParametroTempo() != TempoRega){
      return true;
    }
    else{
      return false; 
    }
  }
}

/************************************************************************
 obterAcao
 Obtem uma acao da Matriz de transicao de estados
 Parametros de entrada: estado (int)
                        evento (int)
 Retorno: codigo da acao
*************************************************************************/
int obterAcao(int estado, int codigoEvento) {
  return acao_matrizTransicaoEstados[estado][codigoEvento];
} // obterAcao


/************************************************************************
 obterProximoEstado
 Obtem o proximo estado da Matriz de transicao de estados
 Parametros de entrada: estado (int)
                        evento (int)
 Retorno: codigo do estado
*************************************************************************/
int obterProximoEstado(int estado, int codigoEvento) {
  return proximo_estado_matrizTransicaoEstados[estado][codigoEvento];
} // obterAcao



/***********************************************************************
 Tasks
 ***********************************************************************/

/************************************************************************
 taskMaqEstados
 Task principal de controle que executa a maquina de estados
 Parametros de entrada: nenhum
 Retorno: nenhum
*************************************************************************/
void taskMaqEstados() {
  if (eventoInterno != NENHUM_EVENTO) {
      codigoEvento = eventoInterno;
  }
  if (codigoEvento != NENHUM_EVENTO)
  {
      codigoAcao = obterAcao(estado, codigoEvento);
      estado = obterProximoEstado(estado, codigoEvento);
      eventoInterno = executarAcao(codigoAcao);
      Serial.print("Estado: ");
      Serial.print(estado);
      Serial.print(" Evento: ");
      Serial.print(codigoEvento);
      Serial.print(" Acao: ");
      Serial.println(codigoAcao);
  }
}

/************************************************************************
 taskObterEvento
 Task que faz pooling de eventos
 Parametros de entrada: nenhum
 Retorno: nenhum
*************************************************************************/
void taskObterEvento() {
  codigoEvento = NENHUM_EVENTO;

  if (decodificarTimeout()) {
    codigoEvento = timer_rega;
    return;
  }
  if (decodificarButtonGreen()) {
    codigoEvento = finalizar_falha;
    return;
  }
  if (decodificarButtonYellow()) {
    codigoEvento = iniciar_emparelhamento;
    return;
  }
  if (decodificarButtonRed()) {
    codigoEvento = cancelar;
    return;
  }
  if (decodificarSensorAbaixo()){
    codigoEvento = umidade_abaixo;
    return;
  }
  if (decodificarSensorAcima()){
    codigoEvento = umidade_acima;
    return;
  }
  if (decodificarSensorManteve()){
    codigoEvento = umidade_manteve;
    return;
  }
  if (decodificarParametroUmidade()){
    codigoEvento = parametro_umid;
    return;
  }
  if (decodificarParametroTempo()) {
    codigoEvento = parametro_tempo;
    return;
  }
  if (decodificarEmparelhar()){
    codigoEvento = emparelhando;
    return;
  }
}

/************************************************************************
 Funções de Conexões
 Configura a comunicação com o ESP015
*************************************************************************/
void receberDados() {
  arduinoMaster.receiveData(); 
}

void enviarDados() {
  arduinoMaster.sendData(); 
}
/************************************************************************
 Main
 Setup e Loop principal de controle que executa a maquina de estados
 Parametros de entrada: nenhum
 Retorno: nenhum
*************************************************************************/

void setup() {
  Serial.begin(9600);
  arduinoMaster.begin(endereco);
  arduinoMaster.onReceive(receberDados);
  arduinoMaster.onRequest(enviarDados);
  // Botões
  pinMode(ButtonGRE, INPUT_PULLUP); 
  pinMode(ButtonYEL, INPUT_PULLUP);
  pinMode(ButtonRED, INPUT_PULLUP);
  pinMode(Bomba, OUTPUT);
  digitalWrite(Bomba, LOW);
  pinMode(LEDgre, OUTPUT);
  pinMode(LEDyel, OUTPUT);
  pinMode(LEDred, OUTPUT);
  digitalWrite(LEDgre, HIGH);
  digitalWrite(LEDyel, LOW);
  digitalWrite(LEDred, LOW);
  
  // configure tasks
  TaskController.createTask(&taskMaqEstados, TASK_INTERVAL1);
  TaskController.createTask(&taskObterEvento, TASK_INTERVAL2);
  
  // set up timer interrupt 
  TaskController.begin(1000); // tick @1ms (1000 us)

  iniciaSistema();
  Serial.println("RegadorINATOR iniciado");
} // setup

void loop() {
  TaskController.runCurrentTask();
} // loop
