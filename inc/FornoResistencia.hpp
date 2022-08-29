#ifndef FORNORESISTENCIA_HPP
#define FORNORESISTENCIA_HPP

#include <unistd.h>
#include <iostream>

#include <Forno.hpp>

class FornoResistencia : public Forno {

    const int pinoResistencia{4};

    public:
        FornoResistencia();
        ~FornoResistencia();
};

#endif