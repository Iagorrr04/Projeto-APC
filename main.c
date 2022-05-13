#include <stdio.h>
#include <stdlib.h>
#if defined(_WIN32) || defined(_WIN64)
    #include <conio.h>
#endif
#include <time.h>
#include <locale.h>
#include <string.h>

/*==================================================================DEFINI��ES================================================================*/

#define MAXnumVoo 18               // M�ximo de caracteres que um numVoo pode ter
#define MAXlinha  804              // M�ximo de caracteres que uma linha nos registros pode ter
#define MAXreservas 850            // M�ximo de passageiros que um voo pode ter.
#define MAXvoos 204                // M�ximo de voos que podem ser cadastrados
#define MAXnumReserva 20           // Tamaho m�ximo que a string que contem o n�mero de reserva pode ter.
#define MAXCPF 24                  // Tamanho m�ximo que a string que contem um cpf pode ter.
#define MAXnome 747                // Tamanho m�ximo que a string que contem um nome completo pode ter.
#define MAXDataChar 24            // Tamanho m�ximo que uma string contendo uma data pode ter
#define MAXHoraChar 16            // Tamanho m�ximo para uma string contendo a hora pode ter
#define MAXClientes 1200           // Quantidae m�xima de clientes simult�neos que podemos ter.
#define MAXCharTemp 10              // Tamanho maximo para uma string contendo uma informa��o tempor�ria, geralmente a ser convertida

#define LeituraReserva "%i,%[^,],%[^,],%[^,],%[^,],%c,%[^,],%i,%i,%i,%i\n" // Usado para ler uma reserva por inteira do .csv
#define LeituraVoo "%[^,],%i,%i,%i,%[^,],%i,%i,%i,%[^,],%i,%i,%i,%i\n"

#define RegistroVoo "%s,%i,%i,%i,%s,%i,%i,%i,%s,%i,%i,%i,%i\n" // Usado para registrar um voo no .csv
#define RegistroReserva "%i,%s,%s,%s,%s,%c,%s,%i,%i,%i,%i\n" // Usado para escrever os dados de uma reserva em um cliente
#define RegistroCliente "%s,%s,%c,%i,%i,%i,%s,%i,%i\n"  // Usado para registrar um cliente no .csv

#define CADASTROS_VOOS "cadastrosVoos.csv"
#define CADASTROS_RESERVAS "cadastrosReservas.csv"
#define CADASTROS_CLIENTES "cadastrosClientes.csv"

#define CAB_RESERVAS "IndiceVoo,numVoo,numVooRes,CPF,nomeCompleto,sexo,dataChar,diaNascimento,mesNascimento,anoNascimento,status\n"
#define CAB_CLIENTES "nome,cpf,sexo,dia,mes,ano,dataChar,qtdReservas,qtdReservasAtivas\n"
#define CAB_VOO "numVoo,maxPass,atualPass,totalReservasVoo,dataChar,dia,mes,ano,horaChar,horas,min,seg,status\n"


/*===================================================================SRTUCTS================================================================*/

//Definindo struct para registrar datas.
struct data
{
    int dia;
    int mes;
    int ano;
    char dataChar[MAXDataChar];
};
typedef struct data Data;

//Definindo struct para registrar horas.
struct hora
{
    int horas;
    int min;
    int segundos;
    char horaChar[MAXHoraChar];
};
typedef struct hora Hora;


//Defindo struct das reservas.
struct reserva
{
    int indiceVoo;
    char numVooRes[MAXnumVoo];

    char numRes[MAXnumReserva];

    char CPF[MAXCPF];
    char nomeCompleto[MAXnome];
    char sexo;                          // M = masculino, F = feminio, N = prefiro n�o me identificar
    Data dataNascimento;
    
    int statusRes; //1 = ativo, 0 para cancelado, 2 = voo que j� aconteceu
};
typedef struct reserva Reserva;

//Definindo struct do voo.
struct voo
{
    char numVoo[MAXnumVoo];

    Data DataPartida;
    Hora HoraPartida;
    
    int maxPass;
    int atualPass;                     // Definido pela quantidade de reservas ativas para esse mesmo voo.
    int totalReservasVoo;                  // Quantas reservas est�o cadastradas, independente de ativas ou n�o.
    Reserva reservaVoos[MAXreservas];
    
    int status; //0 = cancelado, 1 = ativo, 2 = j� aconteceu, 3 = Vagas esgotadas
    
};
typedef struct voo Voo;

struct cliente
{
    char ClienteNome[MAXnome];
    char ClienteSexo;
    char ClienteCPF[MAXCPF];
    Data ClienteNascimento;
    int qtdReservas;
    int qtdReservasAtivas;
}; 
typedef struct cliente Cliente;
/*=================================================================PROTOTIPOS================================================================*/
void mostrarHub();

int menuPrincipalWin();
int menuPrincipal();

void limparTela();
void espacamentoVertical(int altura);

void limparVetorChar(char *vetor, int tamanho);

void atualizarCadastrosVoos();
void atualizarCadastrosReservas();

int calculaDias(Data inical, Data final);

int contarTotalVoos();
void contarTotalReservas();


void iniciarProg();
    void atraso(int tempo);
    void barraProgresso();
    void aviao();
    void receberDataEHoraAtual();
    void receberCadastrosVoos();
    void receberCadastrosReservas();
    void gerarDadosClientes();
        void contarTotalClientes();
        int buscarCPFClientes(char CPFAbus[MAXCPF]);
        void registrarDadosClientes();
    void checarVoos();

void cadastrarVoo();
    void cadastrarNumVoo(char *numVooACad);
        int validarNumVoo(char numVooAVal[MAXnumVoo]);
    int cadastrarMaxPass();
    Data cadastrarDataVoo();
        int validarData(Data *DataAVal);
            int validarAnoBisexto(int anoAVal);
    Hora cadastrarHoraVoo();
        int validarHoraVoo(Hora *HoraAVal);
    int confirmarCadastroVoo(Voo VooACon);
    int cadastrarStatusVoo();
    void registrarVoo();


void consultarVoo();
    int receberNumVooACon();
    int selecionarVooAConWin();
    int acharNumVoo(char[MAXnumVoo]);
    void exibirDadosVoo(int vooX);
    int exibirReservasAtivas(int iVoo);

void cadastrarReserva();
    int cadastrarNumVooRes(char *numVooRes);
    int selecionarNumVooRes();
    void cadastrarNumRes(char *numResACad);
        int validarNumRes(char numResAVal[MAXnumReserva]);
            int buscarNumRes(char numResABus[MAXnumReserva]);
    int cadastrarCPF(char* CPFACadk, int iVoo);
        int validarCPF(char* CPFAVal);
        int buscarCPFEmVoo(char CPFABus[MAXCPF], int iVoo);
    void cadastrarNome(char *nomeACad);
        int validarNome(char *nomeAVal);
    char cadastrarSexo();
    Data cadastrarDataNascimento();
    int cadastarStatusReserva();
    void registrarReserva(Reserva reservaAReg);
    
void consultarReserva();
    int selecionarReservaACon();
    int receberNumReservaACon();
    void exibirDadosReserva(int indiceRes);

void consultarCliente();
    void exibirDadosCliente();
    void exibirReservasCliente(char CPFCliente[MAXCPF]);

void cancelarVoo();
    int selecionarVooACanWin();
    void receberNumVooACancelar(char *numVooACan);
    int confirmarCancelamentoVoo(Voo VooACan);

void cancelarReserva();
    void receberNumResACan(char *numResACan);
    int confirmarCancelamentoReserva(Reserva ResACan);

void excluirVoo();
    int selecionarVooAExcWin();
    void receberNumVooAExc(char *numVooAExc);
    int confirmarExclusaoVoo(Voo VooAEx);
    void excluirDadosReservas(char numVooAEx[MAXnumVoo]);
    void excluirDadosVoo(int indiceVooAExc);

void sairDoPrograma();
/*=================================================================GLOBAIS===================================================================*/

Voo todosVoos[MAXvoos];
int totalVoos;
int totalVoosAtivos;

Reserva todasReservas[MAXreservas*MAXvoos];
int totalReservas;
int totalReservasAtivas;

Cliente todosClientes[MAXClientes];
int totalClientes;

Data DataAtual;
Hora HoraAtual;

/*==================================================================MAIN====================================================================*/

int main()
{   
    
    //Permitindo caracteres do PT-BR
    setlocale(LC_ALL, "Portuguese");

    //Iniciando programa
    iniciarProg();

    int opcao;
    do
    {   
        #if defined(_WIN32) || defined(_WIND64)
            opcao = menuPrincipalWin();
        #endif
        
        #if defined(linux) || defined(uinix) || defined(APLE)
            opcao = menuPrincipal();
        #endif

        switch (opcao)
        {
        case (0):
            cadastrarVoo();
            break;

        case (1):
            cadastrarReserva();
            break;

        case (2):
            consultarVoo();
            break;
        case(3):
            consultarReserva();
            break;
        case(4):
            consultarCliente();
            break;
        case(5):
            cancelarVoo();
            break;
        case(6):
            cancelarReserva();
            break;
        case(7):
            excluirVoo();
            break;
        case(8):
            sairDoPrograma();
            break;
        default:
            break;
        }
    }while(opcao != 8);
    
    return 0;
}

/*=================================================================FUN��ES==================================================================*/
// Fun��o para espacar linhas verticalmente.
void espacamentoVertical(int altura)
{
    int s;
    for (s =0; s < altura; s++)
    {
        printf ("\n");
    }
}
// Fun��o para checar a integridade do voos, por enquanto s� checa se  o voo j� passou.
void checarVoos()
{
    // Verificar se a data de um voo ativo j� n�o passou se sim cancelar.
    int v;
    int r;
    for (v = 0; v < totalVoos; v++)
    {
        if (todosVoos[v].status != 2)
        {
            if ((validarData(&todosVoos[v].DataPartida) == -3) || ((validarData(&todosVoos[v].DataPartida) == 2)  && (validarHoraVoo(&todosVoos[v].HoraPartida) ==2)))
            {   
                // Alterar dados do voo
                int indiceVooACan = acharNumVoo(todosVoos[v].numVoo);
                todosVoos[indiceVooACan].status = 2;
                
                // Cancelar todas as reservas com aquele �ndice.
                for (r = 0; r < totalReservas; r++)
                {
                    if (todasReservas[r].indiceVoo == indiceVooACan)
                    {
                        todasReservas[r].statusRes = 2;
                    }
                }

                // Atualizar todos os cadastros presentes no .csv.
                atualizarCadastrosVoos();
                atualizarCadastrosReservas();
                gerarDadosClientes();

                // S� exibir uma notifica��ozinha.
                mostrarHub();
                printf ("\n                                               ! O voo %s ocorreu em %s e foi cancelado automaticamente !", todosVoos[v].numVoo, todosVoos[v].DataPartida.dataChar);
                printf ("\n                                                                  Digite qualquer tecla pra continuar");
                fflush(stdin);
                getc(stdin);
            }
            
        }
    }
}
// Fun��o para limpar tela independente do sistema.
void limparTela()
{
    #if defined(linux) || defined(uinix) || defined(APLE)
        system ("clear");
    #endif

    #if defined(_WIN32) || defined(_WIND64)
        system("cls || clr");
    #endif
}

// Fun��o para atualizar os cadastros dos Voos, passar os novos dados alterados nos vetores para o .csv
void atualizarCadastrosReservas()
{

    // Abrir o arquivo e reescrever tudo oque est� nele.
    FILE *cadastrosReservas = fopen(CADASTROS_RESERVAS, "w");

    // Cabe�alho.
    fprintf (cadastrosReservas, CAB_RESERVAS);

    // Escrever todos os dados salvos no vetor.
    int r;
    for (r = 0; r < totalReservas; r++)
    {
        todasReservas[r].indiceVoo = acharNumVoo(todasReservas[r].numVooRes);
        fprintf (cadastrosReservas, RegistroReserva,
                                                    todasReservas[r].indiceVoo,
                                                    todasReservas[r].numVooRes,
                                                    todasReservas[r].numRes,
                                                    todasReservas[r].CPF,
                                                    todasReservas[r].nomeCompleto,
                                                    todasReservas[r].sexo,
                                                    todasReservas[r].dataNascimento.dataChar,
                                                    todasReservas[r].dataNascimento.dia,
                                                    todasReservas[r].dataNascimento.mes,
                                                    todasReservas[r].dataNascimento.ano,
                                                    todasReservas[r].statusRes);

        // Notar que quando um voo � exclu�do seu �ndice tamb�m � alterado, portanto o �ndice do voo � "din�mico"

    }

    // Sempre fecha o arquivoooooo.
    fclose(cadastrosReservas);
}

// Fun��o para registrar os clientes dos vetores no arquivo
void registrarDadosClientes()
{
    FILE *cadastrosClientes = fopen(CADASTROS_CLIENTES, "w");

    //Cabe�alho
    fprintf(cadastrosClientes, CAB_CLIENTES);

    // Printar cliente por cliente.
    int c;
    for (c = 0; c < totalClientes; c++)
    {
        fprintf(cadastrosClientes, RegistroCliente, 
                                                    todosClientes[c].ClienteNome,
                                                    todosClientes[c].ClienteCPF,
                                                    todosClientes[c].ClienteSexo,
                                                    todosClientes[c].ClienteNascimento.dia,
                                                    todosClientes[c].ClienteNascimento.mes,
                                                    todosClientes[c].ClienteNascimento.ano,
                                                    todosClientes[c].ClienteNascimento.dataChar,
                                                    todosClientes[c].qtdReservas,
                                                    todosClientes[c].qtdReservasAtivas);
    }
    
    // Sempre fechar o arquivo.
    fclose(cadastrosClientes);
}

// Fun��o para verificar se o um dado CPF j� n�o foi cadastrado, retorna a posi��o caso ache e -1 caso nao.
int buscarCPFClientes(char CPFABus[MAXCPF])
{   
    // Varrer o vetor com todos os clientes.
    int c;
    for (c = 0; c < totalClientes; c++)
    {
        if (strcmp(todosClientes[c].ClienteCPF,CPFABus) == 0)
        {
            return c;
        }
    }

    // Se n�o achar para nem um dos clientes registrados.
    return -1;
}
// Fun��o para alterar a vari�vel que armazena a quantidade total de clientes.
void contarTotalClientes()
{
    totalClientes = 0;
    FILE *cadastrosClientes = fopen(CADASTROS_CLIENTES, "w");
    char linha[MAXlinha];
    while (fgets(linha, MAXlinha, cadastrosClientes))
    {
        totalClientes++;
    }
    totalClientes--;
}

