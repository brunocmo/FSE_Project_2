#include <Controle.hpp>

bool ligarGLOBAL = true;

bool executar = true;

void tratarSinal(int s){
    std::cout << "Sinal: " << s << " adquirido!" << '\n';
	std::cout << "Interrupção, acabando com o programa corretamente..." << '\n';
	sleep(1);
	executar = false;
}

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
    temperaturaAmbiente = 0.0f;
    temperaturaInterna = 0.0f;
    temperaturaReferencia = 0.0f;
    modoOperacao = 0;
    tempTimer = 0;

    pidControle.pid_configura_constantes(30.0, 0.2, 400.0);

    comunicacao.enviarEstado(0);

    comunicacao.enviarTemporizador(0);

    signal(SIGINT, tratarSinal);
    signal(SIGTERM, tratarSinal);
    signal(SIGHUP, tratarSinal);

    system("clear");
}

Controle::~Controle() = default;

void Controle::init() {

    if (executar) {

    lerComandoUsuario();                // 600 ms
    interpretarComandos();

    if( estaLigado ) {
        if( estaIniciado ) {
            recebeValorTemperaturas();  // 1200 ms
            controleTemperatura();      // 0 ms
            contadorTempo();            // 0 ms
            registraValores();
        } else 
            mudarModo();
    }

    } else estadoDesligado();
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
        estadoDesligado();
        break;
    case 3:
        this->estaIniciado = true;
        comunicacao.enviarFuncionamento(true);
        tela.ClrLcd();
        break;
    case 4:
        acabouOperacao();
        break;
    case 5:
        if(estaLigado) {
        this->aumentarTempo = true;
        tempoAtual += 60;
        comunicacao.enviarTemporizador(tempoAtual/60);
        }
        break;
    case 6:
        if(estaLigado) {
        this->diminuirTempo = true;
        tempoAtual -= 60;
        comunicacao.enviarTemporizador(tempoAtual/60);   
        }     
        break;
    case 7:
        if(estaLigado)
        modoOperacao++;
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

        if( int(floor(temperaturaInterna))<=int(temperaturaAmbiente) && int(temperaturaReferencia) == int(temperaturaAmbiente) ) {
            acabouOperacao();
        }
        comunicacao.enviarTemporizador(tempoAtual/60); 
    }
}

void Controle::recebeValorTemperaturas() {

    comunicacao.solicitacao(Comms_MODBUS::SOLICITACAO_TEMP_INTERNA);
    
    temperaturaAmbiente = tempExterna.pegarValorTemperatura();
    temperaturaInterna = comunicacao.getTemperaturaInterna();

    if(tempoAtual <= 0 ) {
        temperaturaReferencia = temperaturaAmbiente;
    } else {
        if ( modoOperacao == 0) {
            tempTimer++;
            if(tempTimer > 2){
            comunicacao.solicitacao(Comms_MODBUS::SOLICITACAO_TEMP_REFERENCIA);
            temperaturaReferencia = comunicacao.getTemperaturaReferencia();
            
            tempTimer = 0;
            }
        }
    }

    std::string escreveTemperatura;
    std::string escreveTempo;

    if(int(temperaturaInterna)>=10) {
    escreveTemperatura.append("TI: ");
    } else {
    escreveTemperatura.append("TI: 0");
    }
    escreveTemperatura.append(std::to_string(int(temperaturaInterna)));

    if(int(temperaturaReferencia)>=10) {
    escreveTemperatura.append(" TR: ");
    } else {
    escreveTemperatura.append(" TR: 0");
    }
    escreveTemperatura.append(std::to_string(int(temperaturaReferencia)));

    if(tempoAtual >= 100) {
        escreveTempo.append("Timer(s): ");
    } else {
        if(tempoAtual>=10) {
        escreveTempo.append("Timer(s): 0");
        } else {
        escreveTempo.append("Timer(s): 00");
        }
    }
    escreveTempo.append(std::to_string(tempoAtual));
    
    tela.set_mensagemAcima16(escreveTemperatura);
    tela.set_mensagemAbaixo16(escreveTempo);

    tela.mostrarMensagem();

}

