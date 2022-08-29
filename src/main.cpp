#include "ShowInfoLCD.hpp"
#include <bme280.h>
#include <TemperaturaExterna.hpp>
#include <FornoResistencia.hpp>
#include <FornoVentoinha.hpp>
#include "Comms_MODBUS.hpp"

#include "unistd.h"
#include <iostream>

int main() {

  // std::cout << "Hello World!!!!" << '\n';

  // ShowInfoLCD tela;

  // tela.set_mensagemAcima16("21.1");
  // tela.set_mensagemAbaixo16("Boa tarde!");

  // sleep(1);

  // tela.mostrarMensagem();

  // sleep(1);

  // Comms_MODBUS comunicacao;

  // comunicacao.solicitacao(Comms_MODBUS::SOLICITACAO_TEMP_INTERNA);
  // // comunicacao.enviar(14);
  // // comunicacao.enviar(35.87f);

  // comunicacao.enviarControle(false);
  // // comunicacao.enviar(16.18f);

  // TemperaturaExterna tempExterna;

  try {

  std::cout << "Progamei a resistencia" << '\n';
  FornoResistencia tentoSerBom;
  std::cout << "Progamei a ventoinha" << '\n';
  FornoVentoinha precisoSerBom;

  std::cout << "Liguei a resistencia" << '\n';
  tentoSerBom.inserirValorPwm(100);

  sleep(5);

  tentoSerBom.inserirValorPwm(0);
  std::cout << "Parei a resistencia" << '\n';


  std::cout << "Liguei a Ventoinha" << '\n';

  precisoSerBom.inserirValorPwm(100);

  sleep(5);

  precisoSerBom.inserirValorPwm(0);
  std::cout << "Parei a resistencia" << '\n';


  std::cout << "Finalizei!" << '\n';

  } catch(const char* exception) {
    std::cerr << exception << '\n';
  }

  return 0;

}
