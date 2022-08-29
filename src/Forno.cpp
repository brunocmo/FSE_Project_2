#include <Forno.hpp>

Forno::Forno() {
    piSetup = wiringPiSetup();
}

Forno::Forno(int valorPino) {
    piSetup = wiringPiSetup();

    setPino(valorPino);

    init();
}

Forno::~Forno() {
    std::cout << "Parei alguma coisa" << '\n';
    pararPwm();
}

int Forno::getPino() {
    return this->pino;
}

void Forno::setPino(int valorPino) {
    this->pino = valorPino;
}

void Forno::init() {
    int softPwmC = softPwmCreate( pino , 0, 100 );

    if (softPwmC) {
        throw("Erro da inicialização do pino");
    }
}

void Forno::inserirValorPwm(int valor) {
    softPwmWrite(pino, valor);
}

void Forno::pararPwm() {
    softPwmStop(pino);
}