/*
    Fun��o para gerar os dados dos clientes, baseado no cadastro das reservas.
        Conta a quantidade atual de clientes
        Altera os valores nos vetores
        Adiciona os dados no cadastro clientes
    
*/
void gerarDadosClientes()
{   

    FILE *cadastrosReservas = fopen(CADASTROS_RESERVAS, "r");
    Reserva ReservaTemp;
    char lineTemp[MAXlinha];
    fgets(lineTemp, MAXlinha, cadastrosReservas);

    totalClientes = 0;
    int ClientePos;
    int r;
    for (r = 0; r < totalReservas; r++)
    {
        fscanf(cadastrosReservas, LeituraReserva,
                                                            &ReservaTemp.indiceVoo,
                                                            ReservaTemp.numVooRes,
                                                            ReservaTemp.numRes,
                                                            ReservaTemp.CPF,
                                                            ReservaTemp.nomeCompleto,
                                                            &ReservaTemp.sexo,
                                                            ReservaTemp.dataNascimento.dataChar,
                                                            &ReservaTemp.dataNascimento.dia,
                                                            &ReservaTemp.dataNascimento.mes,
                                                            &ReservaTemp.dataNascimento.ano,
                                                            &ReservaTemp.statusRes);
                                                            
        // Caso esse CPF ainda n�o tenha sido cadastrado.
        ClientePos = buscarCPFClientes(ReservaTemp.CPF);
        if (ClientePos == -1)
        {   
            // Nome.
            strcpy(todosClientes[totalClientes].ClienteNome, ReservaTemp.nomeCompleto);

            // Cpf
            strcpy(todosClientes[totalClientes].ClienteCPF, ReservaTemp.CPF);

            // Sexo.
            todosClientes[totalClientes].ClienteSexo = ReservaTemp.sexo;

            // data int.
            todosClientes[totalClientes].ClienteNascimento.dia = ReservaTemp.dataNascimento.dia;
            todosClientes[totalClientes].ClienteNascimento.mes = ReservaTemp.dataNascimento.mes;
            todosClientes[totalClientes].ClienteNascimento.ano = ReservaTemp.dataNascimento.ano;

            // data char.
            strcpy(todosClientes[totalClientes].ClienteNascimento.dataChar, ReservaTemp.dataNascimento.dataChar);

            // se foi achado agora ent�o a quantidade de reservas � igual a 1
            todosClientes[totalClientes].qtdReservas = 1;
            if (ReservaTemp.statusRes == 1)
            {
                todosClientes[totalClientes].qtdReservasAtivas = 1;
            }
            else
            {
                todosClientes[totalClientes].qtdReservasAtivas = 0;
            }
            totalClientes++;
        }
        else // se foir um cliente que ja existe, incrementar a quantidade de reservas e quantidade de reservas ativas, de acordo
        {
            todosClientes[ClientePos].qtdReservas++;
            if (ReservaTemp.statusRes == 1)
            {
                todosClientes[ClientePos].qtdReservasAtivas++;
            }
        }
    }

    // Sempre fechar o arquivo
    fclose(cadastrosReservas);

    // Depois de ter gerado os dados dos clientes e armazenado nos vetores registr�los no arquivo
    registrarDadosClientes();
}

// Fun��o para selecionar e retornar um n�mero de voo existente, S� FUNCIONA EM WINDOWS.
int selecionarVooAConWin()
{   
    char indicadoresE[MAXvoos];
    char indicadoresD[MAXvoos];
    limparVetorChar(indicadoresE, MAXvoos);
    limparVetorChar(indicadoresD, MAXvoos);

    int i;
    int pos = 0;
    char posChar;
    while (1)
    {
        mostrarHub();
        printf ("\n                                                                       >>>> Consultando Voo <<<<\n");
        printf ("\n\n                                                                          >> Selecione o Voo <<\n");
        indicadoresE[pos] = '>';
        indicadoresD[pos] = '<';

        for (i = 0; i < totalVoos; i++)
        {
            printf ("\n                                                                             %c%c %s %c%c\n", indicadoresE[i], indicadoresE[i], todosVoos[i].numVoo, indicadoresD[i], indicadoresD[i]);
        }
        fflush(stdin);
        posChar = getch();

        // Alterar a posicao do indicador de acordo com o input.
        limparVetorChar(indicadoresE, MAXvoos);    
        limparVetorChar(indicadoresD, MAXvoos);    

        if (posChar == 13)
        {
            break;
        }
        else if ((posChar == 's') || (posChar == 'S'))
        {
            if (pos >= totalVoos-1)
            {
                pos = totalVoos -1;
            }
            else
            {
                pos++;
            }
        }
        else if ((posChar == 'w') || (posChar == 'W'))
        {
            if (pos <= 0)
            {
                pos = 0;
            }
            else
            {
                pos--;
            }
        }
    }

    // Retornar a posicao final.
    return pos;
}

// Fun��o para contar a quantidade total de reservas e alterar a vari�vel global.
void contarTotalReservas()
{   
    // Considerar o total como a quantidade de linhas -1.
    FILE *cadastrosReservas = fopen(CADASTROS_RESERVAS, "r");
    char line[MAXlinha];
    totalReservas = 0;
    while (fgets(line, MAXlinha, cadastrosReservas))
    {
        totalReservas++;
    }
    totalReservas--;
}

// Fun��o para contar a quantidade total de voos e alterar a vari�vel global, e tamb�m retornar.
int contarTotalVoos()
{
    FILE *cadastroVoos = fopen(CADASTROS_VOOS, "r");
    char line[MAXlinha];

    // Considerar igual a quantidade de linhas menos o cabe�alho.
    int totalRetornar = 0;
    totalVoos = 0;
    while (fgets(line, MAXlinha, cadastroVoos))
    {   
        totalRetornar++;
        totalVoos++;
    }
    totalRetornar--;
    totalVoos--;

    return totalRetornar;
}
/*  
    Fun��o para atualizar o arquivo com os cadastros dos voos,
    apaga todas as linhas e escreve o valor dos vetores novamente...
*/
void atualizarCadastrosVoos()
{
    // Abrir o arquivo de maneira que "sobrescreva" o que j� est� nele.
    FILE *cadastrosVoos = fopen(CADASTROS_VOOS, "w");

    //Primeira linha padr�o.
    fprintf(cadastrosVoos, CAB_VOO);

    // Os dados dos voos "atualizados", presentes nos vetores.
    int v;
    
    for (v = 0; v < totalVoos; v++)
    {
        fprintf(cadastrosVoos, RegistroVoo, 
                                                            todosVoos[v].numVoo, 
                                                            todosVoos[v].maxPass,
                                                            todosVoos[v].atualPass,
                                                            todosVoos[v].totalReservasVoo,
                                                            todosVoos[v].DataPartida.dataChar,
                                                            todosVoos[v].DataPartida.dia,
                                                            todosVoos[v].DataPartida.mes,
                                                            todosVoos[v].DataPartida.ano, 
                                                            todosVoos[v].HoraPartida.horaChar,
                                                            todosVoos[v].HoraPartida.horas, 
                                                            todosVoos[v].HoraPartida.min, 
                                                            todosVoos[v].HoraPartida.segundos, 
                                                            todosVoos[v].status);
    }

    // Sempre lembrar de fechar o arquivo.
    fclose(cadastrosVoos);
}

// Fun��o para reveber as informa��es das reservas nos arquivos.
void receberCadastrosReservas()
{   
    // Primeiro descobrindo quantas reservas existem.
    contarTotalReservas();
    totalReservasAtivas = 0;

    //Armazenar cada reserva em seu respecivo Voo.
    FILE *cadastrosReservas = fopen(CADASTROS_RESERVAS, "r");
    char lineTemp[MAXlinha];
    fgets(lineTemp, MAXlinha, cadastrosReservas);
    int r;
    for (r = 0; r < totalReservas; r++)
    {
        fscanf(cadastrosReservas, LeituraReserva,
                                                            &todasReservas[r].indiceVoo,
                                                            todasReservas[r].numVooRes,
                                                            todasReservas[r].numRes,
                                                            todasReservas[r].CPF,
                                                            todasReservas[r].nomeCompleto,
                                                            &todasReservas[r].sexo,
                                                            todasReservas[r].dataNascimento.dataChar,
                                                            &todasReservas[r].dataNascimento.dia,
                                                            &todasReservas[r].dataNascimento.mes,
                                                            &todasReservas[r].dataNascimento.ano,
                                                            &todasReservas[r].statusRes);

        if (todasReservas[r].statusRes == 1){totalReservasAtivas++;}
    }

    
    // SEMPRE FECHA O ARQUIVO
    fclose(cadastrosReservas);

}

//Fun��o para receber inform��o do arquivo com os cadastros dos voos e atribuir aos vetores.
void receberCadastrosVoos()
{       
    //Receber o total de voos cadastrados baseado na quantidade de linhas -1.
    totalVoosAtivos = 0;
    contarTotalVoos();

    //Colocando o localizador na linha do primeiro voo.
    FILE *cadastrosVoos = fopen(CADASTROS_VOOS, "r");
    char line[MAXlinha];
    int v;
    fgets(line, MAXlinha-1, cadastrosVoos);
    for (v = 0; v < totalVoos; v++)
    {
        fscanf(cadastrosVoos, LeituraVoo, 
                                                            todosVoos[v].numVoo, 
                                                            &todosVoos[v].maxPass,
                                                            &todosVoos[v].atualPass,
                                                            &todosVoos[v].totalReservasVoo,
                                                            todosVoos[v].DataPartida.dataChar,
                                                            &todosVoos[v].DataPartida.dia,
                                                            &todosVoos[v].DataPartida.mes,
                                                            &todosVoos[v].DataPartida.ano, 
                                                            todosVoos[v].HoraPartida.horaChar,
                                                            &todosVoos[v].HoraPartida.horas, 
                                                            &todosVoos[v].HoraPartida.min, 
                                                            &todosVoos[v].HoraPartida.segundos, 
                                                            &todosVoos[v].status);
        
        if (todosVoos[v].status == 1){totalVoosAtivos++;}
    }


    //Ap�s atribuir tudo aos vetores fechar o program!!!
    fclose(cadastrosVoos);
}

//Fun��o para receber a data ,e hora atual da maquina e armazenar nas vari�veis globais.
void receberDataEHoraAtual()
{   
    //Vari�vel para receber o total de segundos desde de 1 de janeiro de 1970
    time_t seconds;
    seconds = time(NULL);

    //Vari�vel que vai receber todas as informa��es da data da m�quina.
    struct tm myData;
    myData = *localtime(&seconds); //Fun��o que vai converter os segundos totais retornar na formata��o usual.

    //Atribuindo os valores obtidos a vari�vel global Data DataAtual.
    DataAtual.ano = myData.tm_year + 1909;
    DataAtual.dia = myData.tm_mday;
    DataAtual.mes = myData.tm_mon+1;
    //String.
    DataAtual.dataChar[0] = DataAtual.dia/10 + '0';
    DataAtual.dataChar[1] = DataAtual.dia%10 + '0';
    DataAtual.dataChar[2] = '/';
    DataAtual.dataChar[3] = DataAtual.mes/10 + '0';
    DataAtual.dataChar[4] = DataAtual.mes%10 + '0';
    DataAtual.dataChar[5] = '/';
    DataAtual.dataChar[6] = DataAtual.ano/1000 + '0';
    DataAtual.dataChar[7] = (DataAtual.ano/100) % 10 + '0';
    DataAtual.dataChar[8] = (DataAtual.ano/10)%100 + '0';
    DataAtual.dataChar[9] = (((float)DataAtual.ano/(float)10)-(DataAtual.ano/10))*10 + '0';// Ex: 2031, (203,1 -203)*10 = 1
    DataAtual.dataChar[10] = '\0';



    //Atribuindo os valores obtidos a vari�vel global HoraAtual.
    HoraAtual.horas = myData.tm_hour;
    HoraAtual.min = myData.tm_min;
    HoraAtual.segundos = myData.tm_sec;
    // String.
    HoraAtual.horaChar[0] = HoraAtual.horas/10 + '0';
    HoraAtual.horaChar[1] = HoraAtual.horas%10 + '0';
    HoraAtual.horaChar[2] = ':';
    HoraAtual.horaChar[3] = HoraAtual.min/10 + '0';
    HoraAtual.horaChar[4] = HoraAtual.min%10 + '0';
    HoraAtual.horaChar[5] = ':';
    HoraAtual.horaChar[6] = HoraAtual.segundos/10 + '0';
    HoraAtual.horaChar[7] = HoraAtual.segundos%10 + '0';
    HoraAtual.horaChar[8] = '\0';

}

//Fun��o para parar a execu��o do programa pela quantidade de segundos dada.
void atraso(int tempo)
{
    int mili_segundos = 25 * tempo;
    clock_t tempo_inicio = clock();
    while (clock() < tempo_inicio + mili_segundos);
}

//Fun��o para exibir a barra de progresso.
void barraProgresso()
{
    //Solicitando ao usu�rio para colocar o terminal em tela cheia, equanto exibe tela de carregamento.
    int progresso = 0;
    int iTemp;
    char pisca;
    int piscaInt = 0;
    int b;
    for (progresso = 1; progresso < 101; progresso++)
    {   
        limparTela();
        for (b = 0; b < 20; b++)
        {
            printf("\n");
        }
        if (piscaInt < 4)
        {
            pisca = ' ';
            piscaInt++;
        }
        else
        {
            pisca = '!';
            piscaInt = 0;
        }

        printf ("                                                               %c  Por favor, coloque o terminal em tela cheia. %c", pisca, pisca);
        printf ("\n                               [");
        for (iTemp = 0; iTemp< progresso; iTemp++)
        {   
            printf ("%c", 254);
        }
        for (iTemp = 0; iTemp < 100 - progresso; iTemp++)
        {
            printf("-");
        }
        printf("] %i%%\n", progresso);
        printf ("                                                                             Carregando programa...");
        atraso(rand()%2);
    }
}
//Fun��o para passar a anima��o do avi�o.
void aviao()
{   
    char espacamento[169];
    int e;
    espacamento[0] = ' ';
    for (e = 1; e < 168; e++)
    {
        espacamento[e] = ' ';
        espacamento[e+1] = '\0';

        limparTela();
        printf("\n");
        printf ("%s                     \\\\ \\                                                \n",espacamento);
        printf ("%s                       \\\\ `\\                                             \n",espacamento);
        printf ("%s    ___                 \\\\  \\                                            \n",espacamento);
        printf ("%s   |    \\                \\\\  `\\                                         \n",espacamento);
        printf ("%s   |_____\\                \\    \\                                         \n",espacamento);
        printf ("%s   |______\\                \\    `\\                                       \n",espacamento);
        printf ("%s   |       \\                \\     \\                                      \n",espacamento);
        printf ("%s   |      __\\__---------------------------------._.                        \n",espacamento);
        printf ("%s __|---~~~__o_o_o_o_o_o_o_o_o_o_o_o_o_o_o_o_o_o_[][\\__                     \n",espacamento);
        printf ("%s|___                         /~      )                \\                    \n",espacamento);
        printf ("%s    ~~~---..._______________/      ,/_________________/                     \n",espacamento);
        printf ("%s                           /      /                                         \n",espacamento);
        printf ("%s                          /     ,/                                          \n",espacamento);
        printf ("%s                         /     /                                            \n",espacamento);
        printf ("%s                        /    ,/                                             \n",espacamento);
        printf ("%s                       /    /                                               \n",espacamento);
        printf ("%s                      //  ,/                                                \n",espacamento);
        printf ("%s                     //  /                                                  \n",espacamento);
        printf ("%s                    // ,/                                                   \n",espacamento);
        printf ("%s                   //_/                                                     \n",espacamento);
    }
}
/*
  Fun��o para iniciar o programa, solicita ao usu�rio para deixar o terminal em tela cheia, "carrega o programa"
  e passa a anima��o do avi�o passando.
*/
void iniciarProg()
{
    // Configutando terinal windows.
    #if defined(_WIN32) || defined(_WIN64)
        system("mode con: cols=168 lines=45");
        system("COLOR 0D");
    #endif

    // barraProgresso();
    // aviao();
    contarTotalVoos();
    contarTotalReservas();
    receberDataEHoraAtual();
    receberCadastrosVoos();
    receberCadastrosReservas();
    gerarDadosClientes();
    checarVoos();
}

