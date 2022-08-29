#ifndef FORNO_HPP
#define FORNO_HPP

#include <softPwm.h>
#include <wiringPi.h>
#include <unistd.h>
#include <iostream>

class Forno {

    int piSetup;
    int pino;

    public:
        Forno();
        Forno(int valorPino);
        ~Forno();

        void init();

        int getPino();
        
        void setPino(int valorPino);

        void inserirValorPwm(int valor); 

        void pararPwm();
};

#endif