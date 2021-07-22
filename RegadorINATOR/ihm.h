#ifndef IHM_H_INCLUDED
#define IHM_H_INCLUDED

#define NENHUMA_TECLA -1

class InterfaceHomemMaquina {
  public:
    /************************
   LED
  LEDs para debug
  entradas
    inteiro: recebe o número da porta
  saidas
    nenhuma
  *************************/
  virtual void LED(int porta) = 0;

  /************************
   button
  Percebe o acionamento do botão
  entradas
    inteiro: recebe número da porta
  saídas
    nenhuma
  *************************/
  virtual int button(int porta) = 0;
};

#endif // IHM_H_INCLUDED
