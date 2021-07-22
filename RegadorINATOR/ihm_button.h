#ifndef IHM_BUTTON_H_INCLUDED
#define IHM_BUTTON_H_INCLUDED

#include "ihm.h"

#define NENHUMA_TECLA -1

class InterfaceHomemMaquinaButton: public InterfaceHomemMaquina {
  public:
  InterfaceHomemMaquinaButton();
  void LED(int porta);
  int button(int porta);

  private:
};

#endif // IHM_BUTTON_H_INCLUDED
