#include <iostream>
#include "ShowInfoLCD.hpp"
#include "Comms_MODBUS.hpp"
#include "unistd.h"

#include <bme280.h>
#include <TemperaturaExterna.hpp>

int main() {

  std::cout << "Hello World!!!!" << '\n';

  ShowInfoLCD tela;

  tela.set_mensagemAcima16("21.1");
  tela.set_mensagemAbaixo16("Boa tarde!");

  sleep(1);

  tela.mostrarMensagem();

  sleep(1);

  Comms_MODBUS comunicacao;

  comunicacao.solicitacao(Comms_MODBUS::SOLICITACAO_TEMP_INTERNA);
  // comunicacao.enviar(14);
  // comunicacao.enviar(35.87f);

  comunicacao.enviarControle(false);
  // comunicacao.enviar(16.18f);

  TemperaturaExterna tempExterna;

  std::cout << "Finalizei!" << '\n';

  return 0;

}
