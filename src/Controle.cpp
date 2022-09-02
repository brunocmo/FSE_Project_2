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

    pidControle.pid_configura_constantes(30.0, 0.2, 500.0);

    comunicacao.enviarEstado(0);

    comunicacao.enviarTemporizador(0);

    signal(SIGINT, tratarSinal);
    signal(SIGTERM, tratarSinal);
    signal(SIGHUP, tratarSinal);
    
    sleep(1);


    system("clear");
}

Controle::~Controle() = default;

void Controle::init() {

    if (executar) {

    lerComandoUsuario();                // 500 ms
    interpretarComandos();

    if( estaLigado ) {

        if( estaLigado && estaIniciado ) {
            recebeValorTemperaturas();  // 500ms
            controleTemperatura();      // 0 ms
            contadorTempo();            // 0 ms
            registraValores();
        }
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

        if( int(temperaturaInterna)<=int(temperaturaAmbiente) && int(temperaturaReferencia) == int(temperaturaAmbiente) ) {
            estadoDesligado();
        }
        comunicacao.enviarTemporizador(tempoAtual); 
    }
}

void Controle::recebeValorTemperaturas() {

    comunicacao.solicitacao(Comms_MODBUS::SOLICITACAO_TEMP_INTERNA);
    comunicacao.solicitacao(Comms_MODBUS::SOLICITACAO_TEMP_REFERENCIA);
    temperaturaAmbiente = tempExterna.pegarValorTemperatura();
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
    escreveTemperatura.append(std::to_string(int(temperaturaInterna)));

    if(temperaturaReferencia>=10) {
    escreveTemperatura.append(" TR: ");
    } else {
    escreveTemperatura.append(" TR: 0");
    }
    escreveTemperatura.append(std::to_string(int(temperaturaReferencia)));

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

    if( int(temperaturaInterna) >= int(temperaturaReferencia) )
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
    resistencia.inserirValorPwm(0);
    ventoinha.inserirValorPwm(0);
    ligarGLOBAL = false;
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