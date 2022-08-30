#ifndef CONTROLE_HPP
#define CONTROLE_HPP

#include <bme280.h>
#include <Comms_MODBUS.hpp>
#include <FornoResistencia.hpp>
#include <FornoVentoinha.hpp>
#include <PID.hpp>
#include <ShowInfoLCD.hpp>
#include <TemperaturaExterna.hpp>

extern bool ligarGLOBAL;

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

    int temperaturaInterna;
    int temperaturaReferencia;

    double sinalControle;

    ShowInfoLCD tela;
    Comms_MODBUS comunicacao;
    FornoResistencia resistencia;
    FornoVentoinha ventoinha;
    PID pidControle;
    
    public:
        Controle();
        ~Controle();

        void init();
        void lerComandoUsuario();
        void interpretarComandos();

        void contadorTempo();
        void recebeValorTemperaturas();
        void controleTemperatura();

};

#endif