//Fun��o para mostrar o HUD principal superior, vai ser usada durante todo o programa
void mostrarHub()
{
    receberDataEHoraAtual();
    
    limparTela();
    printf ("\n========================================================================================================================================================================");
    printf ("\n                                                                         GALATIC BIRDS RESERVAS");
    printf ("\n                                                                               %s", DataAtual.dataChar);
    printf ("\nVoos Cadastrados: %2i                                                            %s                                                       Reservas Cadastradas: %3i", totalVoos, HoraAtual.horaChar, totalReservas);
    printf ("\nVoos Ativos: %i                                                    Iago Rocha Santos Marques - 211039484                                             Reservas ativas: %3i", totalVoosAtivos, totalReservasAtivas); 
    printf ("\n========================================================================================================================================================================");
    
}
//Fun��o pra preencher um dado vetor com espa�os.
void limparVetorChar(char *vetor, int tamanho)
{
    int c;
    for (c = 0; c < tamanho; c++)
    {
        *(vetor+c) = ' ';
    }
}

// Fun��o para receber a op��o que o usu�rio n�o windows deseja.
int menuPrincipal()
{

    int opcao;
    char opcaoCharTemp[10];
    
    do
    {
        mostrarHub();
        printf ("\n");
        printf ("\n                                                               Escolha uma op��o abaixo e pressione ENTER");
        printf ("\n                                                                       Navegue com: ^W^ e vSv");
        printf ("\n");
        printf ("\n                                                                          n1. Cadastrar voo");
        printf ("\n");
        printf ("\n                                                                        n2. Cadastrar reserva");
        printf ("\n");
        printf ("\n                                                                          n3. Consultar voo");
        printf ("\n");
        printf ("\n                                                                        n4. Consultar reserva");
        printf ("\n");
        printf ("\n                                                                       n5. Consultar passageiro");
        printf ("\n");
        printf ("\n                                                                           n6. Cancelar voo");
        printf ("\n");
        printf ("\n                                                                         n7. Cancelar reserva");
        printf ("\n");
        printf ("\n                                                                           n8. Excluir voo");
        printf ("\n");
        printf ("\n                                                                         n9. Sair do programa");
        printf ("\n");
        printf ("\n                                                                        >> Insira aqui: n");
        fflush(stdin);
        fgets(opcaoCharTemp, 10, stdin);
        opcaoCharTemp[strlen(opcaoCharTemp)-1] = '\0';
        opcao = atoi(opcaoCharTemp);
        printf (" <<");


    }while ((opcao > 9) || (opcao < 1));


    return opcao;
}

//Fun��o para receber a op��o do menu principal de um usu�rio windows.
int menuPrincipalWin()
{   
    char posL[10];
    char posR[10];
    char input;
    int pos;

    limparVetorChar(posR, 10);
    limparVetorChar(posL, 10);
    posL[0] = '>';
    posR[0] = '<';
    pos = 0;
    do
    {
        mostrarHub();
        printf ("\n");
        printf ("\n                                                               Escolha uma op��o abaixo e pressione ENTER");
        printf ("\n                                                                       Navegue com: ^W^ e vSv");
        printf ("\n");
        printf ("\n                                                                          %c%c Cadastrar voo %c%c", posL[0], posL[0], posR[0], posR[0]);
        printf ("\n");
        printf ("\n                                                                        %c%c Cadastrar reserva %c%c", posL[1], posL[1], posR[1], posR[1]);
        printf ("\n");
        printf ("\n                                                                          %c%c Consultar voo %c%c", posL[2], posL[2], posR[2], posR[2]);
        printf ("\n");
        printf ("\n                                                                        %c%c Consultar reserva %c%c", posL[3],  posL[3], posR[3], posR[3]);
        printf ("\n");
        printf ("\n                                                                       %c%c Consultar passageiro %c%c", posL[4], posL[4], posR[4], posR[4]);
        printf ("\n");
        printf ("\n                                                                          %c%c Cancelar voo %c%c", posL[5], posL[5], posR[5], posR[5]);
        printf ("\n");
        printf ("\n                                                                         %c%c Cancelar reserva %c%c", posL[6], posL[6], posR[6], posR[6]);
        printf ("\n");
        printf ("\n                                                                           %c%c Excluir voo %c%c", posL[7], posL[7], posR[7], posR[7]);
        printf ("\n");
        printf ("\n                                                                        %c%c Sair do programa %c%c", posL[8], posL[8], posR[8], posR[8]);
        printf ("\n");

        limparVetorChar(posL, 10);
        limparVetorChar(posR, 10);
        
        //Recebendo input e realizando "movimento"
        input = getch();
        if (input == 13)
        {
            return pos;
        }
        else
        {
            if ((input == 'w') || (input == 'W'))
            {
                pos--;
            }
            else if ((input == 'S') || (input == 's'))
            {
                pos++;
            }
            if (pos < 0)
            {
                pos = 0;
            }
            if (pos > 8)
            {
                pos = 8;
            }
            posL[pos] = '>';
            posR[pos] = '<';
        }

    }while (input != '\n');


    return pos;
}

//Fun��o que recebe o n�mero de um voo como par�metro e verifica se o mesmo � v�lido, 1 para v�lido, 0 para inv�lido, -1 para caso seja devido a j� existir um voo cadastrado com o n�mero dado
/*
    Fun��o que dado um numVoo verifica se o mesmo v�lido e retorna 1 caso seja v�lido, e um dos seguintes valores negativos caso n�o
    -1, tamanho n�a apropriado
    -2, posi��o do '-' incorreta
    -3, as tr�s primeiras letras n�o formama palavra JEB
    -4, os 4 �ltimos d�gitos t�o est�o de acordo
    -5, o numVoo a ser  cadastrado j� existe
*/
int validarNumVoo(char numVooAVal[MAXnumVoo])
{   
    // De acordo com a formata��o da string.

    // Tamanho
    if (strlen(numVooAVal) != 8){return -1;} 

    // Posi��o da '-'
    if (numVooAVal[3] != '-'){return -2;} 

    // Tr�s primeiros formarem a sigla JEB.
    if (numVooAVal[0] != 'J'){return -3;}
    if (numVooAVal[1] != 'E'){return -3;}
    if (numVooAVal[2] != 'B'){return -3;}

    // Quatro �ltimos serem n�meros.
    if ((numVooAVal[4] < '0') || (numVooAVal[4] > '9')){return -4;}
    if ((numVooAVal[5] < '0') || (numVooAVal[5] > '9')){return -4;}
    if ((numVooAVal[6] < '0') || (numVooAVal[6] > '9')){return -4;}
    if ((numVooAVal[7] < '0') || (numVooAVal[7] > '9')){return -4;}

    // Verificando se j� n�o existe um voo com o mesmo numVoo.
    int v;
    for (v = 0; v < totalVoos; v++)
    {   
        
        if (strcmp(numVooAVal, todosVoos[v].numVoo) == 0){return -5;}
    }

    //Considerando que n�o tenha sido barrado em nem uma das condi��es acima.
    return 1;
}

//Fun��o para receber o n�mero de um Voo, retorna o mesmo caso ele seja apropiado.
void cadastrarNumVoo(char *numVooACad)
{   
    //Parte visual.
    mostrarHub();
    printf ("\n                                                                       >>>> Cadastrando Voo <<<<\n");

    int verify;
    do
    {
        printf ("\n                                                                      Insira abaixo o n�mero do voo");
        printf ("\n                                                                           no formato JEB-XXXX");
        printf ("\n                                                                            >> ");
        fflush(stdin);
        fgets(numVooACad, MAXnumVoo, stdin);
        numVooACad[strlen(numVooACad) - 1] = '\0';
        printf (" <<");

        //Parte visual.
        mostrarHub();
        printf ("\n                                                                       >>>> Cadastrando Voo <<<<\n");
        verify = validarNumVoo(numVooACad);
        if (verify == -5)
        {
            printf ("\n                                                    ! J� existe um voo cadastrado com esse n�mero, tente novamente !");
        }
        else if (verify <= 0)
        {
            printf ("\n                                                               ! N�mero de voo inv�lido tente novamente !");;
        }
    }while(verify != 1);
    
}

// Func�o para receber, validar e retornar o n�mero m�ximo de passageiros de um vetor
int cadastrarMaxPass()
{
    //Parte visual.
    mostrarHub();
    printf ("\n                                                                       >>>> Cadastrando Voo <<<<\n");

    char maxPassStr[8];
    do
    {
        printf ("\n                                                              Insira abaixo o n�mero m�ximo de passageiros:");
        printf ("\n                                                                               >> ");
        fflush(stdin);
        fgets(maxPassStr, 8, stdin);
        maxPassStr[strlen(maxPassStr)-1] = '\0';
        printf (" <<");

        //Parte visual.
        mostrarHub();
        printf ("\n                                                                       >>>> Cadastrando Voo <<<<\n");
        if (atoi(maxPassStr) <= 0)
        {
            printf ("\n                                                      ! Quantidae m�xima de passageiros inv�ida, tente novamente !");
        }
        else
        {
            break;
        }
    }while (1);

    // Sempre retornar os valores ao final.
    return atoi(maxPassStr);
}