void Controle::controleTemperatura() {

    pidControle.pid_atualiza_referencia(temperaturaReferencia);
    sinalControle = pidControle.pid_controle(double(temperaturaInterna));

    if (sinalControle > 0) {
        valorResistor = int(sinalControle);
        valorVentoinha = 0;
    } else {
        if (sinalControle < 0) {
            if(int(sinalControle) > -40.0 && int(sinalControle) < 0)
                valorVentoinha = 40;   
            else
                valorVentoinha = abs(int(sinalControle));
            valorResistor = 0;

        } else {
            valorResistor = 0;
            valorVentoinha = 0;
        }
    }

    resistencia.inserirValorPwm(valorResistor);
    ventoinha.inserirValorPwm(valorVentoinha);

    comunicacao.enviar(int(sinalControle));

    if( int(ceil(temperaturaInterna)) >= int(temperaturaReferencia) )
        atingiuTemp = true;
    else 
        atingiuTemp = false;
}

void Controle::estadoDesligado() {
    tela.set_mensagemAcima16(std::string("   Desligando!  "));
    tela.set_mensagemAbaixo16(std::string("   Aguarde...   "));

    tela.mostrarMensagem();

    sleep(1);

    estaLigado = false;
    estaIniciado = false;
    tempoAtual = 0;
    comunicacao.enviarEstado(0);
    comunicacao.enviarFuncionamento(false);
    comunicacao.enviarTemporizador(0);
    comunicacao.enviar(0);
    tela.ClrLcd();
    resistencia.inserirValorPwm(0);
    ventoinha.inserirValorPwm(0);
    ligarGLOBAL = false;
}

void Controle::acabouOperacao() {
    estaLigado = true;
    estaIniciado = false;
    tempoAtual = 0;
    comunicacao.enviarFuncionamento(false);
    comunicacao.enviarTemporizador(0);
    comunicacao.enviar(0);
    tela.ClrLcd();
    resistencia.inserirValorPwm(0);
    ventoinha.inserirValorPwm(0);
}

void Controle::registraValores() {
    		auto comecaCronometro = std::chrono::system_clock::now();
			auto comeca_tempo = std::chrono::system_clock::to_time_t(comecaCronometro);

            std::time(&comeca_tempo);
			timeinfo = std::localtime(&comeca_tempo);

    		std::strftime( tempoString, 18, "%d/%m/%y %T", timeinfo );

			std::printf("%s -> Temp.Interna: %.2f deg C Temp.Externa: %.2f deg C Temp.Rerencia: %.2f deg C Resistor: %3d%% Ventoinha: %3d%% \n", 
				tempoString,
				temperaturaInterna,
				temperaturaAmbiente,
				temperaturaReferencia,
				valorResistor,
				valorVentoinha
			);

			registro.set_dataHora(tempoString);
			registro.set_tempInterna(temperaturaInterna);
			registro.set_tempExterna(temperaturaAmbiente);
			registro.set_tempReferencia(temperaturaReferencia);
			registro.set_valorPotenciometro(sinalControle);
			registro.registrarInformacoes();
}

void Controle::mudarModo() {

    tela.set_mensagemAcima16(std::string("     MODO:     "));

    if(modoOperacao > 2) {
        modoOperacao = 0;
        tempoAtual = 0;
    };

    switch (modoOperacao)
    {
    case 0:
        tela.set_mensagemAbaixo16(std::string("     MANUAL    "));
        comunicacao.solicitacao(Comms_MODBUS::SOLICITACAO_TEMP_REFERENCIA);
        temperaturaReferencia = comunicacao.getTemperaturaReferencia();

        break;
    case 1:
        tela.set_mensagemAbaixo16(std::string("     PIPOCA    "));
        temperaturaReferencia = 45;
        tempoAtual = 90;
        break;
    case 2:
        tela.set_mensagemAbaixo16(std::string("  DESCONGELAR  "));
        temperaturaReferencia = 80;
        tempoAtual = 300;
        break;            
    default:
        modoOperacao = 0;
        break;
    }

    tela.mostrarMensagem();


}