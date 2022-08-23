#include <iostream>
#include "ShowInfoLCD.hpp"
#include "unistd.h"

int main() {

  std::cout << "Hello World!!!!" << '\n';

  ShowInfoLCD tela;

  tela.set_mensagemAcima16("Ola mundo");
  tela.set_mensagemAbaixo16("Boa noite!");

  sleep(1);

  tela.mostrarMensagem();

  sleep(1); 

  tela.ClrLcd();

  sleep(1);

  return 0;

}
