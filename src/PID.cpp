#include <PID.hpp>

PID::PID() {
    referencia = 0.0;
    Kp = 0.0;  // Ganho Proporcional
    Ki = 0.0;  // Ganho Integral
    Kd = 0.0;  // Ganho Derivativo
    T = 1.0;      // Período de Amostragem (ms)
    erro_total = 0.0; 
    erro_anterior = 0.0;
    sinal_de_controle_MAX = 100.0;
    sinal_de_controle_MIN = -100.0;
}

PID::~PID() = default;

void PID::pid_configura_constantes(double Kp_, double Ki_, double Kd_){
    Kp = Kp_;
    Ki = Ki_;
    Kd = Kd_;
}

void PID::pid_atualiza_referencia(float referencia_){
    referencia = (double) referencia_;
}

double PID::pid_controle(double saida_medida){

    double erro = referencia - saida_medida;

    erro_total += erro; // Acumula o erro (Termo Integral)

    if (erro_total >= sinal_de_controle_MAX) 
    {
        erro_total = sinal_de_controle_MAX;
    }
    else if (erro_total <= sinal_de_controle_MIN) 
    {
        erro_total = sinal_de_controle_MIN;


    double delta_error = erro - erro_anterior; // Diferença entre os erros (Termo Derivativo)

    sinal_de_controle = Kp*erro + (Ki*T)*erro_total + (Kd/T)*delta_error; // PID calcula sinal de controle

    if (sinal_de_controle >= sinal_de_controle_MAX) 
    {
        sinal_de_controle = sinal_de_controle_MAX;
    }
    else if (sinal_de_controle <= sinal_de_controle_MIN) 
    {
        sinal_de_controle = sinal_de_controle_MIN;
    }

    erro_anterior = erro;

    return sinal_de_controle;
}