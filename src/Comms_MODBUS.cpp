#include "../inc/Comms_MODBUS.hpp"

Comms_MODBUS::Comms_MODBUS() {
    matricula[0] = 7;
    matricula[1] = 5;
    matricula[2] = 4;
    matricula[3] = 8;
}

Comms_MODBUS::~Comms_MODBUS() = default;

bool Comms_MODBUS::init() {

    uart0_filestream = -1;

    uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY, O_NDELAY);
    if(uart0_filestream == -1) {
        std::cout << "Erro - Não foi possível iniciar a UART" << '\n';
        throw("Wtf");
    } else {
        std::cout << "UART inicializada!" << '\n';
    }

    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);

    return EXIT_SUCCESS;
}

bool Comms_MODBUS::exit() {
    close(uart0_filestream);
    std::cout << "UART desligada!" << '\n';

    return EXIT_SUCCESS;
}

void Comms_MODBUS::solicitacao(unsigned char codigoProtocolo) {
    init();
    p_tx_buffer = &tx_buffer[0];
    *p_tx_buffer++ = ESP32;   
    *p_tx_buffer++ = PEDIR;
    *p_tx_buffer++ = codigoProtocolo;
    memcpy(p_tx_buffer, matricula, 4);
    p_tx_buffer += 4;

    crcValue = calcula_CRC( &tx_buffer[0], 7);
    memcpy(crcMessage, &crcValue, sizeof(short));

    *p_tx_buffer++ = crcMessage[0];
    *p_tx_buffer++ = crcMessage[1];


    // for(int i{0}; i<9; i++) {
    //     std::cout << std::hex << int(tx_buffer[i]) << '\n';
    // }

    count = write(uart0_filestream, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));
    std::cout << (count < 0 ? "UART TX error" : "Mandei a mensagem!") << '\n';

    sleep(1);

    rx_length = read(uart0_filestream, (void*)&rx_buffer, 255);
    if(rx_length < 0) {
        std::cout << "Erro na leitura" << '\n';
    } else if(rx_length == 0) {
        std::cout << "Nenhum dado disponível." << '\n';
    } else {
        rx_buffer[rx_length] = '\0';
        
        std::cout << "Recebi a mensagem! com " << rx_length << " chars! " <<  '\n';

        if( validarCRC(rx_buffer, rx_length-2)) throw("CRC invalido!");

    switch (codigoProtocolo)
        {
        case SOLICITACAO_TEMP_INTERNA:
            std::cout << 
                "Retornou a temperatura interna: " << 
                convertBufferFloat(rx_buffer) << 
            '\n';
            break;    
        case SOLICITACAO_TEMP_REFERENCIA:
            std::cout << 
                "Retornou a temperatura de referencia: " << 
                convertBufferFloat(rx_buffer) << 
            '\n';
            break;    
        case LER_USUARIO:
            std::cout << 
                "Retornou o comando do usuario " << 
                convertBufferInteiro(rx_buffer) <<
            '\n';
            break;    
        default:
            std::cout << "Deu ruim ! :c " << '\n';
            break;
        }
    }
    exit();
}

void Comms_MODBUS::enviar(int valorEnviado) {
    unsigned char bufferLocal[4];
    memcpy(bufferLocal, &valorEnviado, sizeof(int));
    enviarBuffer(1, bufferLocal);
}

void Comms_MODBUS::enviar(float valorEnviado) {
    unsigned char bufferLocal[4];
    memcpy(bufferLocal, &valorEnviado, sizeof(float));
    enviarBuffer(2, bufferLocal);
}

void Comms_MODBUS::enviarEstado(bool estaLigado) {
    unsigned char bufferLocal;
    memcpy(&bufferLocal, &estaLigado, sizeof(bool));
    enviarBuffer(3, &bufferLocal);
}

void Comms_MODBUS::enviarControle(bool estaLigado) {
    unsigned char bufferLocal;
    memcpy(&bufferLocal, &estaLigado, sizeof(bool));
    enviarBuffer(4, &bufferLocal);
}

void Comms_MODBUS::enviarFuncionamento(bool estaLigado) {
    unsigned char bufferLocal;
    memcpy(&bufferLocal, &estaLigado, sizeof(bool));
    enviarBuffer(5, &bufferLocal);
}

void Comms_MODBUS::enviarTemporizador(bool estaLigado) {
    unsigned char bufferLocal;
    memcpy(&bufferLocal, &estaLigado, sizeof(bool));
    enviarBuffer(6, &bufferLocal);
}


int Comms_MODBUS::convertBufferInteiro(unsigned char *buffer) {
    int convertedValue;
    memcpy(&convertedValue, &buffer[3], sizeof(int));
    return convertedValue;
}

float Comms_MODBUS::convertBufferFloat(unsigned char *buffer) {
    float convertedValue;
    memcpy(&convertedValue, &buffer[3], sizeof(float));
    return convertedValue;

}

