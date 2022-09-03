#ifndef CONTROLE_HPP
#define CONTROLE_HPP

#include <bme280.h>
#include <Comms_MODBUS.hpp>
#include <FornoResistencia.hpp>
#include <FornoVentoinha.hpp>
#include <PID.hpp>
#include <ShowInfoLCD.hpp>
#include <TemperaturaExterna.hpp>
#include <RegistrarInformacoes.hpp>
#include <chrono>
#include <ctime>
#include <stdio.h>
#include <csignal>
#include <cmath>

extern bool ligarGLOBAL;

extern bool executar;

void tratarSinal(int s);


class Controle {

    bool estaLigado;
    bool estaIniciado;

    bool aumentarTempo;
    bool diminuirTempo;

    bool subirTemperatura;
    bool diminuirTemperatura;

    bool menu;
    bool atingiuTemp;

    int lerUsuario;
    int tempoAtual;

    int tempTimer;

    float temperaturaInterna;
    float temperaturaReferencia;
    float temperaturaAmbiente;

    int valorVentoinha;
    int valorResistor;

    double sinalControle;

    int modoOperacao;

    char tempoString[18];
	struct tm * timeinfo;

    ShowInfoLCD tela;
    Comms_MODBUS comunicacao;
    FornoResistencia resistencia;
    FornoVentoinha ventoinha;
    PID pidControle;
    TemperaturaExterna tempExterna;
    RegistrarInformacoes registro;
    
    public:
        Controle();
        ~Controle();

        void init();
        void lerComandoUsuario();
        void interpretarComandos();

        void contadorTempo();
        void recebeValorTemperaturas();
        void controleTemperatura();

        void estadoDesligado();

        void registraValores();

        void acabouOperacao();

        void mudarModo();



};

#endif