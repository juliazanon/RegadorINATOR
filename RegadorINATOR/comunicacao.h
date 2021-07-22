#ifndef COMUNICACAO_H_INCLUDED
#define COMUNICACAO_H_INCLUDED

class Comunicacao {
  public:
  /************************
   feedback
  Envia mensagem para a Central
  entradas
    int: umidade e tempo de rega
  saidas
    nenhuma
  *************************/
  virtual void feedback(int umidade, int tempoRega) = 0;
  /************************
   mudaParametroUmidade
  Muda a variavel de umidade
  entradas
    nenhuma
  saidas
    umidade
  *************************/
  virtual int mudaParametroUmidade() = 0;
  /************************
   mudaParametroTempo
  Muda a variavel de umidade
  entradas
    nenhuma
  saidas
    tempo de rega
  *************************/
  virtual int mudaParametroTempo() = 0;
  /************************
  confirmacaoEmparelhamento
  Muda do estado de emparelhamento para idle
  entradas
    nenhuma
  saidas
    true se for confirmado
  *************************/
  virtual bool confirmacaoEmparelhamento() = 0;
};

#endif // COMUNICACAO_H_INCLUDED