std::string Comms_MODBUS::convertBufferString(unsigned char *buffer) {
    int sizeBuffer{int(buffer[3])};
    std::string convertedValue((char *)(&buffer[4]), sizeBuffer);    

    return convertedValue;
}

bool Comms_MODBUS::enviarBuffer(int flag, unsigned char *buffer) {
    init();
    p_tx_buffer = &tx_buffer[0];
    *p_tx_buffer++ = ESP32;   
    *p_tx_buffer++ = ENVIAR;

    switch (flag)
    {
    case 1:
        *p_tx_buffer++ = ENVIAR_SINAL_CONTROLE;
        memcpy(p_tx_buffer, matricula, 4);
        p_tx_buffer += 4;
        memcpy(p_tx_buffer, buffer, sizeof(int));
        p_tx_buffer += sizeof(int);
        crcValue = calcula_CRC( &tx_buffer[0], 11);
        break;
    case 2:
        *p_tx_buffer++ = ENVIAR_SINAL_REFERENCIA;
        memcpy(p_tx_buffer, matricula, 4);
        p_tx_buffer += 4;
        memcpy(p_tx_buffer, buffer, sizeof(float));
        p_tx_buffer += sizeof(float);
        crcValue = calcula_CRC( &tx_buffer[0], 11);
        break;
    case 3:
        *p_tx_buffer++ = ENVIAR_ESTADO_SISTEMA;
        memcpy(p_tx_buffer, matricula, 4);
        p_tx_buffer += 4;
        *p_tx_buffer++ = buffer[0];
        crcValue = calcula_CRC( &tx_buffer[0], 8);
        break;

    case 4:
        *p_tx_buffer++ = ENVIAR_MODO_DE_CONTROLE;
        memcpy(p_tx_buffer, matricula, 4);
        p_tx_buffer += 4;
        *p_tx_buffer++ = buffer[0];
        crcValue = calcula_CRC( &tx_buffer[0], 8);
        break;
    case 5:
        *p_tx_buffer++ = ENVIAR_ESTADO_FUNCIONAMENTO;
        memcpy(p_tx_buffer, matricula, 4);
        p_tx_buffer += 4;
        *p_tx_buffer++ = buffer[0];
        crcValue = calcula_CRC( &tx_buffer[0], 8);
        break;
    case 6:
        *p_tx_buffer++ = ENVIAR_VALOR_TEMPORIZADOR;
        memcpy(p_tx_buffer, matricula, 4);
        p_tx_buffer += 4;
        *p_tx_buffer++ = buffer[0];
        crcValue = calcula_CRC( &tx_buffer[0], 8);
        break;
    
    default:
        throw("Erro Operacao -> modoEnvio");
        break;
    }

    memcpy(crcMessage, &crcValue, sizeof(short));

    *p_tx_buffer++ = crcMessage[0];
    *p_tx_buffer++ = crcMessage[1];

    count = write(uart0_filestream, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));
    std::cout << (count < 0 ? "UART TX error" : "Mandei a mensagem!") << '\n';

    sleep(1);

    if(flag == 3 || flag == 4 || flag == 5 || flag == 6) {

        rx_length = read(uart0_filestream, (void*)&rx_buffer, 255);
        if(rx_length < 0) {
            std::cout << "Erro na leitura" << '\n';
        } else if(rx_length == 0) {
            std::cout << "Nenhum dado disponível." << '\n';
        } else {
            rx_buffer[rx_length] = '\0';
            
            std::cout << "Recebi a mensagem!" << '\n';
            
            if( validarCRC(rx_buffer, rx_length-2)) throw("CRC invalido!");

            switch (flag)
            {
            case 3:
                std::cout << 
                    "Retornou um inteiro de estado " << 
                    convertBufferInteiro(rx_buffer) << 
                '\n';
                break;
            case 4:
                std::cout << 
                    "Retornou um inteiro de controle " << 
                    convertBufferInteiro(rx_buffer) << 
                '\n';
                break;
            case 5:
                std::cout << 
                    "Retornou um inteiro de funcionamento " << 
                    convertBufferInteiro(rx_buffer) << 
                '\n';
                break;
            case 6:
                std::cout << 
                    "Retornou um inteiro de temporizador " << 
                    convertBufferInteiro(rx_buffer) << 
                '\n';
                break;
            
            default:
                std::cout << "teste erro" << '\n';
                throw("Erro Operacao -> modoEnvio");
                break;
            }
        }

    }

    return exit();
}

bool Comms_MODBUS::validarCRC(unsigned char *bufferValidacao, size_t tamanho) {
    short crcCheck;

    crcValue = calcula_CRC( &bufferValidacao[0], tamanho);
    memcpy(&crcCheck, &bufferValidacao[tamanho], sizeof(short));

    if (crcCheck == crcValue) {
        std::cout << "CRC validada e aceita!" << '\n';
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }

}