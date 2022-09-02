#include <Controle.hpp>

    bool ligarGLOBAL = true;

Controle::Controle() {

    estaLigado = false;
    estaIniciado = false;
    aumentarTempo = false;
    diminuirTempo = false;
    subirTemperatura = false;
    diminuirTemperatura = false;
    menu = false;
    lerUsuario = 0;
    tempoAtual = 0;
    sinalControle = 0.0;

    pidControle.pid_configura_constantes(30.0, 0.2, 425.0);

    comunicacao.enviarEstado(0);

    comunicacao.enviarTemporizador(0);

    sleep(1);


}

Controle::~Controle() = default;

void Controle::init() {
    lerComandoUsuario();                // 500 ms
    interpretarComandos();

    if( estaLigado ) {

        if( estaLigado && estaIniciado ) {
            recebeValorTemperaturas();  // 500ms
            controleTemperatura();      // 0 ms
            contadorTempo();            // 0 ms
        }
    }

    system("clear");
}

void Controle::lerComandoUsuario() {
    comunicacao.solicitacao(Comms_MODBUS::LER_USUARIO);
    this->lerUsuario = comunicacao.getComandoDoUsuario();
}

void Controle::interpretarComandos() {
    switch (this->lerUsuario)
    {
    case 1:
        this->estaLigado = true;
        comunicacao.enviarEstado(true);
        tela.set_mensagemAcima16(std::string("      MODO:     "));
        tela.set_mensagemAbaixo16(std::string("      MANUAL    "));
        break;
    case 2:
        estadoDesligado();
        break;
    case 3:
        this->estaIniciado = true;
        comunicacao.enviarFuncionamento(true);
        break;
    case 4:
        this->estaIniciado = false;
        comunicacao.enviarFuncionamento(false);
        break;
    case 5:
        this->aumentarTempo = true;
        tempoAtual += 60;
        comunicacao.enviarTemporizador(tempoAtual);
        break;
    case 6:
        this->diminuirTempo = true;
        tempoAtual -= 60;
        comunicacao.enviarTemporizador(tempoAtual);        
        break;
    case 7:
        this->menu = true;
        break;
    default:
        break;
    }
}

void Controle::contadorTempo() {
    if (estaIniciado) {
        if( tempoAtual > 0 && atingiuTemp) {
            tempoAtual--;
        }

        if( temperaturaInterna<=temperaturaAmbiente && temperaturaReferencia == temperaturaAmbiente ) {
            estadoDesligado();
        }
        comunicacao.enviarTemporizador(tempoAtual); 
    }
}

void Controle::recebeValorTemperaturas() {

    comunicacao.solicitacao(Comms_MODBUS::SOLICITACAO_TEMP_INTERNA);
    comunicacao.solicitacao(Comms_MODBUS::SOLICITACAO_TEMP_REFERENCIA);
    temperaturaAmbiente = int(tempExterna.pegarValorTemperatura());
    std::cout << "Temperatura Ambiente: " << tempExterna.pegarValorTemperatura() << '\n';

    temperaturaInterna = comunicacao.getTemperaturaInterna();

    if(tempoAtual > 0 ) {
        temperaturaReferencia = comunicacao.getTemperaturaReferencia();
    } else {
        temperaturaReferencia = temperaturaAmbiente;
    }

    std::string escreveTemperatura;
    std::string escreveTempo;

    if(temperaturaInterna>=10) {
    escreveTemperatura.append("TI: ");
    } else {
    escreveTemperatura.append("TI: 0");
    }
    escreveTemperatura.append(std::to_string(temperaturaInterna));

    if(temperaturaReferencia>=10) {
    escreveTemperatura.append(" TR: ");
    } else {
    escreveTemperatura.append(" TR: 0");
    }
    escreveTemperatura.append(std::to_string(temperaturaReferencia));

    if(tempoAtual>=10) {
    escreveTempo.append("Timer: ");
    } else {
    escreveTempo.append("Timer: 0");
    }
    escreveTempo.append(std::to_string(tempoAtual));
    
    tela.set_mensagemAcima16(escreveTemperatura);
    tela.set_mensagemAbaixo16(escreveTempo);

    tela.mostrarMensagem();

}

void Controle::controleTemperatura() {

    pidControle.pid_atualiza_referencia(float(temperaturaReferencia));
    sinalControle = pidControle.pid_controle(double(temperaturaInterna));

    if (sinalControle > 0) {
        resistencia.inserirValorPwm(int(sinalControle));
        ventoinha.inserirValorPwm(0);
    } else 
        if (sinalControle < 0) {
            if(int(sinalControle) > -40.0 && int(sinalControle) < 0)
                ventoinha.inserirValorPwm(40);
            else
                ventoinha.inserirValorPwm(abs(int(sinalControle)));
        resistencia.inserirValorPwm(0);
    } else {
        resistencia.inserirValorPwm(0);
        ventoinha.inserirValorPwm(0);
    }

    comunicacao.enviar(int(sinalControle));

    if( temperaturaInterna >= temperaturaReferencia )
        atingiuTemp = true;
    else 
        atingiuTemp = false;
}

void Controle::estadoDesligado() {
    tela.set_mensagemAcima16(std::string("   Desligando!  "));
    tela.set_mensagemAbaixo16(std::string("   Aguarde...   "));

    sleep(3);

    estaLigado = false;
    estaIniciado = false;
    tempoAtual = 0;
    comunicacao.enviarEstado(0);
    comunicacao.enviarFuncionamento(false);
    comunicacao.enviarTemporizador(0);
    comunicacao.enviar(0);
    tela.ClrLcd();
    ligarGLOBAL = false;
}