//Fun��o para verificar se um ano � bisexto.1 para sim, 0 para n�o
int validarAnoBisexto(int anoAVal)
{
    if (anoAVal % 4 == 0)
    {
        if (anoAVal % 100 == 0)
        {
            if (anoAVal % 400 == 0)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 0;
    }
}

/*
    Fun��o para verificar se uma data � v�lida, recebe uma struct data, e altera os valores da mesma, retorna nos seguintes valores.
    2, caso seja no mesmo dia.
    1, caso seja uma data v�lida e futura.
    -1, para caso um dos valores seja negativo.
    -2, para caso seja numa data incorreta, em quest�o.
    -3, para caso seja uma data passada.               ********
    -4, para caso o mes seja maior que 12.
    -5, caso a data n�o esteja no tamanho certo.
    -6, posi��o das barras.
    -7, n�mero correspondente ao dia ser inv�lido.
    -8, n�mero correspondente ao mes ser inv�lido.
    -9, n�mero correspondente ao ano ser inv�lido.
*/
int validarData(Data *DataAVal)
{   
    /*----------------------------------PARTE CHAR--------------------------*/
    // Tamanho
    if (strlen(DataAVal->dataChar) != 10){return -5;}

    // Posi��o das'/'
    if (DataAVal->dataChar[2] != '/'){return -6;}
    if (DataAVal->dataChar[5] != '/'){return -6;}

    // Um dos d�gitos n�o serem n�meros.
    if ((DataAVal->dataChar[0] < '0') || (DataAVal->dataChar[0] > '9')){return -7;}
    if ((DataAVal->dataChar[1] < '0') || (DataAVal->dataChar[1] > '9')){return -7;}

    if ((DataAVal->dataChar[3] < '0') || (DataAVal->dataChar[3] > '9')){return -8;}
    if ((DataAVal->dataChar[4] < '0') || (DataAVal->dataChar[4] > '9')){return -8;}

    if ((DataAVal->dataChar[6] < '0') || (DataAVal->dataChar[6] > '9')){return -9;}
    if ((DataAVal->dataChar[7] < '0') || (DataAVal->dataChar[7] > '9')){return -9;}
    if ((DataAVal->dataChar[8] < '0') || (DataAVal->dataChar[8] > '9')){return -9;}
    if ((DataAVal->dataChar[9] < '0') || (DataAVal->dataChar[9] > '9')){return -9;}

    
    /*----------------------------------PARTE INTEIRA----------------------------*/
    char diaChar[3] = {DataAVal->dataChar[0], DataAVal->dataChar[1], '\0'};
    char mesChar[3] = {DataAVal->dataChar[3], DataAVal->dataChar[4], '\0'};
    char anoChar[5] = {DataAVal->dataChar[6], DataAVal->dataChar[7], DataAVal->dataChar[8], DataAVal->dataChar[9],'\0'};

    DataAVal->dia = atoi(diaChar);
    DataAVal->mes = atoi(mesChar);
    DataAVal->ano = atoi(anoChar);

    //Qualquer um dos valores serem menores ou igual a zero.
    if ((DataAVal->dia <= 0) || (DataAVal->mes <= 0) || (DataAVal->ano <= 0)){return -1;}

    if (DataAVal->mes > 12){return -4;}

    //Caso o dia seja maior do que 31.
    if (DataAVal->dia > 31)
    {
        return -2;
    }

    else//Verificando dia de acordo com o mes
    {
        //Meses com 30.
        if (((DataAVal->mes == 4) || (DataAVal->mes == 6) ||(DataAVal->mes == 7) ||(DataAVal->mes == 9) ||(DataAVal->mes == 11)) && (DataAVal->dia > 30))
        {
            return -2; 
        }
        if ((DataAVal->mes == 2) && (DataAVal->dia > 29))
        {
            return -2;
        }
        else if ((DataAVal->mes == 2) && (validarAnoBisexto(DataAVal->ano) == 0) && (DataAVal->dia > 28))
        {
            return -2;
        }
    }

    //Verificando se foi  marcado em uma data passada.
    if (DataAVal->ano < DataAtual.ano)
    {
        return -3;
    }
    else if (DataAVal->ano == DataAtual.ano)
    {
        if (DataAVal->mes < DataAtual.mes)
        {
            return -3;
        }
        else if (DataAVal->mes == DataAtual.mes)
        {
            if (DataAVal->dia < DataAtual.dia)
            {
                return -3;
            }
        }
    }

    // caso seja no mesmo dia.
    if  (   (DataAtual.ano == DataAVal->ano) &&
            (DataAtual.mes == DataAVal->mes) &&
            (DataAtual.dia == DataAVal->dia))
    {
        return 2;
    }

    //Retrona 1 caso seja uma data v�lida e que ainda n�o aconteceu.
    return 1;
}

//Fun��o para cadastrar a data de um Voo e armazenar no �ltimo voo do vetor.
Data cadastrarDataVoo()
{   
    //Parte visual.
    mostrarHub();
    printf ("\n                                                                       >>>> Cadastrando Voo <<<<\n");

    Data dataACad;
    int verify;
    do
    {   
        //Receber a data inteira como uma string s� e depois dividir em dia, mes e ano.
        printf ("\n                                                               Insira a data do voo, no formato DD/MM/AAAA");
        printf ("\n                                                                           >> ");
        fflush(stdin);
        fgets (dataACad.dataChar, MAXDataChar, stdin);
        dataACad.dataChar[strlen(dataACad.dataChar) - 1] = '\0';
        printf (" <<");

        //Parte visual.
        mostrarHub();
        printf ("\n                                                                       >>>> Cadastrando Voo <<<<\n");
        verify = validarData(&dataACad);
        if (verify < 0)
        {   
            printf ("\n                                                              ! Data inv�lida, por favor tente novamente !");
        }
        if (verify == 2)
        {
            printf("\n                                                   ! N�o permitimos cadadastrar um voo no mesmo dia, tente novamente !");
        }
    }while(verify != 1);

    // Considerando que foi feita uma data v�lida,  retorn�la
    return dataACad;
}

/*
    Fun��o para verificar se a hora deum voo � v�lida, retorna os seguintes valores.
    1, v�lida e anterior a da ta atual da m�quina.
    2, v�lida por�m posterior a data atual da m�quina.
    0, segundo a formata��o padr�o de hora.
    -1, inv�lido devido ao tamano da string fornecida.
    -2, posi��o dos ":"
    -3, caracter inv�lido nas horas;
    -4, caracter inv�lido nos minutos;
*/
int validarHoraVoo(Hora *HoraAVal)
{   
    /*-----------------------------------------------------------CHAR------------------------------------------------------------------*/
    // Tamanho.
    if (strlen(HoraAVal->horaChar) != 5){return -1;}

    // Posi��o ':
    if (HoraAVal->horaChar[2] != ':'){return -2;}

    // D�gitos inv�lidos, hora, min, 
    if ((HoraAVal->horaChar[0] < '0') || (HoraAVal->horaChar[0] > '9')){return -3;}
    if ((HoraAVal->horaChar[1] < '0') || (HoraAVal->horaChar[1] > '9')){return -3;}

    if ((HoraAVal->horaChar[3] < '0') || (HoraAVal->horaChar[3] > '9')){return -4;}
    if ((HoraAVal->horaChar[4] < '0') || (HoraAVal->horaChar[4] > '9')){return -4;}

    HoraAVal->horaChar[5] = ':';
    HoraAVal->horaChar[6] = '0';
    HoraAVal->horaChar[7] = '0';
    HoraAVal->horaChar['\0'] = '0';
    /*-----------------------------------------------------------INTEIRO----------------------------------------------------------------*/
    char horasChar[3] = {HoraAVal->horaChar[0], HoraAVal->horaChar[1], '\0'};
    char minsChar[3] = {HoraAVal->horaChar[3], HoraAVal->horaChar[4], '\0'};
    char segsChar[3] = {'0', '0', '\0'};
    HoraAVal->horas = atoi(horasChar);
    HoraAVal->min = atoi(minsChar);
    HoraAVal->segundos = atoi(segsChar);

    //Primeiro verificando se o formato � v�lido
    if ((HoraAVal->min > 59) || (HoraAVal->min < 0)){return 0;}//Min
    if ((HoraAVal->horas > 23) || (HoraAVal->horas < 0)){return 0;}//Horas
    if ((HoraAVal->segundos > 59) || (HoraAVal->segundos < 0)){return 0;}//Segundos

    // V�lida verificar se � anterior a data atual.
    if (HoraAVal->horas > HoraAtual.horas)
    {
        return 2;
    }
    else if (HoraAVal->horas == HoraAtual.horas)
    {
        if (HoraAVal->min > HoraAtual.min)
        {
            return 2;
        }
    }

    //Considerando que n�o tenha sido barrada nos testes acima � uma data v�lida.
    return 1;
}

//Fun��o para cadastrar a hora de um novo voo.
Hora cadastrarHoraVoo()
{
    
    //Parte visual.
    mostrarHub();
    printf ("\n                                                                       >>>> Cadastrando Voo <<<<\n");
    int verify;
    Hora horaACad;
    do
    {
        printf ("\n                                                                     Insira a hora de partida do voo,");
        printf ("\n                                                                             no formato HH:MM");
        printf ("\n                                                                              >> ");
        fflush(stdin); 
        fgets(horaACad.horaChar, MAXHoraChar, stdin);
        horaACad.horaChar[strlen(horaACad.horaChar)-1] = '\0';
        printf (" <<");

        //Parte visual.
        mostrarHub();
        printf ("\n                                                                       >>>> Cadastrando Voo <<<<\n");
        verify = validarHoraVoo(&horaACad);
        if (verify <= 0)
        {
            printf ("\n                                                               ! Hora de partida inv�lida, tente novamente !");
        }
            
    }while(verify <= 0);


    //Sempre retornar o valor de acordo.
    return horaACad;
}

//Fun��o para cadastrar o status do voo, ativo = 1, cancelado = 0.
int cadastrarStatusVoo()
{
    //Parte visual.
    mostrarHub();
    printf ("\n                                                                       >>>> Cadastrando Voo <<<<\n");

    char status;
    do
    {
        printf ("\n                                                                        Qual o status desse voo ?");
        printf ("\n                                                                                n1. Ativo");
        printf ("\n                                                                              n2. Cancelado");
        printf ("\n                                                                           >> Insira aqui: n");
        fflush(stdin);
        status = getc(stdin);

        //Parte visual.
        mostrarHub();
        printf ("\n                                                                       >>>> Cadastrando Voo <<<<\n");
        if ((status != '1') && (status != '2'))
        {
            printf ("\n                                                                    ! Op��o inv�lida, tente novamente !");
        }
    }while ((status != '1') && (status != '2'));

    // Considerando que tenha chegado num valor v�lido retorn�-lo
    if (status == '1'){return 1;}
    else {return 2;}
}

//Fun��o para pegar os dados do vetor do �ltimo voo e escrever no cadastro de voos.
void registrarVoo(Voo vooACad)
{   
    todosVoos[totalVoos] = vooACad;

    //Abrir o cadastroVoos no modo append, j� direciona na �ltima linha.
    FILE *cadastrosVoos = fopen(CADASTROS_VOOS, "a");

     fprintf (cadastrosVoos, RegistroVoo, 
                                                    todosVoos[totalVoos].numVoo, 
                                                    todosVoos[totalVoos].maxPass, 
                                                    todosVoos[totalVoos].atualPass,
                                                    todosVoos[totalVoos].totalReservasVoo,
                                                    todosVoos[totalVoos].DataPartida.dataChar,
                                                    todosVoos[totalVoos].DataPartida.dia, 
                                                    todosVoos[totalVoos].DataPartida.mes, 
                                                    todosVoos[totalVoos].DataPartida.ano,
                                                    todosVoos[totalVoos].HoraPartida.horaChar,
                                                    todosVoos[totalVoos].HoraPartida.horas, 
                                                    todosVoos[totalVoos].HoraPartida.min,
                                                    todosVoos[totalVoos].HoraPartida.segundos, 
                                                    todosVoos[totalVoos].status);   


    //SEMPRE FECHA O ARQUIVO !!
    fclose(cadastrosVoos);
}


//Fun��o para cadastrar um Voo, e todas as suas vari�veis.
void cadastrarVoo()
{   
    //Parte visual.
    mostrarHub();
    printf ("\n                                                                       >>>> Cadastrando Voo <<<<\n");

    Voo vooACad;
    // N�mero do voo
    cadastrarNumVoo(vooACad.numVoo);

    // M�ximo de passageiros.
    vooACad.maxPass = cadastrarMaxPass();

    vooACad.atualPass = 0; //Considerando que o voo est� sendo cadastra agora, nao tem nem uma vaga ocupada
    vooACad.totalReservasVoo = 0; //Considerando que o voo est� sendo cadastrado agora nao tem nem uma reserva feita.
    
    // Data de partida.
    vooACad.DataPartida = cadastrarDataVoo();

    // Hora de partida
    vooACad.HoraPartida =  cadastrarHoraVoo();

    // Status do voo, considerar ativo no ato do cadastro.
    // vooACad.status = cadastrarStatusVoo();
    vooACad.status = 1;

    // Confirmando ou abortando cadastro do voo.
    if (confirmarCadastroVoo(vooACad)==1)
    {
         // Adicionar voo gerado aos vetores e por fim escrevelo no .csv
        registrarVoo(vooACad);  
        totalVoos++;

        printf ("\n                                                                       VOO CADASTRADO COM SUCESSO");     
    }
    else
    {
        printf ("\n                                                                     CADASTRO DE VOO ABORTADO COM SUCESSO");
    }
    

    printf ("\n                                                           >> DIGITE QUALQUER TECLA PRA RETORNAR AO MENU PRINCIPAL <<");
    fflush(stdin);
    getch();
}

/*
    Fun��o que dada um n�mero de Voo verifica se o mesmo e v�lido,
    -1, para formata��o do numVoo inv�lida.
    -2, para caso o numVoo n�o esteja cadastrado.
    -4, caso ocorra um erro
    int V, com a posi��o do voo nos vetores caso seja v�lido
*/
int acharNumVoo(char numVooAA[MAXnumVoo])
{
    int statusNumVoo;
    statusNumVoo = validarNumVoo(numVooAA);

    //A format�o n�o � v�lida, e tamb�m n�o � um num que j� existe.
    if ((statusNumVoo != 1) && (statusNumVoo != -5))
    {
        return -1;
    }

    //A formata��o � v�lida no entanto o numVoo n�o est� registrado.
    if (statusNumVoo == 1)
    {
        return -2;
    }

    //O numVoo j� existe, portanto a formata��o � v�lida, buscar nos vetores qual � a sua posi��o e retorn�-la
    if (statusNumVoo == -5)
    {
        int v;
        for (v = 0; v < totalVoos; v++)
        {
            if (strcmp(todosVoos[v].numVoo, numVooAA) == 0)
            {
                return v;
            }
        }
    }

    //Caso nem uma das condi��es acima seja atendida retornar -4 de erro.
    return -4;
}

//Fun��o para exibir as informa��es de um dado voo, limpa a tela insire o hub e mostra as informa��es.
void exibirDadosVoo(int vooX)
{
    mostrarHub();
    printf ("\n                                                                       >>>> Consultando Voo <<<<");
    printf ("\n");
    printf ("\n                                                                >> Aqui v�o as informa��es do seu voo <<");
    printf ("\n");
    printf ("\n                                                                             N�mero do voo");
    printf ("\n                                                                               %s", todosVoos[vooX].numVoo);
    printf ("\n");
    printf ("\n                                                                          M�ximo de passageiros");
    printf ("\n                                                                                  %i", todosVoos[vooX].maxPass);
    printf ("\n");
    printf ("\n                                                                             Reservas ativas");
    printf ("\n                                                                                   %i", todosVoos[vooX].atualPass);
    printf ("\n");
    printf ("\n                                                                         Percentual de ocupa��o");
    printf ("\n                                                                                 %.2f %%", ((float) ((float)todosVoos[vooX].atualPass/(float)todosVoos[vooX].maxPass) * 100));
    printf ("\n");
    printf ("\n                                                                            Data de partida");
    printf ("\n                                                                              %s", todosVoos[vooX].DataPartida.dataChar);
    printf ("\n");
    printf ("\n                                                                            Hora de partida");
    printf ("\n                                                                               %s", todosVoos[vooX].HoraPartida.horaChar);
    printf ("\n");
    printf ("\n                                                                             Status do voo");
    if (todosVoos[vooX].status == 1)
    {
        printf ("\n                                                                                 ATIVO");
    }
    else
    {
        printf ("\n                                                                              CANCELADO");
    }
}

// Fun��o para exibir as reservas ativas de um dado voo, localizado pela posi��o no vetor, retonar a quantidade de reservas exibidas
int exibirReservasAtivas(int iVoo)
{
    printf ("\n\n                                                                          >> Reservas Ativas <<");

    // Reservas a ativas
    int r;
    int spc = 0;
    int s;
    printf ("\n                                     |  N�mero   |                   Nome                   |       CPF      | Nascimento | S |");
    for (r = 0; r < totalReservas; r++)
    {   
        if (todasReservas[r].indiceVoo == iVoo)
        {      
            spc++;  

            printf ("\n                                     | %s | %s ",
                                                        todasReservas[r].numRes,
                                                        todasReservas[r].nomeCompleto);
            for (s = 0; s < 40 -strlen(todasReservas[r].nomeCompleto); s++)
            {
                printf (" ");
            }
            printf ("| %s | %s | %c |", todasReservas[r].CPF, todasReservas[r].dataNascimento.dataChar, todasReservas[r].sexo);
        }    
    }

    return spc;
}

//Fun��o para cosultar as informa��es do voo, essa por sua vez vai chamar diversas fun��es auxliares para mante a integridade do programa.
void consultarVoo()
{
    mostrarHub();
    printf ("\n                                                                       >>>> Consultando Voo <<<<\n");
    int indiceVooACon;

    // Selecionar/digitar n�mero do voo de acordo com o OS.
    #if defined(_WIN32) || defined(_WIN64)
        indiceVooACon = selecionarVooAConWin();
    #else
        indiceVooACon = receberNumVooACon();
    #endif
    // Com  a posi��o do voo escolhido basta exibir os dados do voo.
    exibirDadosVoo(indiceVooACon);

    // Na mesma l�gica exibir as reservas ativas.
    int spc;
    if (todosVoos[indiceVooACon].atualPass != 0)
    {
        spc = exibirReservasAtivas(indiceVooACon);
    }
    else
    {
        printf ("\n                                                                 ! ESSE VOO N�O POSS�I RESERVAS ATIVAS !");
    }
    //Printar isso no final da dela caso d� espa�o
    espacamentoVertical(12-spc);
    printf ("\n                                                      >> DIGITE QUALQUER TECLA PARA RETORNAR AO MENU PRINCIPAL <<");
    fflush(stdin);
    getch();
}

/*
    Fun��o para receber, validar, e "retornar" o n�mero de um voo, e retornar de fato o indice desse voo nos vetores.
    validar inclui verificar se o mesmo ainda tem ocupa��o dispon�vel...
*/
int cadastrarNumVooRes(char *numVooRes)
{
    
    mostrarHub();
    printf ("                                                                      >>>> Cadastro de Reserva <<<<\n");

    int verify;
    do
    {
       
        printf ("\n                                                       Digite o n�mero do voo em que deseja realizar uma reserva,");
        printf ("\n                                                                          no formato JEB-XXXX");
        printf ("\n                                                                           >> ");
        fflush(stdin);
        fgets(numVooRes, MAXnumVoo, stdin);
        numVooRes[strlen(numVooRes) -1] = '\0';
        printf (" <<");

        mostrarHub();
        printf ("                                                                      >>>> Cadastro de Reserva <<<<\n");
        verify = acharNumVoo(numVooRes);
        if (verify == -1)// Formata��o inv�lida.
        {
            printf ("\n                                                          ! N�mero de inserido n�o � v�lido, tente novamente !");
        }
        else if (verify == -2)// NumVoo n�o est� cadastrado.
        {
            printf ("\n                                              ! O n�mero de voo inserido n�o est� cadastrado no sistema, tente novamente !");
        }
        else if (verify == -4)// Erro inesperado
        {
            printf ("\n                                                                        ! ERRO, tente novamente !");
        }
        else if (verify >= 0)// Se for um n�mero v�lido (verify > 0) verificar se o voo n�o est� cheio ou cancelado
        {   
            // printf ("\nMAXPASS : %i AUTUALPASS: %i", todosVoos[verify].maxPass ,todosVoos[verify].atualPass);
            if (todosVoos[verify].atualPass >= todosVoos[verify].maxPass)
            {   
                printf ("\n                                                            ! O voo escolhido j� est� cheio, tente novamente !");
                verify = -4;
            }

            if (todosVoos[verify].status == 0)
            {
                printf ("\n                                                            ! O voo escolhido n�o est� ativo, tente novamente !");
                verify = -4;
            }
        }

    }while(verify < 0);

    // Considerando que o numVoo para a reserva seja v�lido retorn�lo.
    return verify;
}

/*
    Fun��o para verificar se um n�mero de reserva j� existe independente do voo, considerando que esteja em uma format��o v�lida, retorna:
    -1, caso o n�mero de reserva n�o encontrado nos registros.
    int pos, maior ou igual a zero com a posi��o da linha em que aquele n�mero de reserva se encontra.
*/
int buscarNumRes(char numResAbus[MAXnumReserva])
{   
    // Armazenar as informa��es de cada linha na reserva temp e verificar se a o numRes daquela linha � igual ao fornecido.
    Reserva reservaTemp;
    FILE *cadastrosReservas = fopen(CADASTROS_RESERVAS, "r");
    char lineTemp[MAXlinha];

    fgets(lineTemp, MAXlinha, cadastrosReservas);
    int pos = 0;
    while (pos < totalReservas)
    {
        fscanf(cadastrosReservas, LeituraReserva,
                                                            &reservaTemp.indiceVoo,
                                                            reservaTemp.numVooRes,
                                                            reservaTemp.numRes,
                                                            reservaTemp.CPF,
                                                            reservaTemp.nomeCompleto,
                                                            &reservaTemp.sexo,
                                                            reservaTemp.dataNascimento.dataChar,
                                                            &reservaTemp.dataNascimento.dia,
                                                            &reservaTemp.dataNascimento.mes,
                                                            &reservaTemp.dataNascimento.ano,
                                                            &reservaTemp.statusRes);
        if (strcmp(reservaTemp.numRes,numResAbus) == 0)
        {
            return pos;
            fclose(cadastrosReservas);
        }
        pos++;
    }
    
    // Se chegar at� aqui � por que n�o foi encontrado nos registros.
    return -1;
    fclose(cadastrosReservas);
}
/*
    Fun��o para validar um n�mero de reserva, vai recebeloe e retornar:
    1, caso seja um n�mero de reserva v�lido;
    -1, caso n�o esteja do tamanho adequado;
    -2, caso os 2 caracteres do in�cio estejam incorreto;
    -3, caso "-" n�o esteja na posi��o correta;
    -4, caso os �ltimos 6 d�gitos n�o sejam n�meros.
    -5, caso j� exista um n�mero de reserva igual.
*/
int validarNumRes(char numResAVal[MAXnumReserva])
{
    // Tamanho.
    if (strlen(numResAVal) != 9){return -1;}

    // Caracteres iniciais.
    if (numResAVal[0] != 'G'){return -2;}
    if (numResAVal[1] != 'B'){return -2;}

    // Posi��o do "-"
    if (numResAVal[2] != '-'){return -3;}

    // Seis �ltimos n�meros.
    if ((numResAVal[3] < '0') || (numResAVal[3] > '9')){return -4;}
    if ((numResAVal[4] < '0') || (numResAVal[4] > '9')){return -4;}
    if ((numResAVal[5] < '0') || (numResAVal[5] > '9')){return -4;}
    if ((numResAVal[6] < '0') || (numResAVal[6] > '9')){return -4;}
    if ((numResAVal[7] < '0') || (numResAVal[7] > '9')){return -4;}
    if ((numResAVal[8] < '0') || (numResAVal[8] > '9')){return -4;}

    // Verificando se j� n�o existe esse mesmo n�mero de reserva
    if (buscarNumRes(numResAVal) != -1){return -5;}

    // Considerando que n�o foi barrado, � um n�mero de reserva v�lido.
    return 1;

}

// Fun��o para receber o n�mero de uma reserva, e "retorna-lo", caso seja v�lido.
void cadastrarNumRes(char *numResACad)
{   
    mostrarHub();
    printf ("                                                                      >>>> Cadastro de Reserva <<<<\n");

    int verify;
    do
    {
        printf ("\n                                                            Digite o n�mero da reserva que est� realizando,");
        printf ("\n                                                                        no formato GB-XXXXXX");
        printf ("\n                                                                         >> ");
        fflush(stdin);
        fgets(numResACad, MAXnumReserva, stdin);
        numResACad[strlen(numResACad) - 1] = '\0';
        printf (" <<");
        
        mostrarHub();
        printf ("                                                                      >>>> Cadastro de Reserva <<<<\n");
        verify = validarNumRes(numResACad);
        if (verify == -5)
        {
            printf ("\n                                                    ! J� existe um n�mero de reserva igual a esse, tente novamente !");
        }
        else if (verify < 0)
        {
            printf ("\n                                                            ! N�mero de reserva inv�lida, tente novamente !");
        }
        
    }while(verify < 0);
}

/*
    Fun��o que "recebe" um CPF e retorna valores negativos caso inv�lido e 1 caso v�lido
    -1, caso o seja tamanho inv�lido;
    -2, caso um dos tr�s primeiros d�gitos sejam inv�lidos;
    -3, caso a posi��o do primeiro . seja inv�lida;
    -4, caso um dos tr�s d�gitos do meio sejam inv�lidos;
    -5, caso a posi��o do segundo ponto seja inv�lida;
    -6, caso um dos tr�s �ltimos d�gitos seja inv�lido;
    -7, caso a posi��o do '-' seja inv�lido;
    -8, caso um dos dois �ltimos d�gitos seja inv�lido;
    -9, caso a valida��o do CPF pelo primeiro d�gito seja inv�lido;
    -10, caso a valida��o do CPF pelo segundo d�gito seja inv�lida;
    -11, caso seja um CPF com todos os n�meros iguais;
    1, caso o CPF seja v�lido.
*/
int validarCPF(char* CPFAVal)
{
    // Tamanho.
    if (strlen(CPFAVal) != 14){return -1;}

    // Tr�s primeiros d�gitos.
    if (('0' > CPFAVal[0]) || (CPFAVal[0] > '9')){return -2;}
    if (('0' > CPFAVal[1]) || (CPFAVal[1] > '9')){return -2;}
    if (('0' > CPFAVal[2]) || (CPFAVal[2] > '9')){return -2;}

    // Posi��o do  primeiro "."
    if (CPFAVal[3] != '.'){return -3;}

    // Tr�s d�gitos do meio.
    if (('0' > CPFAVal[4]) || (CPFAVal[4] > '9')){return -4;}
    if (('0' > CPFAVal[5]) || (CPFAVal[5] > '9')){return -4;}
    if (('0' > CPFAVal[6]) || (CPFAVal[6] > '9')){return -4;}

    // Posi��o do segundo "."
    if (CPFAVal[7] != '.'){return -5;}

    // Tr�s �ltimos d�gitos.
    if (('0' > CPFAVal[8]) || (CPFAVal[8] > '9')){return -6;}
    if (('0' > CPFAVal[9]) || (CPFAVal[9] > '9')){return -6;}
    if (('0' > CPFAVal[10]) || (CPFAVal[10] > '9')){return -6;}

    // Posi��o do "-"
    if (CPFAVal[11] != '-'){return -7;}

    // Dois �ltimos d�gitos.
    if (('0' > CPFAVal[12]) || (CPFAVal[12] > '9')){return -8;}
    if (('0' > CPFAVal[13]) || (CPFAVal[13] > '9')){return -8;}

    // Considerando que a formata��o seja v�lida, verificar se obedece as regras do CPF

    // Gerando string somente com os n�meros, para facilitar o processo
    char CPFNum[12];
    int i;
    int c = 0;
    for (i = 0; i < 14; i++)
    {
        if ((CPFAVal[i] != '-') && (CPFAVal[i] != '.'))
        {
            CPFNum[c] = CPFAVal[i];
            c++;
        }
    }
    CPFNum[11] = '\0';

    // Validando o primeiro d�gito.
    int somaCPF = 0;
    c = 10;
    for (i = 0; i < 9; i++)
    {   
        somaCPF = somaCPF + ((CPFNum[i]-'0') * c);
        c--;
    }
    int resto;
    resto = (somaCPF*10) % 11;
    if (resto == 10)
    {
        resto = 0;
    }
    if (resto != (CPFNum[9]-'0')){return -9;}

    // Validanddo segundo d�gito.
    c = 11;
    somaCPF = 0;
    for (i = 0; i < 10; i++)
    {   
        somaCPF = somaCPF + ((CPFNum[i]-'0') * c);
        c--;
    }
    resto = (somaCPF*10) % 11;
    if (resto == 10)
    {
        resto = 0;
    }
    if (resto != (CPFNum[10] - '0')){return -10;}

    // Verificando se n�o � um CPF com todos os n�meros iguais.
    int totalIguais;
    for (i = 0; i < 10; i++)
    {   
        totalIguais = 0;
        for (c = 0; c < 11; c++)
        {
            if ((CPFNum[c]-'0') == i)
            {
                totalIguais++;
            }
        }
        if (totalIguais == 11){return -11;}
    }


    // Considerando que n�o foi barrado em nenhuma das verifica��es acima o CPF � v�lido.
    return 1;
}

/* 
    Fun��o para verificar se j� existe alguma reserva num dado voo, com um dado CPF, retorna:
    1, se o CPF n�o foi encontrado nas reservas do Voo;
    -1, caso CPF seja encontrado em umas das reservas, e reserva est� ativa;
    -2, caso o CPF seja encontrado em umas das reservas por�m a reserva n�o esteja ativa.
*/
int buscarCPFEmVoo(char CPFAbus[MAXCPF], int iVoo)
{
    Reserva reservaTemp;
    FILE *cadastrosReservas = fopen(CADASTROS_RESERVAS, "r");
    char line[MAXlinha];
    fgets(line, MAXlinha, cadastrosReservas);
    int pos = 0;
    while (pos < totalReservas)
    {
        fscanf(cadastrosReservas, LeituraReserva,
                                                            &reservaTemp.indiceVoo,
                                                            reservaTemp.numVooRes,
                                                            reservaTemp.numRes,
                                                            reservaTemp.CPF,
                                                            reservaTemp.nomeCompleto,
                                                            &reservaTemp.sexo,
                                                            reservaTemp.dataNascimento.dataChar,
                                                            &reservaTemp.dataNascimento.dia,
                                                            &reservaTemp.dataNascimento.mes,
                                                            &reservaTemp.dataNascimento.ano,
                                                            &reservaTemp.statusRes);
        
        if (iVoo == reservaTemp.indiceVoo)
        {
            if (strcmp(reservaTemp.CPF, CPFAbus) == 0)
            {
                if (reservaTemp.statusRes == 1)
                {
                    return -1;
                }
                else
                {
                    return -2;
                }
            }
        }
        
        pos++;
    }

    // Considerando que n�o foi encontrado um CPF igual retorna verdadeiro.
    return 1;
}

/*  
    Fun��o para receber um CPF, at� que seja v�lido retorna os seguintes valores.
    -1, caso seja um CPF que j� existe, em outro voo.
    1, caso tenha ocorrido tudo certo.
*/
int cadastrarCPF(char *CPFAcad, int inVoo)
{
    mostrarHub();
    printf ("                                                                      >>>> Cadastro de Reserva <<<<\n");

    int verify;
    do
    {
        printf ("\n                                                                     Digite o seu n�mero de CPF");
        printf ("\n                                                                     no formato 709.719.610-69");
        printf ("\n                                                                       >> ");
        fflush(stdin);
        fgets(CPFAcad, MAXCPF, stdin);
        CPFAcad[strlen(CPFAcad) - 1] = '\0';
        printf (" <<");


        mostrarHub();
        printf ("                                                                      >>>> Cadastro de Reserva <<<<\n");
        verify = validarCPF(CPFAcad);
        if(verify < 0)
        {
            printf ("\n\n                                                              ! N�mero de CPF inv�lido tente novamente !");
        }
        else // Se for um CPF v�lido verificar se j� n�o foi usado em outra reserva, ATIVA.
        {
            if (buscarCPFEmVoo(CPFAcad, inVoo) == -1)
            {
                printf ("\n                                                   ! J� existe uma reserva ativa cadastrada neste voo com este CPF !");
                printf ("\n                                                                           tente novamente");
                verify = -1;
            }
            else if (buscarCPFClientes(CPFAcad) != -1)
            {
                printf ("\n                                                         ! Foi encontrado um registro com esse mesmo CPF !");
                printf ("\n                                                         iremos utilizar os dados cadastrados previamente.");
                printf ("\n"); 
                printf ("\n"); 
                printf ("\n                                                            PRESSIONE ENTER PARA PROSEEGUEIR COM O CADASTRO");
                fflush(stdin);
                getc(stdin);
                return -1;
            }
        }

    }while(verify != 1);

    // Um cpf novo e n�o registrado no voo foi inserido.
    return 1;
}   
/*
    Fun��o que dado o endere�o de um nome retorna um valor inteiro de acordo com a valida��o do nome.
    1, caso seja um nome v�lido.
    -1, cao seja um nome inv�lido
*/
int validarNome(char *nomeAVal)
{   
    // Verificando se n�o existe nem um caracter inapropriado, considerando que seja um nome "comum".
    int i;
    for (i = 0; i < strlen(nomeAVal)-1; i++)
    {   
        if (    ((nomeAVal[i] >= 0) && (nomeAVal[i] <= 31))     ||
                ((nomeAVal[i] >= 33) && (nomeAVal[i] <= 64))    ||
                ((nomeAVal[i] >= 91) && (nomeAVal[i] <= 93))    ||
                ((nomeAVal[i] >= 123) && (nomeAVal[i] <= 125))  ||
                ((nomeAVal[i] >= 127) && (nomeAVal[i] <= 179))  ||
                ((nomeAVal[i] >= 181) && (nomeAVal[i] <= 191))   )
        {   
            printf ("\nCARACTER INV�LIDO: %c", nomeAVal[i]);
            return -1;
        }
    }


    // Considerando que n�o foi barrado em nem uma das verifica��es acima, o nome � v�lido.
    return 1;
}

// Fun��o para receber e adicionar o nome a um dado endere�o do char, solicita nomes at� que seja inserido um nome v�lido.
void cadastrarNome(char *nomeACad)
{
    mostrarHub();
    printf ("                                                                      >>>> Cadastro de Reserva <<<<\n");

    int verify;
    do
    {
        printf ("\n                                                                   Digite o nome do portador do CPF");
        printf("\n                                                                 >> ");
        fflush(stdin);
        fgets(nomeACad, MAXnome, stdin);
        nomeACad[strlen(nomeACad) -1] = '\0';
        printf (" <<");
        
        mostrarHub();
        printf ("                                                                      >>>> Cadastro de Reserva <<<<\n");
        verify = validarNome(nomeACad);
        if (verify != 1)
        {
            printf ("\n                                                           ! O nome inserido n�o � v�lido, tente novamente !");
        }
    }while(verify != 1);
}


// Fun��o que retorna um caractere conrrespondente ao sexo do usu�rio, solicita um caractere v�lido at� conseguir.
char cadastrarSexo()
{
    mostrarHub();
    printf ("                                                                      >>>> Cadastro de Reserva <<<<\n");

    char sexoACad;
    do
    {
        printf ("\n                                                                           Qual o seu sexo ?");
        printf ("\n                                                                             n1. Masculino");
        printf ("\n                                                                             n2. Feminino");
        printf ("\n                                                                    n3. Prefiro n�o me indentificar");
        printf ("\n                                                                          >> Inisira aqui: n");
        fflush (stdin);
        sexoACad = getc(stdin);

        mostrarHub();
        printf ("                                                                      >>>> Cadastro de Reserva <<<<\n");
        if ((sexoACad != '1') && (sexoACad != '2') && (sexoACad != '3'))
        {
            printf ("\n                                                                  ! N�mero inv�lido, tente novamente !");
        }

    }while((sexoACad != '1') && (sexoACad != '2') && (sexoACad != '3'));

    if (sexoACad == '1')
    {
        return 'M';
    }
    else if (sexoACad == '2')
    {
        return 'F';
    }
    else
    {
        return 'N';
    }
    
}

// Fun��o para cadastrar e retornar uma data de nascimento v�lida
Data cadastrarDataNascimento()
{
    mostrarHub();
    printf ("                                                                      >>>> Cadastro de Reserva <<<<\n");

    Data dataNasACad;
    int verify;
    do
    {
        // Receber como um char e converter para inteiro cada parte
        printf ("\n                                                             Insira a data de nascimento do portador do CPF");
        printf ("\n                                                                         no formato 00/00/0000");
        printf ("\n                                                                           >> ");
        fflush(stdin);
        fgets(dataNasACad.dataChar, MAXDataChar, stdin);
        dataNasACad.dataChar[strlen(dataNasACad.dataChar) -1] = '\0';
        printf (" <<");
    

        // Verificar se corresponde a uma data passada.
        mostrarHub();
        printf ("                                                                      >>>> Cadastro de Reserva <<<<\n");
        verify = validarData(&dataNasACad);
        if (verify != -3) //-r � para datas passadas
        {   
            printf ("\n\n                                                          ! A data inserida n�o � v�lida, tente novamente !");
        }
        else // Considerando que seja uma data que j� passou verifiacar se tem mais de 84 anos.
        {
            if ((DataAtual.ano - dataNasACad.ano) > 84)
            {   
                printf ("\n\n                                                        ! N�o s�o permitidos passageiros acima de 84 anos !");
                printf ("\n                                                                            tente novamente");

                verify = -9;
            }
            else if ((DataAtual.ano - dataNasACad.ano) < 10)
            {
                printf ("\n\n                                                    ! N�o permitimos crian��s abaixo de 10 anos, tente novamente !");
                verify = -9;
            }
        }

    }while(verify != -3);

    // Retornar a data de nascimento j� validada.
    return dataNasACad;
}

// Fun��o para cadastrar o status de uma reserva, retorna o inteiro de acordo com o mesmo.
int cadastrarStatusReserva()
{
    char statusACad;
    do
    {
        printf ("\n                                                                      Qual o status dessa reserva ?");
        printf ("\n                                                                               n1. Ativo");
        printf ("\n                                                                              n2. Cancelado");
        printf ("\n                                                                           >> Inisra aqui: n");
        fflush(stdin);
        statusACad = getc(stdin);

        if ((statusACad != '1') && (statusACad != '2'))
        {
            printf ("\n\n                                                            ! O status inserido � inv�lido, tente novamente");
        }
    }while ((statusACad != '1') && (statusACad != '2') );

    // Sempre retornar o valor ao finaldas fun��es de cadastro.
    if (statusACad == '1'){return 1;}
    else {return 0;}
}

// Fun��o que dada uma reserva adiciona a mesma no arquivo de reservas na �ltima posi��o, e adiciona a mesma ao vetor do voo a qual pertence.
void registrarReserva(Reserva reservaAReg)
{
    // Primeiro adicionar essa reserva no voo a qual pertence.
    todosVoos[reservaAReg.indiceVoo].reservaVoos[todosVoos[reservaAReg.indiceVoo].totalReservasVoo] = reservaAReg;

    // Incrementar a quantidade de reservas daquele voo.
    todosVoos[reservaAReg.indiceVoo].totalReservasVoo++;

    // Caso seja uma reserva ativa aumentar a quantidade de passageiros do Voo.
    if (reservaAReg.statusRes == 1)
    {
        todosVoos[reservaAReg.indiceVoo].atualPass++;
    }

    // Agora cuidar da parte de registrar no arquivo, abrir o arquivo apontando na pr�xima linha e escrever as informa��es da reserva.
    FILE *cadastroReservas = fopen(CADASTROS_RESERVAS, "a");

    // N�O ESQUCER DO '\n' PARA APONTAR PARA A PR�XIMA LINHA !!!!
    fprintf(cadastroReservas, RegistroReserva,
                                                            reservaAReg.indiceVoo,
                                                            reservaAReg.numVooRes,
                                                            reservaAReg.numRes,
                                                            reservaAReg.CPF,
                                                            reservaAReg.nomeCompleto,
                                                            reservaAReg.sexo,
                                                            reservaAReg.dataNascimento.dataChar,
                                                            reservaAReg.dataNascimento.dia,
                                                            reservaAReg.dataNascimento.mes,
                                                            reservaAReg.dataNascimento.ano,
                                                            reservaAReg.statusRes);

    // SEMPRE FECHA O ARQUIVO !!
    fclose(cadastroReservas);

}

// Fun��o para escolher um voo do jeito  "cool"
int selecionarNumVooRes()
{
    char indicadoresE[MAXvoos];
    char indicadoresD[MAXvoos];
    limparVetorChar(indicadoresE, MAXvoos);
    limparVetorChar(indicadoresD, MAXvoos);

    int i;
    int pos = 0;
    char posChar;
    while (1)
    {
        mostrarHub();
        printf ("                                                                      >>>> Cadastro de Reserva <<<<\n");
        printf ("\n\n                                                                          >> Selecione o Voo <<\n");
        indicadoresE[pos] = '>';
        indicadoresD[pos] = '<';

        for (i = 0; i < totalVoos; i++)
        {
            printf ("\n                                                                             %c%c %s %c%c\n", indicadoresE[i], indicadoresE[i], todosVoos[i].numVoo, indicadoresD[i], indicadoresD[i]);
        }
        fflush(stdin);
        posChar = getch();

        // Alterar a posicao do indicador de acordo com o input.
        limparVetorChar(indicadoresE, MAXvoos);    
        limparVetorChar(indicadoresD, MAXvoos);    

        if (posChar == 13)
        {   
            if (todosVoos[pos].status == 0)
            {   
                mostrarHub();
                printf ("                                                                      >>>> Cadastro de Reserva <<<<\n");
                printf ("\n\n                                                                          >> Selecione o Voo <<\n");
                printf ("\n                                                           ! O Voo escolhido foi cancelado, tente novamente !");
                espacamentoVertical(25);
                printf("\n                                                                   >> PRESSIONE ENTER PARA CONTINUAR <<");
                fflush(stdin);
                getc(stdin);
            }
            else if (todosVoos[pos].atualPass >= todosVoos[pos].maxPass)
            {
                mostrarHub();
                printf ("                                                                      >>>> Cadastro de Reserva <<<<\n");
                printf ("\n\n                                                                          >> Selecione o Voo <<\n");   
                printf ("\n                                                           ! O Voo escolhido j� est� cheio, tente novamente !");
                espacamentoVertical(25);
                printf("\n                                                                   >> PRESSIONE ENTER PARA CONTINUAR <<");
                fflush(stdin);
                getc(stdin);
            }
            else if (todosVoos[pos].status == 2)
            {
                mostrarHub();
                printf ("                                                                      >>>> Cadastro de Reserva <<<<\n");
                printf ("\n\n                                                                          >> Selecione o Voo <<\n");
                printf ("\n                                                            ! Voo escolhido j� aconteceu, tente novamente !");
                espacamentoVertical(25);
                printf("\n                                                                   >> PRESSIONE ENTER PARA CONTINUAR <<");
                fflush(stdin);
                getc(stdin);
            }
            else
            {
                break;
            }
        }
        else if ((posChar == 's') || (posChar == 'S'))
        {
            if (pos >= totalVoos-1)
            {
                pos = totalVoos -1;
            }
            else
            {
                pos++;
            }
        }
        else if ((posChar == 'w') || (posChar == 'W'))
        {
            if (pos <= 0)
            {
                pos = 0;
            }
            else
            {
                pos--;
            }
        }
    }

    // Retornar a posicao final.
    return pos;
}

/*
    Fun��o para cadastrar uma reserva,
    vai chamar as fun��es para cadastrar cada valor de uma reserva.
    e no final adicionar a reserva ao vetor e por fim adicionar no .csv
*/
void cadastrarReserva()
{
    Reserva reservaACad;
    mostrarHub();
    printf ("                                                                      >>>> Cadastro de Reserva <<<<\n");
    
    // Recebendo numVoo da reserva e o indice do mesmo numa paulada s� !
    #if defined(_WIN32) || defined(_WIN64)
        reservaACad.indiceVoo = selecionarNumVooRes();
        strcpy(reservaACad.numVooRes, todosVoos[reservaACad.indiceVoo].numVoo);
    #else
        reservaACad.indiceVoo = cadastrarNumVooRes(reservaACad.numVooRes);
    #endif

    // N�mero da reserva.
    cadastrarNumRes(reservaACad.numRes);

    // CPF
    if (cadastrarCPF(reservaACad.CPF, reservaACad.indiceVoo) == -1) // Se for um CPF pertencente a um cliente existente.
    {
        int indiceCliente = buscarCPFClientes(reservaACad.CPF);

        // Aproveitar o nome.
        strcpy(reservaACad.nomeCompleto, todosClientes[indiceCliente].ClienteNome);

        //Aproveitar o sexo  ( ?� ?? ?�).
        reservaACad.sexo = todosClientes[indiceCliente].ClienteSexo;

        // Aproveitar a data de nascimento.
        reservaACad.dataNascimento = todosClientes[indiceCliente].ClienteNascimento;

    }
    else // � um cliente novo, registrar todos os dados.
    {
        // Cadastrar nome da pessoa.
        cadastrarNome(reservaACad.nomeCompleto);

        // Sexo '-'
        reservaACad.sexo = cadastrarSexo();

        // Data.
        reservaACad.dataNascimento = cadastrarDataNascimento();
    }

    // Status, considerar ativa durante o cadastro
    // reservaACad.statusRes = cadastrarStatusReserva();
    reservaACad.statusRes = 1;

    // Est�tica/Confirma��o
    int confirma;
    char confirmaChar[10];
    do
    {
        mostrarHub();
        printf ("                                                                      >>>> Cadastro de Reserva <<<<\n");
        printf ("\n                                                                          >> Sua Reserva <<");
        printf ("\n                                                                      N�mero Reserva:%s",reservaACad.numRes);
        printf ("\n                                                                      N�mero Voo:%s", reservaACad.numVooRes);
        printf ("\n                                                                      Data de Partida: %s", todosVoos[reservaACad.indiceVoo].DataPartida.dataChar);
        printf ("\n                                                                      Hora de Partida: %s", todosVoos[reservaACad.indiceVoo].HoraPartida.horaChar);
        printf ("\n                                                                      Data Nasimento: %s",reservaACad.dataNascimento.dataChar);
        printf ("\n                                                                      Sexo: %c", reservaACad.sexo);
        printf ("\n                                                                      Nome: %s", reservaACad.nomeCompleto);
        printf ("\n                                                                      CPF: %s", reservaACad.CPF);
        printf ("\n");  
        printf ("\n                                                                      >> Confirmar Reserva ? <<");
        printf ("\n                                                                              n1. Sim");
        printf ("\n                                                                              n2. N�o");
        printf ("\n                                                                      >> Insira aqui: n");
        fflush(stdin);
        fgets(confirmaChar, 10, stdin);
        confirmaChar[strlen(confirmaChar)-1] = '\0';
        confirma = atoi(confirmaChar);
        printf (" <<");

    }while ((confirma != 1) && (confirma != 2));

    // De acordo com a confirma��o ou n�o.
    if (confirma == 1)
    {    
        
        // Registrando reserva no arquivo de reservas.
        registrarReserva(reservaACad);

        // Atualizar todos os cadastros
        atualizarCadastrosVoos();
        receberCadastrosReservas();
        gerarDadosClientes();

        
        espacamentoVertical(14);
        printf ("\n                                                                     RESERVA CADASTRADA COM SUCESSO");
    }
    else
    {
        espacamentoVertical(14);
        printf ("\n                                                                          RESERVA N�O CONCLU�DA");
    }

    printf ("\n                                                      >> DIGITE QUALQUER TECLA PARA RETORNAR AO MENU PRINCIPAL <<");
    fflush(stdin);
    getch();
}

// Fun��o para recber confirma��o de um voo, 1 pra sim 0 para n�o.
int confirmarCadastroVoo(Voo VooACon)
{
    int confirma;
    char confirmaCharTemp[10];

    do 
    {
        mostrarHub();
        printf ("\n                                                                       >>>> Cadastrando Voo <<<<\n");
        printf ("\n                                                                             >> Novo Voo <<\n");
        printf ("\n                                                                                %s", VooACon.numVoo);
        printf ("\n                                                                                  %4i", VooACon.maxPass);
        printf ("\n                                                                               %s", VooACon.DataPartida.dataChar);
        printf ("\n                                                                               %s", VooACon.HoraPartida.horaChar);


        printf ("\n                                                                   >> Confirmar cadastro do voo ? <<");
        printf ("\n                                                                               n1. Sim");
        printf ("\n                                                                               n2. N�o");
        printf ("\n                                                                       >> Insira aqui: ");
        fflush(stdin);
        fgets(confirmaCharTemp, 10, stdin);
        confirmaCharTemp[strlen(confirmaCharTemp)-1] = '\0';
        confirma = atoi(confirmaCharTemp);
        printf (" <<");

    }while ((confirma != 1) && (confirma != 2));

    if (confirma == 1){confirma = 1;}
    else{confirma = 0;}

    return confirma;
}

// Fun��o que simplesmente exibe os dados de uma dada reserva.
void exibirDadosReserva(int indiceResAEx)
{
    // N�mero do voo da reserva.
    printf ("\n                                                                           >> N�mero do voo <<");
    printf ("\n                                                                                %s", todasReservas[indiceResAEx].numVooRes);
    printf ("\n");

    // N�mero da reserva em quest�o.
    printf ("\n                                                                         >> N�mero da reserva <<");
    printf ("\n                                                                               %s", todasReservas[indiceResAEx].numRes);
    printf ("\n");

    // CPF
    printf ("\n                                                                          >> CPF cadastrado <<");
    printf ("\n                                                                             %s", todasReservas[indiceResAEx].CPF);
    printf ("\n");

    // Nome completo
    printf ("\n                                                                           >> Nome completo <<");
    printf ("\n                                                                       %s", todasReservas[indiceResAEx].nomeCompleto);
    printf ("\n");

    // Data de nascimento
    printf ("\n                                                                        >> Data de nascimento <<");
    printf ("\n                                                                               %s", todasReservas[indiceResAEx].dataNascimento.dataChar);
    printf ("\n");

    // status
    printf ("\n                                                                              >> Status <<");
    if (todasReservas[indiceResAEx].statusRes == 1)
    {
        printf ("\n                                                                                  Ativo");
    }
    else
    {
        printf ("\n                                                                                Cancelada");
    }
}

// Fun��o para receber a reserva a ser consultada, e retornar o �ndice.
int receberNumReservaACon()
{
    // Est�tica.
    mostrarHub();
    printf ("\n                                                                     >>>> Consultando Reserva <<<<\n");

    // Primeiro achar qual o n�mero da reserva.
    char numResACon[MAXnumReserva];
    int indiceReserva;
    do
    {
        printf ("\n                                                           Qual o n�mero da reserva que deseja consultar ?");
        printf ("\n                                                                >> Insira aqui: ");
        fflush(stdin);
        fgets(numResACon, MAXnumReserva, stdin);
        numResACon[strlen(numResACon) - 1] = '\0';
        printf (" <<");

        mostrarHub();
        printf ("\n                                                                     >>>> Consultando Reserva <<<<\n");
        indiceReserva = validarNumRes(numResACon);
        if (indiceReserva != -5) // Exibir mensagem de acordo com a invalidez.
        {
            if (indiceReserva == 1)
            {
                printf ("\n                                                       ! N�o achamos nem uma reserva cadastrada com esse n�mero !");
                printf ("\n                                                                            tente novamente");
                indiceReserva = -1;
            }
            else
            {
                printf ("\n                                                             ! N�mero de reserva inv�lido, tente novamente !");
            }
        }
        else // Caso seja um n�mero de reserva v�lido descobrir qual � a posi��o do mesmo.
        {
            indiceReserva = buscarNumRes(numResACon);
            if (indiceReserva == -1)
            {
                printf ("\n                                                                  !!!! Um erro inesperado aconteceu !!!!");
            }
        }
    }while (indiceReserva < 0);

    // Retornar o �ndice conseguido.
    return indiceReserva;
}

// Fun��o pra selecionar reserva a ser consultada.
int selecionarReservaACon()
{
    int i;

    char indicadoresE[MAXvoos];
    char indicadoresD[MAXvoos];
    limparVetorChar(indicadoresE, MAXvoos);
    limparVetorChar(indicadoresD, MAXvoos);
    
    int pos = 0;
    char input;

    while(1)
    {
        mostrarHub();
        printf ("\n                                                                     >>>> Consultando Reserva <<<<\n");
        printf ("\n                                                                        >> Selecione a Reserva <<\n");

        indicadoresD[pos] = '<';
        indicadoresE[pos] = '>';

        for (i = 0; i < totalReservas; i++)
        {
            printf ("\n                                                                           %c%c %s %c%c", indicadoresE[i], indicadoresE[i], todasReservas[i].numRes, indicadoresD[i], indicadoresD[i]);
        }
        input = getch();

        limparVetorChar(indicadoresE, MAXvoos);
        limparVetorChar(indicadoresD, MAXvoos);

        if (input == 13)
        {
            break;
        }
        else if ((input == 'S') || (input == 's'))
        {
            pos++;
            if (pos >= totalReservas)
            {
                pos = totalVoos - 1;
            }
        }
        else if ((input == 'W') || (input == 'w'))
        {
            pos--;
            if (pos < 0)
            {
                pos = 0;
            }
        }
    }

    return pos;
}


/*
    Fun��o para consultar uma reserva, solicita o n�mero at� ser v�lido, e ent�o exibe os dados.
*/
void consultarReserva()
{   
    // Est�tica.
    mostrarHub();
    printf ("\n                                                                     >>>> Consultando Reserva <<<<\n");

    // Primeiro achar qual o n�mero da reserva.
    int indiceReserva;
    // indiceReserva = selecionarReservaACon();
    indiceReserva = receberNumReservaACon();

    //Con o indice do n�mero da reserva "em m�os" basta exibir os dados.
    exibirDadosReserva(indiceReserva);
    espacamentoVertical(18);
    printf ("\n                                                        >> DIGITE QUALQUER TECLA PARA RETORNAR AO MENU PRINCIPAL <<");
    getc(stdin);
}

// Fun��o para exbir dados gerais dos clientes.
void exibirDadosCliente(int indiceCliente)
{
    printf ("\n                                                                               >> Nome <<");
    printf ("\n                                                                            %s", todosClientes[indiceCliente].ClienteNome);
    printf ("\n");

    printf ("\n                                                                               >> CPF <<");
    printf ("\n                                                                            %s", todosClientes[indiceCliente].ClienteCPF);
    printf ("\n");

    printf ("\n                                                                         >> Data nascimento <<");
    printf ("\n                                                                              %s",todosClientes[indiceCliente].ClienteNascimento.dataChar);
    printf ("\n");

    printf ("\n                                                                               >> Sexo <<");
    printf ("\n                                                                                   %c", todosClientes[indiceCliente].ClienteSexo);
    printf ("\n");

    printf ("\n                                                                        >> Total de reservas <<");
    printf ("\n                                                                                   %i", todosClientes[indiceCliente].qtdReservas);
    printf ("\n");

    printf ("\n                                                                     >> Total de reservas ativas <<");
    printf ("\n                                                                                   %i", todosClientes[indiceCliente].qtdReservasAtivas);
    printf ("\n");

}

// Fun��o para exibir TODAS as reservas de um cliente, espec�ficado pelo CPF.
void exibirReservasCliente(char CPFCliente[MAXCPF])
{   
    printf ("\n                                                                       >> Reservas cadastradas <<");
    char status[20];
    int spc = 0;
    int r;
    for (r = 0; r < totalReservas; r++)
    {
        // Caso seja a reserva esteja no CPF a consultar
        if (strcmp(CPFCliente, todasReservas[r].CPF) == 0)
        {
            if (todasReservas[r].statusRes == 1)
            {
                strcpy(status, "      ATIVO       ");
            }
            else
            {
                strcpy(status, "    CANCELADO     ");
            }
            if (spc == 0)
            {
                printf ("\n                                                | N�mero da Reserva | N�mero do Voo | Data do voo | Status da Reserva |");
            }
            spc++;  
            printf ("\n                                                |     %s     |   %s    | %s  | %s| ", 
                                        todasReservas[r].numRes,
                                        todasReservas[r].numVooRes,
                                        todosVoos[todasReservas[r].indiceVoo].DataPartida.dataChar,
                                        status);
        }
    }

    espacamentoVertical(12-spc);
    printf ("\n                                                    >>  DIGITE QUALQUER TECLA PARA RETORNAR AO MENU PRINCIPAL <<");
    fflush(stdin);
    getch();
}

// Fun��o para consultar um cliente.
void consultarCliente()
{
    // Est�tica.
    mostrarHub();
    printf ("\n                                                                      >>>> Consultando Cliente <<<<\n");
    
    // Receber CPF at� que seja inserido um v�lido.
    char CPFClienteACons[MAXCPF];
    int verify;
    do
    {
        printf ("\n                                                          Digite o nome do CPF do cliente que deseja consultar: ");
        printf ("\n                                                                        >> ");
        fflush(stdin);
        fgets(CPFClienteACons, MAXCPF, stdin);
        CPFClienteACons[strlen(CPFClienteACons) - 1] = '\0';
        printf (" <<");

        // Est�tica.
        mostrarHub();
        printf ("\n                                                                      >>>> Consultando Cliente <<<<\n");
        verify = validarCPF(CPFClienteACons);
        if (verify != 1)
        {
            printf ("\n                                                           ! O CPF inserido n�o � v�lido tente novamente !");

        }
        else // Se for v�lido verificar se o CPF pertence a um cliente.
        {
            verify = buscarCPFClientes(CPFClienteACons);
            if (verify == -1)
            {
                printf ("\n                                                     ! O CPF fornecido n�o corresponde a nem um de nossos clientes !");
                printf ("\n                                                                            tente novamente");
            }
        }
    }while(verify < 0);

    // Exibir dados gerais do cliente, tendo em mente que a "verify" agora contem a posi��o do cliente nos vetores.
    exibirDadosCliente(verify);


    // Exibir todas as reservas desse cliente.
    exibirReservasCliente(CPFClienteACons);
}

// Fun��o para receber o n�mero de voo a cancelar, n�o se encerra at� receber um n�mero de voo v�lido.
void receberNumVooACancelar(char *numVooACan)
{
    // Est�tica.
    mostrarHub();
    printf ("\n                                                                        >>>> Cancelando Voo <<<<\n");

    int verify;
    do
    {
        printf ("\n                                                                 >> Digite o n�mero de voo a cancelar <<");
        printf ("\n                                                                            >> ");
        fflush(stdin);
        fgets(numVooACan, MAXnumVoo, stdin);
        numVooACan[strlen(numVooACan) - 1] = '\0';
        printf (" <<");

        // Est�tica.
        mostrarHub();
        printf ("\n                                                                        >>>> Cancelando Voo <<<<\n");

        verify = acharNumVoo(numVooACan);
        if (verify == -1)
        {
            printf ("\n                                                                ! N�mero de voo inv�lido, tente novamente !");
        }
        else if (verify == -2)
        {
            printf ("\n                                                    ! O n�mero do voo inserido n�o foi encontrado no nosso sistema !");
            printf ("\n                                                                             tente novamente");
        }
        else if (verify == -4)
        {
            printf ("\n\n                                                                     ! UM ERRO INESPERADO OCORREU !");
        }
        else // Caso v�lido verificar se o voo j� n�o est� cancelado
        {
            if (todosVoos[verify].status == 0)
            {
                printf ("\n                                                   ! O n�mero do voo inserido, corresponde a um voo j� cancelado !");
                printf ("\n                                                                             tente novamente");
                verify = -1;
            }
            else if (todosVoos[verify].status == 2)
            {
                printf ("\n\n                                                    ! O n�mero do voo corresponde a um voo que j� aconteceu !");
                printf ("\n                                                                            tente novamente");
                verify = -1;
            }

        }
    }while (verify < 0);
}

//Fun��o para confirmar o cancelamento de um voo, 1 pra sim, 0 para n�o.
int confirmarCancelamentoVoo(Voo VooACan)
{
    int confirma;
    char confirmaCharTemp[10];
    do 
    {   mostrarHub();
        printf ("\n                                                                        >>>> Cancelando Voo <<<<\n");
        printf ("\n                                                                        >> Voo a ser Cancelado <<");
        printf ("\n                                                                                %s", VooACan.numVoo);
        printf ("\n                                                                               %s", VooACan.DataPartida.dataChar);
        printf ("\n                                                                                %s", VooACan.HoraPartida.horaChar);
        printf ("\n                                                                               %3i/%3i", VooACan.atualPass, VooACan.maxPass);
        printf ("\n                                                                      >> Confirmar Cancelamento ?<<");
        printf ("\n                                                                                 n1. Sim");
        printf ("\n                                                                                 n2. N�o");
        printf ("\n                                                                        >> Insira aqui: n");
        fflush(stdin);
        fgets(confirmaCharTemp, 10, stdin);
        confirmaCharTemp[strlen(confirmaCharTemp)-1] = '\0';
        confirma = atoi(confirmaCharTemp);
        printf (" <<");

    }while ((confirma != 1) && (confirma != 2));

    if (confirma == 1){return 1;}
    else{return 0;}
}

// Fun��o para selecionar e retornar um n�mero de ativo, S� FUNCIONA EM WINDOWS, devido a conio.h
int selecionarVooACanWin()
{   
    char indicadoresE[MAXvoos];
    char indicadoresD[MAXvoos];
    limparVetorChar(indicadoresE, MAXvoos);
    limparVetorChar(indicadoresD, MAXvoos);

    int i;
    int pos = 0;
    char posChar;
    while (1)
    {
        mostrarHub();
        printf ("\n                                                                        >>>> Cancelando Voo <<<<\n");
        printf ("\n\n                                                                          >> Selecione o Voo <<\n");
        indicadoresE[pos] = '>';
        indicadoresD[pos] = '<';

        for (i = 0; i < totalVoos; i++)
        {
            printf ("\n                                                                             %c%c %s %c%c\n", indicadoresE[i], indicadoresE[i], todosVoos[i].numVoo, indicadoresD[i], indicadoresD[i]);
        }
        fflush(stdin);
        posChar = getch();

        // Alterar a posicao do indicador de acordo com o input.
        limparVetorChar(indicadoresE, MAXvoos);    
        limparVetorChar(indicadoresD, MAXvoos);    

        if (posChar == 13)
        {
            if (todosVoos[pos].status == 1)
            {
                break;
            }
            else if (todosVoos[pos].status == 2)
            {
                mostrarHub();
                printf ("\n                                                                        >>>> Cancelando Voo <<<<\n");
                printf ("\n                                                               ! O Voo escolhido j� passou, ecolha outro !");
                espacamentoVertical(30);
                printf ("\n                                                          >> PRESSIONE ENTER PARA PROSSEGUIR COM O CADASTRO <<");
                fflush(stdin);
                getc(stdin);

            }
            else if (todosVoos[pos].status == 0)
            {
                mostrarHub();
                printf ("\n                                                                        >>>> Cancelando Voo <<<<\n");
                printf ("\n                                                           ! O Voo escolhido j� foi cancelado, ecolha outro !");
                espacamentoVertical(30);
                printf ("\n                                                         >> PRESSIONE ENTER PARA PROSSEGUIR COM O CADASTRO <<");
                fflush(stdin);
                getc(stdin);
            }
            pos = 0;
        }
        else if ((posChar == 's') || (posChar == 'S'))
        {   
            pos++;
            if (pos >= totalVoos-1)
            {
                pos = totalVoos -1;
            }
        }
        else if ((posChar == 'w') || (posChar == 'W'))
        {   
            pos--;
            if (pos < 0)
            {
                pos = 0;
            }
        }
    }

    // Retornar a posicao final.
    return pos;
}
// Fun��o para cancelar um voo, no geral chama as demais fun��es.
void cancelarVoo()
{
    // Est�tica.
    mostrarHub();
    printf ("\n                                                                        >>>> Cancelando Voo <<<<\n");

    // Receber n�mero do voo a cancelar.
    int indiceVooACan;
    #if defined(_WIN32) || defined(_WIN64)
        indiceVooACan = selecionarVooACanWin();
    #else
        char numVooACan[MAXnumVoo];
        receberNumVooACancelar(numVooACan);
        indiceVooACan= acharNumVoo(numVooACan);
    #endif


    // Confirmar ou abortar opera��o de acordo.
    if (confirmarCancelamentoVoo(todosVoos[indiceVooACan]) == 1)
    {   
        // Cacelar o voo baseado no n�mero fornecido.(s� alterar o status.)
        todosVoos[indiceVooACan].status = 0;

        // J� que o voo foi cancelado a quantidade de reservas agora tamb�m � zero.
        todosVoos[indiceVooACan].atualPass = 0;

        // Cancelar todas as reservas com aquele �ndice.
        int r;
        for (r = 0; r < totalReservas; r++)
        {
            if (todasReservas[r].indiceVoo == indiceVooACan)
            {
                todasReservas[r].statusRes = 0;
            }
        }

        // Atualizar todos os cadastros presentes no .csv.
        atualizarCadastrosVoos();
        atualizarCadastrosReservas();
        gerarDadosClientes();

        // Nem sei.]
        espacamentoVertical(20);
        printf ("\n\n                                                                       VOO CANCELADO COM SUCESSO");
    }
    else
    {   
        espacamentoVertical(20);
        printf ("\n                                                               CANCELAMENTO DE VOO ABORTADO COM SUCESSO");
    }
    
    printf ("\n                                                     >>  DIGITE QUALQUER TECLA PARA RETORNAR AO MENU PRINCIPAL <<");
    fflush(stdin);
    getc(stdin);

}

// Fun��o para receber um n�mero de voo a ser cancelado e retornar sua posi��o.
int receberNumVooACon()
{
    int verify;
    char numVooC[MAXnumVoo];
    do
    {   
        //Recebendo numVoo
        printf ("\n                                                                      Qual voo deseja consultar ?");
        printf ("\n                                                                     insira no formato JEB-XXXX");
        printf ("\n                                                                          >> ");
        fflush(stdin);
        fgets(numVooC, MAXnumVoo, stdin);
        numVooC[strlen(numVooC) - 1] = '\0';
        printf (" <<");

        //Verificando se a entrada � v�lida e imprimindo mensagem de acordo.
        mostrarHub();
        printf ("\n                                                                       >>>> Consultando Voo <<<<\n");
        verify = acharNumVoo(numVooC);
        if (verify == -1)
        {
            printf ("\n                                                             ! N�mero de voo inv�lido, tente novamente !");
        }
        else if (verify == -2)
        {
            printf ("\n                                                    ! O n�mero de voo inserido n�o corresponde a nenhum cadastrado !");
            printf ("\n                                                                          ! Tente novamente !");
        }
        else if (verify == -4)
        {
            printf ("\n                                                                !!!! ERRO INESPERADO NO PROGRAMA !!!!");
        }
    } while (verify < 0); 

    return verify;
}
// Fun��o para receber um n�mero de reserva a cancelar.
void receberNumResACan(char *numResACan)
{
    int verify;
    mostrarHub();
    printf ("\n                                                                      >>>> Cancelando Reserva <<<<\n");

    do
    {
        printf ("\n                                                             Digite o n�mero da reserva que deseja cancelar");
        printf ("\n                                                                         no formato GB-123456");
        printf ("\n                                                                           >> ");
        fflush(stdin);
        fgets(numResACan, MAXnumReserva, stdin);
        numResACan[strlen(numResACan)-1] = '\0';
        printf (" <<");

        verify = validarNumRes(numResACan);
        mostrarHub();
        printf ("\n                                                                      >>>> Cancelando Reserva <<<<\n");
        if (verify != -5)
        {
            if (verify == 1)
            {
                printf ("\n                                                         ! N�o foi encontrada nem uma reserva com esse n�mero !");
                printf ("\n                                                                             tente novamente");
            }
            else
            {
                printf ("\n                                                             ! O n�mero de reserva inserido n�o � v�lido !");
                printf ("\n                                                                             tente novamente");
            }
        }
        else    // Caso seja v�lido verificar se a reserva j� n�o est� cancelada
        {
            int posReserva = buscarNumRes(numResACan);
            if (todasReservas[posReserva].statusRes == 0)
            {
                printf ("\n\n                                                      ! A Reserva correspondente a esse n�mero j� est� cancelada !");
                printf ("\n                                                                            tente novamente");
                verify = -1;
            }
            else if (todasReservas[posReserva].statusRes == 2)
            {
                printf ("\n\n                                                  ! O n�mero de reserva inserida corresponde a um voo que j� passou !");
                printf ("\n                                                                            tente novamente");
                verify = -1;

            }
        }
    }while (verify  != -5);
}

//Fun��o para cancelar o cancelamento de uma reserva, 1 para sim, 0 para n�o
int confirmarCancelamentoReserva(Reserva ResACan)
{

    // Receber op��o do usu�rio at� ser v�lido.
    int confirma;
    char confirmaCharTemp[MAXCharTemp];
    do 
    {
        // Est�tica.
        mostrarHub();
        printf ("\n                                                                      >>>> Cancelando Reserva <<<<\n");

        printf ("\n                                                                     >> Reserva a ser cancelada <<");
        printf ("\n                                                                               %s", ResACan.numRes);
        printf ("\n                                                                                %s", ResACan.numVooRes);
        printf ("\n                                                                       %s", ResACan.nomeCompleto);
        printf ("\n                                                                             %s", ResACan.CPF);
        printf ("\n                                                                                   %c", ResACan.sexo);
        printf ("\n                                                                               %s", ResACan.dataNascimento.dataChar);
        
        printf ("\n");
        printf ("\n                                                                     >> Confirmar cancelamento ? <<");
        printf ("\n                                                                                n1.Sim");
        printf ("\n                                                                                n2.N�o");
        printf ("\n                                                                      >> Insira Aqui: n");
        fflush(stdin);
        fgets(confirmaCharTemp, MAXCharTemp, stdin);
        confirmaCharTemp[strlen(confirmaCharTemp)-1] = '\0';
        confirma = atoi(confirmaCharTemp);
        printf (" <<");

    }while ((confirma != 1) && (confirma != 2));

    // Retornar de acordo.
    if (confirma == 1){return 1;}
    else{return 0;}
}

// Fun��o para cancelar uma reserva, baseado no n�mero da reserva, chama as outras reservas.
void cancelarReserva()
{   
    // Est�tica.
    mostrarHub();
    printf ("\n                                                                      >>>> Cancelando Reserva <<<<\n");

    // Receber o n�mero da reserva a ser cancelado.
    char numResACan[MAXnumReserva];
    receberNumResACan(numResACan);

    // Cancelar ou abordar, de acorodo com a confirma��o.
    espacamentoVertical(22);
    if (confirmarCancelamentoReserva(todasReservas[buscarNumRes(numResACan)]) == 1)
    {
        // Alterar no vetor global de reservas.
        int posResACan = buscarNumRes(numResACan);
        todasReservas[posResACan].statusRes = 0;

        // Alterar no vetor global a quantidade de passageiros.
        int posVooAAlt = acharNumVoo(todasReservas[posResACan].numVooRes);
        todosVoos[posVooAAlt].atualPass--;

        //Atualizar todos os dados necess�rios
        totalReservasAtivas--;
        atualizarCadastrosVoos();
        atualizarCadastrosReservas();
        gerarDadosClientes();

        // Est�tica.
        printf ("\n                                                                     RESERVA CANCELADA COM SUCESSO");
    }
    else
    {   
        // Est�tica
        printf ("\n                                                             CANCELAENTO DE RESERVA ABORTADO COM SUCESSO");
    }    


    printf ("\n                                                      >> DIGITE QUALQUER TECLA PARA RETORNAR AO MENU PRINCIPAL <<");
    fflush(stdin);
    getc(stdin);
}

// Fun��o que recebe um n�mero de voo a ser exclu�do at� que seja v�lido e altera a vari�vel fornecida.
void receberNumVooAExc(char *numVooAExc)
{
    mostrarHub();
    printf ("\n                                                                         >> Excluir Voo <<\n");

    int verify;
    do 
    {
        printf ("\n                                                                 Digite o n�mero do voo que deseja excluir");
        printf ("\n                                                                            no formato JEB-1234");
        printf ("\n                                                                             >> ");
        fflush(stdin);
        fgets(numVooAExc, MAXnumVoo, stdin);
        numVooAExc[strlen(numVooAExc)-1] = '\0';
        printf (" <<");
        
        mostrarHub();
        printf ("\n                                                                           >> Excluir Voo <<\n");
        verify = acharNumVoo(numVooAExc);
        if (verify == -1)
        {
            printf ("\n                                                                 ! N�mero de voo inserido n�o � v�lido !");
            printf ("\n                                                                              tente novamente");

        }
        else if (verify == -2)
        {
            printf ("\n\n                                                  ! O n�mero de voo inserido n�o corresponde a nemhum voo cadastrado !");
            printf ("\n                                                                              tente novamente");
        }
    }while(verify < 0);
}

// Fun��o que dado o �ndice de um voo nos vetores o remove.
void excluirDadosVoo(int indiceVooAExc)
{   
    if (todosVoos[indiceVooAExc].status == 1){totalVoosAtivos--;}

    // S� tomar cuidaddo caso o voo a ser removido seja o �ltimo.
    if (indiceVooAExc == totalVoos -1)
    {
        totalVoos--;
    }
    else
    {   
        int i;
        for (i = indiceVooAExc; i < totalVoos; i++)
        {
            todosVoos[i] = todosVoos[i+1];
        }
        totalVoos--;
    }
}

// Fun��o que dado um �ndice de um voo escreve todas as reservas com exec��o das pertencentes a esse voo.
void excluirDadosReservas(char numVooAExc[MAXnumVoo])
{   
    FILE *cadastrosReservas = fopen(CADASTROS_RESERVAS, "w");

    // Cabe�alho
    fprintf (cadastrosReservas, CAB_RESERVAS);

    // Todas as reservas com exec��o das pertencentes ao voo fornecido.
    int r;
    for (r = 0; r < totalReservas; r++)
    {   
        if (strcmp(numVooAExc, todasReservas[r].numVooRes) != 0)
        {    
            fprintf(cadastrosReservas, RegistroReserva, todasReservas[r].indiceVoo,
                                                        todasReservas[r].numVooRes,
                                                        todasReservas[r].numRes,
                                                        todasReservas[r].CPF,
                                                        todasReservas[r].nomeCompleto,
                                                        todasReservas[r].sexo,
                                                        todasReservas[r].dataNascimento.dataChar,
                                                        todasReservas[r].dataNascimento.dia,
                                                        todasReservas[r].dataNascimento.mes,
                                                        todasReservas[r].dataNascimento.ano,
                                                        todasReservas[r].statusRes);
        }
    }

    totalReservas = totalReservas - todosVoos[acharNumVoo(numVooAExc)].totalReservasVoo;

    // AAAAAAAA
    fclose(cadastrosReservas);
}

// Fun��o para confirmar a exclus�o de um voo, 1 para sim 0 para n�o.
int confirmarExclusaoVoo(Voo VooAEx)
{
    // Est�tica.
    mostrarHub();
    printf ("\n                                                                           >> Excluir Voo <<\n");

    char confirmaCharTemp[MAXCharTemp];
    int confirma;
    int spc = 0;
    do 
    {
        printf ("\n                                                                          >> Voo a excluir <<");
        printf ("\n                                                                               %s", VooAEx.numVoo);
        printf ("\n                                                                               %3i/%3i", VooAEx.atualPass, VooAEx.maxPass);
        printf ("\n                                                                               %s", VooAEx.HoraPartida.horaChar);
        printf ("\n                                                                              %s", VooAEx.DataPartida.dataChar);
        if (VooAEx.atualPass > 0)
        {
            spc = exibirReservasAtivas(acharNumVoo(VooAEx.numVoo));
        }
        printf ("\n\n                                                                      >> Confirmar Cancelamento ?<<");
        printf ("\n                                                                                 n1. Sim");
        printf ("\n                                                                                 n2. N�o");
        printf ("\n                                                                        >> Insira aqui: n");
        fflush(stdin);
        fgets(confirmaCharTemp, 10, stdin);
        confirmaCharTemp[strlen(confirmaCharTemp)-1] = '\0';
        confirma = atoi(confirmaCharTemp);
        printf (" <<");

    }while ((confirma != 1) && (confirma != 2));

    // Espa�amento.
    espacamentoVertical(16-spc);
    if (confirma == 1){return 1;}
    else {return 0;}
}

// Fun��o para selecionar o voo a ser exclu�do.
int selecionarVooAExcWin()
{   
    char indicadoresE[MAXvoos];
    char indicadoresD[MAXvoos];
    limparVetorChar(indicadoresE, MAXvoos);
    limparVetorChar(indicadoresD, MAXvoos);

    int i;
    int pos = 0;
    char posChar;
    while (1)
    {
        mostrarHub();
        printf ("\n                                                                           >> Excluir Voo <<\n");
        printf ("\n\n                                                                          >> Selecione o Voo <<\n");
        indicadoresE[pos] = '>';
        indicadoresD[pos] = '<';

        for (i = 0; i < totalVoos; i++)
        {
            printf ("\n                                                                             %c%c %s %c%c\n", indicadoresE[i], indicadoresE[i], todosVoos[i].numVoo, indicadoresD[i], indicadoresD[i]);
        }
        fflush(stdin);
        posChar = getch();

        // Alterar a posicao do indicador de acordo com o input.
        limparVetorChar(indicadoresE, MAXvoos);    
        limparVetorChar(indicadoresD, MAXvoos);    

        if (posChar == 13)
        {
            break;
        }
        else if ((posChar == 's') || (posChar == 'S'))
        {
            if (pos >= totalVoos-1)
            {
                pos = totalVoos -1;
            }
            else
            {
                pos++;
            }
        }
        else if ((posChar == 'w') || (posChar == 'W'))
        {
            if (pos <= 0)
            {
                pos = 0;
            }
            else
            {
                pos--;
            }
        }
    }

    // Retornar a posicao final.
    return pos;
}

/*
    Fun��o para excluir um voo, vai chamar diversas fun��es e ent�o realizar as seguintes opera�oes.
    - Receber um n�mero de voo v�lido a ser exclu�do.
    - Excluir o voo dos vetores.
    - Excluir todas as reservas marcadas para aquele voo
    - atualizar dados das reservas.
    - atualizar dados dos voos.
    - atualizar dados dos clientes.
*/
void excluirVoo()
{
    // Est�tica.
    mostrarHub();
    printf ("\n                                                                          >> Excluir Voo <<\n");

    // Recebendo n�mero do voo que vai ser exclu�do.
    char numVooAEx[MAXnumVoo];
    #if defined(_WIN32) || defined(_WIN64)
        strcpy(numVooAEx,todosVoos[selecionarVooAExcWin()].numVoo);
    #else
        receberNumVooAExc(numVooAEx);
    #endif

    // Confirmando exclus�o do voo.
    if (confirmarExclusaoVoo(todosVoos[acharNumVoo(numVooAEx)]) == 1)
    {        
            // Remover voo dos vetores.
            excluirDadosReservas(numVooAEx); // excluir os dados das reservas primeiro, pois se n�o n�o encontra o n�mero do voo.
            excluirDadosVoo(acharNumVoo(numVooAEx));
            
            
            // Atualizar todos os dados como de costume
            atualizarCadastrosVoos();
            receberCadastrosReservas(); 
            atualizarCadastrosReservas();
            gerarDadosClientes();

            printf ("\n                                                                             VOO EXCLU�DO COM SUCESSO");

    }        
    else
    {
        printf ("\n                                                                        EXCLUS�O DE VOO ABORTADA");
    }

    printf ("\n                                                         >>  PRESSIONE ENTER PARA RETORNAR AO MENU PRINCIPAL <<");
    fflush(stdin);
    getch();
}

// Fun��o para "encerrar" o programa.
void sairDoPrograma()
{
    mostrarHub();
    espacamentoVertical(2);
    printf("\n                                                                 ! Programa finalizado com sucesso !");
}