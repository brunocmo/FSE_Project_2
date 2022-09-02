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

    pidControle.pid_configura_constantes(30.0, 0.2, 200.0);

    comunicacao.enviarEstado(0);

    comunicacao.enviarTemporizador(0);

    sleep(1);


}

Controle::~Controle() = {
    comunicacao.enviarEstado(0);
    comunicacao.enviarFuncionamento(false);
    comunicacao.enviarTemporizador(0);
    comunicacao.enviar(0);
};

void Controle::init() {
    lerComandoUsuario();
    interpretarComandos();
    recebeValorTemperaturas();

    if( estaLigado && estaIniciado ) {
        controleTemperatura();
        contadorTempo();
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
        break;
    case 2:
        this->estaLigado = false;
        comunicacao.enviarEstado(false);
        ligarGLOBAL = false;
        break;
    case 3:
        this->estaIniciado = true;
        comunicacao.enviarFuncionamento(true);
        pidControle.pid_atualiza_referencia(temperaturaReferencia);
        break;
    case 4:
        this->estaIniciado = false;
        comunicacao.enviarFuncionamento(false);
        break;
    case 5:
        this->aumentarTempo = true;
        tempoAtual += 1;
        comunicacao.enviarTemporizador(tempoAtual);
        break;
    case 6:
        this->diminuirTempo = true;
        tempoAtual -= 1;
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

        if( tempoAtual == 0 && temperaturaReferencia != 30) {
            pidControle.pid_atualiza_referencia(30.0);
            temperaturaReferencia = 30;
        }

        if( temperaturaInterna<30 && temperaturaReferencia == 30 ) {
            estaIniciado = false;
            comunicacao.enviarFuncionamento(false);
            ligarGLOBAL = false;
        }
        comunicacao.enviarTemporizador(tempoAtual); 
    }
}

void Controle::recebeValorTemperaturas() {

    comunicacao.solicitacao(Comms_MODBUS::SOLICITACAO_TEMP_INTERNA);
    comunicacao.solicitacao(Comms_MODBUS::SOLICITACAO_TEMP_REFERENCIA);

    temperaturaInterna = comunicacao.getTemperaturaInterna();

    if(tempoAtual > 0 ) {
        temperaturaReferencia = comunicacao.getTemperaturaReferencia();
    }

    std::string escreveTemperatura;
    std::string escreveTempo;

    escreveTemperatura.append("TI: ");
    escreveTemperatura.append(std::to_string(temperaturaInterna));
    escreveTemperatura.append(" TR: ");
    escreveTemperatura.append(std::to_string(temperaturaReferencia));

    escreveTempo.append("Timer: ");
    escreveTempo.append(std::to_string(tempoAtual));
    
    tela.ClrLcd();

    tela.set_mensagemAcima16(escreveTemperatura);
    tela.set_mensagemAbaixo16(escreveTempo);

    tela.mostrarMensagem();

}

void Controle::controleTemperatura() {

    sinalControle = pidControle.pid_controle(temperaturaInterna);

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