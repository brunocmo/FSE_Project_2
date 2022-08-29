#ifndef COMMS_MODBUS_HPP
#define COMMS_MODBUS_HPP

#include <iostream>
#include <unistd.h>
#include <fcntl.h> 
#include <termios.h>
#include <cstring>
#include "crc16.h"
#include <stdlib.h>


class Comms_MODBUS {
    private:

        int uart0_filestream;
        struct termios options;

        unsigned char tx_buffer[255];
        unsigned char rx_buffer[255];
        unsigned char crcMessage[2];
        short crcValue;
        int rx_length;
        unsigned char *p_tx_buffer;
        int count;
        unsigned char matricula[4];

        bool init();
        bool exit();

        int convertBufferInteiro(unsigned char *buffer);
        float convertBufferFloat(unsigned char *buffer);
        std::string convertBufferString(unsigned char *buffer);

        bool enviarBuffer(int flag, unsigned char *buffer);

        bool validarCRC(unsigned char *bufferValidacao, size_t tamanho);

    public:
        static const unsigned char ESP32 = 0x01;

        static const unsigned char PEDIR = 0x23;
        static const unsigned char ENVIAR = 0x16;

        static const unsigned char SOLICITACAO_TEMP_INTERNA = 0xC1;
        static const unsigned char SOLICITACAO_TEMP_REFERENCIA = 0xC2;
        static const unsigned char LER_USUARIO = 0xC3;

        static const unsigned char ENVIAR_SINAL_CONTROLE = 0xD1;
        static const unsigned char ENVIAR_SINAL_REFERENCIA = 0xD2;
        static const unsigned char ENVIAR_ESTADO_SISTEMA = 0xD3;
        static const unsigned char ENVIAR_MODO_DE_CONTROLE = 0XD4;
        static const unsigned char ENVIAR_ESTADO_FUNCIONAMENTO = 0XD5;
        static const unsigned char ENVIAR_VALOR_TEMPORIZADOR = 0XD6;

        Comms_MODBUS();
        ~Comms_MODBUS();

        void solicitacao(unsigned char codigoProtocolo);
        void enviar(int valorEnviado);
        void enviar(float valorEnviado);
        void enviarEstado(bool estaLigado);
        void enviarControle(bool estaLigado);
        void enviarFuncionamento(bool estaLigado);
        void enviarTemporizador(bool estaLigado);
};

#endif