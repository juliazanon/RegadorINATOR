#ifndef PONTEH_H_INCLUDED
#define PONTEH_H_INCLUDED

class PonteH {
  public:
  /************************
   acionar
  Aciona ou desaciona a sirene
  entradas
    controle: TRUE:ligar FALSE:desligar
  saidas
    nenhuma
  *************************/
  virtual void acionar(int controle, int porta) = 0;

#endif // PONTEH_H_INCLUDED
