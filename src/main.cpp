#include <iostream>
#include <Controle.hpp>

int main() {
  try {

    Controle airfryControl;

    std::cout << "Airfry inicializada." << '\n';

    while(ligarGLOBAL) {
      airfryControl.init();
    }

  } catch(const char* exception) {
    std::cerr << exception << '\n';
  }

  std::cout << "Airfry finalizada." << '\n';

  return 0;

}
