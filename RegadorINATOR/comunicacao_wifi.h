#ifndef COMUNICACAO_WIFI_H
#define COMUNICACAO_WIFI_H

#include "comunicacao.h"
#include <A2a.h>

class ComunicacaoWifi: public Comunicacao {
  public:
    ComunicacaoWifi();
    void feedback( int umidade, int tempoRega);
    int mudaParametroUmidade();
    int mudaParametroTempo();
    bool confirmacaoEmparelhamento();
  private:
};

extern A2a arduinoMaster;
#endif // COMUNICACAO_WIFI_H
