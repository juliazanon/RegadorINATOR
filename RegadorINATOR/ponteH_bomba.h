#ifndef PONTEH_BOMBA_H_INCLUDED
#define PONTEH_BOMBA_H_INCLUDED

#include "ponteH.h"

class PonteHBomba: public PonteH {
  public:
    PonteHBomba();
  	void acionar(int controle, int porta);

  private:
    int acionado;
};


#endif // PonteH_Bomba_H_INCLUDED
