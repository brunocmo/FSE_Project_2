# FSE_Project_2

Para acessar o projeto, clique [aqui](https://gitlab.com/fse_fga/trabalhos-2022_1/trabalho-2-2022-1).

***

## Aluno
|Matrícula | Aluno |
| -- | -- |
| 18/0117548  |  [Bruno Carmo Nunes](https://github.com/brunocmo) |

## Requisitos

É necessário o *git*, *make*, e *gpp* instalados.

Tem o uso das bibliotecas instalados na Raspberry Pi: *wiringPi.h*, *wiringPiI2C.h* e *softPwm.h*.

***
## Instalação 

Para compilar o programa:

`make`

Para executar o programa`:

`make run`

Para limpar os arquivos .o e prog(executável):

`make clean`

***
## Uso

Ao iniciar o programa, todo o processo é tratado via dashboard disponibilizada pelo docente. Temos então três tipos de saída da aplicação.

* Via dashboard (interação com o usuário)

![Dashboard](/doc/fse_figura_1.png)

* Via Youtube

![Youtube](/doc/fse_figura_3.png)

* Via console

![Console](/doc/fse_figura_2.png)

Ao ligar a Airfryer pelo dashboard, você pode escolher pelo menu o modo de usabilidade. Sendo ele:

* Modo: Manual


![MANUAL](/doc/fse_modo1.png)


* Modo: Pipoca

![PIPOCA](/doc/fse_modo2.png)

* Modo: Descongelar

![DESCONGELAMENTO](/doc/fse_modo3.png)

Depois de selecionar os modos, é possível iniciar a operação.

No modo pipoca e descongelar, a temperatura de referência e timer, são aplicados automáticamente, sendo possível somente modificar o tempo durante o funciomento do modo.

Já no modo manual, o usuário primeiro escolhe o tempo, depois escolhe a temperatura desejada. Por fim, é só necesssário iniciar o sistema.

Quando o sistema termina de operar em um certo modo, ele volta na seleção de modos possíveis novamente. Reiniciando as operações anteriores.

Toda vez que o funcionamento do sistema é ligado, é gerado um log dos dados que fica armazeda em doc/log.csv, toda vez que o programa é rodado novamente, é sobrescrevido no arquivo.

Para sair do sistema, é possível por dois modos. Apertando o botão desligar da dashboard. Ou apertar CTRL-C dentro do console onde está rodando o programa.

***
## Experimentos

1. Airfryer ligada por 14 minutos no modo manual em 65ºC de referência.

    * Figura mostrando as temperaturas e o tempo:

        ![Temperatura1](/doc/Experimento_1_Temperaturas.svg)

    * Figura mostrando o sinal e o tempo:

        ![Controle1](/doc/Experimento_1_Controle.svg)



2.  Airfryer ligada modo Pipoca.

    * Figura mostrando as temperaturas e o tempo:

        ![Temperatura2](/doc/Experimento_2_Temperaturas.svg)

    * Figura mostrando o sinal e o tempo:

        ![Controle2](/doc/Experimento_2_Controle.svg)

3.  Airfryer ligada por 16 minutos no modo manual em 80ºC de referência.

    * Figura mostrando as temperaturas e o tempo:

        ![Temperatura3](/doc/Experimento_3_Temperaturas.svg)

    * Figura mostrando o sinal e o tempo:

        ![Controle3](/doc/Experimento_3_Controle.svg)

4. Airfryer ligada por 20 minutos no modo manual com mudança de temperatura de referência.

    * Figura mostrando as temperaturas e o tempo:

        ![Temperatura4](/doc/Experimento_4_Temperaturas.svg)

    * Figura mostrando o sinal e o tempo:

        ![Controle4](/doc/Experimento_4_Controle.svg)

Todos os dados referentes a esses gráficos estão guardados na pasta doc/ respectivamente ao numeração dos graficos. (log1, log2, log3, log4).

***
