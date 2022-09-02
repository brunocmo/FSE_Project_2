#ifndef PID_HPP
#define PID_HPP

#include <iostream>

class PID {

    double saida_medida;
    double sinal_de_controle;
    double referencia;
    double Kp;  // Ganho Proporcional
    double Ki;  // Ganho Integral
    double Kd;  // Ganho Derivativo
    int T;      // Período de Amostragem (ms)
    unsigned long last_time;
    double erro_total; 
    double erro_anterior;
    int sinal_de_controle_MAX;
    int sinal_de_controle_MIN;

    public:
        PID();
        ~PID();

    void pid_configura_constantes(double Kp_, double Ki_, double Kd_);
    void pid_atualiza_referencia(float referencia_);
    double pid_controle(double saida_medida);
};

#endif
