#ifndef FORNOVENTOINHA_HPP
#define FORNOVENTOINHA_HPP

#include <unistd.h>
#include <iostream>

#include <Forno.hpp>

class FornoVentoinha : public Forno {

    const int pinoVentoinha{5};

    public:
        FornoVentoinha();
        ~FornoVentoinha();
};

#endif