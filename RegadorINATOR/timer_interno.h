#ifndef TIMER_INTERNO_H_INCLUDED
#define TIMER_INTERNO_H_INCLUDED

#include "timer.h"

class TimerInterno: public Timer {
  public:
  TimerInterno();
  void iniciar(int controle);
  int timeout(int tempoRega);

  private:
  int situacao;
  unsigned long horaInicio;
};

#endif // TIMER_INTERNO_H_INCLUDED
