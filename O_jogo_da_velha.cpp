//                Universidade Federal de Ouro Preto - UFOP
//Trabalho Final:         Jogo da velha
//Aluno:            Marcelo Edivan Freitas Santos
//Matrícula:                16.2.4040

#include<iostream>
#include<iomanip>
#include<locale.h>
#include<cstdlib>
#include<ctime>
#include<fstream>
#include<cstring>
#include<stdio.h> // Biblioteca para __fpurge.
#define x 50
using namespace std;

struct Dados{  // Struct para os dados dos jogadores.
    char nome[x];
    int vitorias;
    int empates;
    int derrotas;
};

void inicio(char jogo[9]); // Funcção para completar a matriz do jogo.
void imprimir(char jogo[9]);// Função para imprimir a matriz do jogo.
void modo(char& em); // verificar o modo de jogo.
void simbolo(char& s1, char& s2, Dados jogadores[2]); // Simbolo dos jogadores.
void opcoes(char& op, Dados jogadores[2]); // Menu de opções.
void jogoDV(char jogo[9], char& em, char& s1, char& s2, char& op, Dados jogadores[2]); // O jogo em si;
void marcaj (char jogo[9], int& pos, char& sj, int& aux, Dados jogadores[2]); // Marcações dos jogadores.
void marcapc(char jogo[9], int& pos, char& sp, char& s1, int& aux, int& contjogadas); // Marcações do computador.
void verifica(char jogo[9], char& simj, int& aux); // Checa a vitória.
void jogadaDoPC(char jogo[9], char& s1, char& sp, int& pos); // Executa a marcação dos jogadores.
void nome_ranking(char& em, Dados jogadores[2]); // Função que trabalha com o resultado do menu de opções.
void imprimi_rank(void); // Função que imrpime o rank.
int grava_arq(Dados jogadores[2], int& ganhador, int& velha); // Grava os dados no arquivo.
void jogar_dnv(char& escolha, char& alteraop, int& loop); // Função que determina se o jogo será reexecutado.

int main() {
    setlocale(LC_ALL, "portuguese"); // Define o idioma como português.

    char jogo[9]; // Vetor do jogo.
    char em='0'; // Variavel (escolha modo de jogo).
    char s1='0', s2='0'; // Símbolo dos jogadores.
    char op='0'; // Opção do menu.
    char escolha; // Opção jogar novamente.
    char alteraop; // Opção para alterar o fluxo do loop.
    int loop = 1; // Variável de controle (jogar novamente).
    Dados jogadores[2]; // variável para receber os dados dos jogadores atuais.

    fstream ranks; // Criação do arquivo caso ele não exista.
    ranks.open("velha.ini");
    if(ranks.fail()){
        ranks.open("velha.ini", ios::out);
        ranks << "Computador\0" << endl;  // Um dos jogadores sempre será o Computador.
        ranks << 0 << endl;
        ranks << 0 << endl;
        ranks << 0 << endl;
        ranks.close();
    }
    cout << "Bem vindo ao jogo da velha." << endl;
    cout << "AVISO: nos menus predefinidos somente o 1° caracter será válido!" << endl;
    imprimi_rank(); // Ipressão do ranking.
    cout << endl;

    while(loop == 1 || loop ==  2){
        if(loop == 1){ // Se loop for igual a 1 as opções de jogo serão alteradas.
            do{
                modo(em); // Seleciona o modo de jogo.
                if(em != 'S')
                    nome_ranking(em, jogadores); // Atribui nomes aos jogadores ou imprime o ranking;
            }while(em == 'R' || em== 'D');

            if(em != 'S'){
                simbolo(s1,s2, jogadores); // Seleciona o simbolo.
                opcoes(op, jogadores); // Seleciona uma opção.
            }else{
                cout << "Você saiu do jogo!" << endl; // Se op for igual a 'S' o jogo será enceraado.
                return 0; // Encerra o programa caso op = 'S'.
            }
        }
        if(loop == 1 || loop ==2){ // Se loop for igual a 2 as opções de jogo não serão alteradas.
            inicio(jogo); // Preenche o jogo com as posições.
            jogoDV(jogo, em, s1, s2, op, jogadores); // Executa o jogo.
        }
        jogar_dnv(escolha, alteraop, loop); // Define o resultado da variavel loop que é responsavel pela resposta e controle do "jogar novamente".
    }
    return 0; // Encerra o programa.
}

void inicio(char jogo[9]){ // Cria a matriz do jogo.
    int cont=49; // Caracter 1 pela tabela ascii.
    for(int i=0; i<9; i++){
        jogo[i] = cont;
        cont ++;
    }
}

void modo(char& em){ // Executa o menu de opções do modo de jogo.
    cout << "Selecione o modo de jogo:\n";
    cout << "[C] Jogar contra o computador (jogador 2)\n";
    cout << "[P] Jogar contra outro jogador (jogador 1 e jogador 2)\n";
    cout << "[R] Mostar o ranking\n";
    cout << "[D] Deletar o ranking (começãr novo jogo)\n";
    cout << "[S] Se você deseja sair do jogo\n";
    cout << "Escolha: ";
    cin >> em; // Atribuição da escolha do modo de jogo.
    fflush(stdin);
    cout << endl;
    while((em != 'C') && (em != 'P') && (em != 'R') && (em != 'D' && (em != 'S'))){
        cout << "Seleção inválida!\n(confira se a seleção está em maiusculo)\n";
        cout << "Escolha [C] ou [P] ou [R] ou [D] ou [S]:";
        cin >> em;
        fflush(stdin);
        cout << endl;
    }
}

void simbolo(char& s1, char& s2, Dados jogadores[2]){ // Executa a escolha do simbolo.
    cout << jogadores[0].nome << " escolha o seu símbolo (O ou X): ";
    cin >> s1; // Atribuição da escolha do simbolo (jogador 1).
    while((s1!='X') && (s1!='O')){
        cout << "\nSímbolo inválido!";
        cout << "\n(confira se o símbolo está em maiusculo)";
        cout << "\nEscolha (O ou X): ";
        cin >> s1;
        fflush(stdin);
        cout << endl;
    }
    if(s1 == 'X')
        s2='O';
    else
        s2='X'; // Atribuição da escolha do simbolo (jogador 2).
    cout << jogadores[0].nome << ": " << s1 << endl;
    cout << jogadores[1].nome << ": " << s2 << endl << endl;
}

void opcoes(char& op, Dados jogadores[2]){ // Executa o menu de opções avançadas.
    cout << "Selecione uma das opções seguintes: \n";
    cout << "[1] " << jogadores[0].nome << " começa o jogo\n";
    cout << "[2] " << jogadores[1].nome << " começa o jogo\n";
    cout << "Escolha: ";
    cin >> op; // Atribuição da opção cujo o usuário escolheu.
    fflush(stdin);
    cout << endl;
    while(op!='1' && op!='2'){
        cout << "Opção inválida!\n";
        cout << "Escolha [1] ou [2]: ";
        cin >> op;
        fflush(stdin);
        cout << endl; // Verificação se a opção é válida.
    }
}

void imprimir(char jogo[9]){ // Imprime as posições do jogo.
    for(int i=0; i<=6; i=i+3){
        for(int j=i; j<3+i; j++){
            cout<< fixed << setw(2)<< jogo[j];
            if(j<(3+i)-1)
                cout<< fixed << setw(2) << "|";
        }
        if(i<6)
            cout << endl << "---|---|---" << endl;
    }
}

void jogoDV(char jogo[9], char& em, char& s1, char& s2, char& op, Dados jogadores[2]){ // Executa o jogo em si.
    int pos; // Posição das jogadas.
    int aux; // Critério de vitótia.
    int ganhador=0; // Quem foi o ganhador.
    int contjogadas=0; // contador de jogadas.
    int velha=0; // Criterio de empate.

    int teste;
    char teste2;

    if(em == 'C'){ // Executa se a opção de jogo for contra o computador.
        contjogadas = 0;
        aux = 0;
        while(aux == 0 && velha ==0){
            if(op == '1'  || contjogadas > 0){ // Execução do jogador 1 vs PC.
                contjogadas ++;
                imprimir(jogo);

                fflush(stdin);
                pos = -1;
                cout << endl <<"\n" << jogadores[0].nome <<  " Digite a posição que deseja marcar: ";
                teste = cin.peek();
                do{
                    if(isdigit(teste)){
                        cin >> pos;
                        fflush(stdin);
                        if(pos>9 || pos<1){ // Erro de loop infinito.
                            cout << "\nPosição inválida; tente novamente: "; //Impedir um loop infinito caso um caracter seja digitado para posição.
                            teste = cin.peek();
                        }
                    }else{
                        cin >> teste2;
                        fflush(stdin);
                        cout << "\nPosição inválida; tente novamente: ";
                        teste = cin.peek();
                    }
                }while(pos>9 || pos<1);

                marcaj(jogo,pos,s1,aux, jogadores); // Executa a marcação do jogador.

                cout << endl;
                if(contjogadas == 9) // Criterio de empate
                    velha = 1;
            }
            if(aux == 1)
                ganhador = 1; // Define de ganhador caso a definição seja verdadeira.

            if(ganhador == 0 && velha == 0){ // Execução do PC vs jogador 1.
                contjogadas ++;

                marcapc(jogo,pos,s2,s1,aux,contjogadas); // Executa a marcação do PC.

                if(aux == 1)
                    ganhador = 2; // Definição de ganhador caso a definição seja verdadeira.
                if(contjogadas == 9)
                    velha = 1;
            }
        }
    }else{ // Executa caso a opção de jogo seja de dois jogadores.
        contjogadas = 0;
        aux = 0;
        while(aux == 0 && velha ==0){
            if(op == '1'  || contjogadas > 0){
                imprimir(jogo);

                fflush(stdin);
                pos = -1;
                cout << endl <<"\n" << jogadores[0].nome << " Digite a posição que deseja marcar: ";
                teste = cin.peek();
                do{
                    if(isdigit(teste)){
                        cin >> pos;
                        fflush(stdin);
                        if(pos>9 || pos<1){ // Erro de loop infinito.
                            cout << "\nPosição inválida; tente novamente: "; //Impedir um loop infinito caso um caracter seja digitado para posição.
                            teste = cin.peek();
                        }
                    }else{
                        cin >> teste2;
                        fflush(stdin);
                        cout << "\nPosição inválida; tente novamente: ";
                        teste = cin.peek();
                    }
                 }while(pos>9 || pos<1);

                marcaj(jogo,pos,s1,aux,jogadores); // Executa a marcação do jogador.

                cout << endl;
                contjogadas ++;
                if(contjogadas == 9)
                    velha = 1;
            }
            if(aux == 1)
                ganhador = 1;

            if(ganhador == 0 && velha == 0){
                imprimir(jogo);

                fflush(stdin);
                pos = -1;
                cout << endl <<"\n" << jogadores[1].nome << " Digite a posição que deseja marcar: ";
                teste = cin.peek();
                do{
                    if(isdigit(teste)){
                        cin >> pos;
                        fflush(stdin);
                        if(pos>9 || pos<1){ // Erro de loop infinito.
                            cout << "\nPosição inválida; tente novamente: "; //Impedir um loop infinito caso um caracter seja digitado para posição.
                            teste = cin.peek();
                        }
                    }else{
                        cin >> teste2;
                        fflush(stdin);
                        cout << "\nPosição inválida; tente novamente: ";
                        teste = cin.peek();
                    }
                }while(pos>9 || pos<1);

                marcaj(jogo,pos,s2,aux,jogadores);

                cout << endl;
                contjogadas ++;
                if(aux == 1)
                    ganhador = 2;
                if(contjogadas == 9)
                velha = 1;
            }
        }
    }
    imprimir(jogo);
    if(aux == 1)
        cout << "\n\nParabés "<< jogadores[ganhador-1].nome << ", você ganhou!!";
    else
        cout << "\n\nFim de jogo. Deu velha!";

    grava_arq(jogadores, ganhador, velha); // Função para gravar no arquivo.

    cout << endl << endl;
    imprimi_rank(); // Imrpime o ranking ao final de cada jogo.
}


void marcaj (char jogo[9], int& pos, char& sj, int& aux, Dados jogadores[2]){ // Função para fazer a marcação do jogador no PC.
    int teste;
    char teste2;

    while (jogo[pos-1] == 'X' || jogo[pos-1] == 'O'){ // Executa caso a posição já esteja ocupada.
            cout << "\nPosição já selecionada!\n";
            fflush(stdin);
            pos = -1;
            cout <<"\n" << jogadores[0].nome << " Digite a posição que deseja marcar: ";
            teste = cin.peek();
            do{
                if(isdigit(teste)){
                    cin >> pos;
                    fflush(stdin);
                    if(pos>9 || pos<1){ // Erro de loop infinito.
                        cout << "\nPosição inválida; tente novamente: ";
                        teste = cin.peek();
                    }
                }
                else{
                    cin >> teste2;
                    fflush(stdin);
                    cout << "\nPosição inválida; tente novamente: ";
                    teste = cin.peek();
                }
                }while(pos>9 || pos<1);
            cout << endl;
    }
    jogo[pos-1] = sj;

    verifica(jogo, sj, aux); // Verifica se há vencedor.
}

void marcapc(char jogo[9], int& pos, char& sp, char& s1, int& aux, int& contjogadas){ // Função para fazer a marcação do PC no jogo.
    srand((unsigned)time(NULL));
    pos = rand()%9;

    if(contjogadas<4){
        while (jogo[pos] == 'X' || jogo[pos] == 'O'){ // Executa caso a posição já esteja ocupada.
            pos = rand()%9;
        }
        jogo[pos] = sp;
    }else{
        jogadaDoPC(jogo, s1, sp, pos); // Jogada do PC para bloquear o jogador.

        jogo[pos] = sp;
    }
    verifica(jogo, sp, aux); // Verifica se há vencedor.
}

void jogadaDoPC(char jogo[9], char& s1, char& sp, int& pos){ // Função que executa a jogada do PC para bloquear o jogador.

    srand((unsigned)time(NULL));

    if(((jogo[0]==sp && jogo[1]==sp) && (jogo[2]!=s1 && jogo[2]!=sp))|| ((jogo[0]==sp && jogo[2]==sp) && (jogo[1]!=s1 && jogo[1]!=sp))||((jogo[2]==sp && jogo[1]==sp) && (jogo[0]!=s1 && jogo[0]!=sp))){
        pos = rand()%3;
        while (jogo[pos] == 'X' || jogo[pos] == 'O')
            pos = rand()%3;

    }else if(((jogo[3]==sp && jogo[4]==sp) && (jogo[5]!=s1 && jogo[5]!=sp))|| ((jogo[3]==sp && jogo[5]==sp) && (jogo[4]!=s1 && jogo[4]!=sp))||((jogo[4]==sp && jogo[5]==sp) && (jogo[3]!=s1 && jogo[3]!=sp))){
        pos = rand()%3 +3;
        while (jogo[pos] == 'X' || jogo[pos] == 'O')
            pos = rand()%3 +3;

    }else if(((jogo[6]==sp && jogo[7]==sp) && (jogo[8]!=s1 && jogo[8]!=sp))|| ((jogo[6]==sp && jogo[8]==sp) && (jogo[7]!=s1 && jogo[7]!=sp))||((jogo[7]==sp && jogo[8]==sp) && (jogo[6]!=s1 && jogo[6]!=sp))){
        pos = rand()%3+6;
        while (jogo[pos] == 'X' || jogo[pos] == 'O')
            pos = rand()%3+6;

    }else if(((jogo[0]==sp && jogo[3]==sp) && (jogo[6]!=s1 && jogo[6]!=sp))|| ((jogo[0]==sp && jogo[6]==sp) && (jogo[3]!=s1 && jogo[3]!=sp))||((jogo[3]==sp && jogo[6]==sp) && (jogo[0]!=s1 && jogo[0]!=sp))){
        pos = rand()%3*3;
        while (jogo[pos] == 'X' || jogo[pos] == 'O')
            pos = rand()%3*3;

    }else if(((jogo[1]==sp && jogo[4]==sp) && (jogo[7]!=s1 && jogo[7]!=sp))|| ((jogo[1]==sp && jogo[7]==sp) && (jogo[4]!=s1 && jogo[4]!=sp))||((jogo[4]==sp && jogo[7]==sp) && (jogo[1]!=s1 && jogo[1]!=sp))){
        pos = rand()%3*3+1;
        while (jogo[pos] == 'X' || jogo[pos] == 'O')
            pos = rand()%3*3+1;

    }else if(((jogo[2]==sp && jogo[5]==sp) && (jogo[8]!=s1 && jogo[8]!=sp))|| ((jogo[2]==sp && jogo[8]==sp) && (jogo[5]!=s1 && jogo[5]!=sp))||((jogo[5]==sp && jogo[8]==sp) && (jogo[2]!=s1 && jogo[2]!=sp))){
        pos = rand()%3*3+2;
        while (jogo[pos] == 'X' || jogo[pos] == 'O')
            pos = rand()%3*3+2;

    }else if(((jogo[0]==sp && jogo[4]==sp) && (jogo[8]!=s1 && jogo[8]!=sp))|| ((jogo[0]==sp && jogo[8]==sp) && (jogo[4]!=s1 && jogo[4]!=sp))||((jogo[4]==sp && jogo[8]==sp) && (jogo[0]!=s1 && jogo[0]!=sp))){
        pos = rand()%3*4;
        while (jogo[pos] == 'X' || jogo[pos] == 'O')
            pos = rand()%3*4;

    }else if(((jogo[2]==sp && jogo[4]==sp) && (jogo[6]!=s1 && jogo[6]!=sp))|| ((jogo[2]==sp && jogo[6]==sp) && (jogo[4]!=s1 && jogo[4]!=sp))||((jogo[4]==sp && jogo[6]==sp) && (jogo[2]!=s1 && jogo[2]!=sp))){
        pos = rand()%3*2+2;
        while (jogo[pos] == 'X' || jogo[pos] == 'O')
            pos = rand()%3*2+2;

    }else if((jogo[0]==jogo[1] && (jogo[2]!=s1 && jogo[2]!=sp))|| (jogo[0]==jogo[2] && (jogo[1]!=s1 && jogo[1]!=sp))||(jogo[2]==jogo[1] && (jogo[0]!=s1 && jogo[0]!=sp))){
        pos = rand()%3;
        while (jogo[pos] == 'X' || jogo[pos] == 'O')
            pos = rand()%3;

    }else if((jogo[3]==jogo[4] && (jogo[5]!=s1 && jogo[5]!=sp))|| (jogo[3]==jogo[5] && (jogo[4]!=s1 && jogo[4]!=sp))||(jogo[4]==jogo[5] && (jogo[3]!=s1 && jogo[3]!=sp))){
        pos = rand()%3 +3;
        while (jogo[pos] == 'X' || jogo[pos] == 'O')
            pos = rand()%3 +3;

    }else if((jogo[6]==jogo[7] && (jogo[8]!=s1 && jogo[8]!=sp))|| (jogo[6]==jogo[8] && (jogo[7]!=s1 && jogo[7]!=sp))||(jogo[7]==jogo[8] && (jogo[6]!=s1 && jogo[6]!=sp))){
        pos = rand()%3+6;
        while (jogo[pos] == 'X' || jogo[pos] == 'O')
            pos = rand()%3+6;

    }else if((jogo[0]==jogo[3] && (jogo[6]!=s1 && jogo[6]!=sp))|| (jogo[0]==jogo[6] && (jogo[3]!=s1 && jogo[3]!=sp))||(jogo[3]==jogo[6] && (jogo[0]!=s1 && jogo[0]!=sp))){
        pos = rand()%3*3;
        while (jogo[pos] == 'X' || jogo[pos] == 'O')
            pos = rand()%3*3;

    }else if((jogo[1]==jogo[4] && (jogo[7]!=s1 && jogo[7]!=sp))|| (jogo[1]==jogo[7] && (jogo[4]!=s1 && jogo[4]!=sp))||(jogo[4]==jogo[7] && (jogo[1]!=s1 && jogo[1]!=sp))){
        pos = rand()%3*3+1;
        while (jogo[pos] == 'X' || jogo[pos] == 'O')
            pos = rand()%3*3+1;

    }else if((jogo[2]==jogo[5] && (jogo[8]!=s1 && jogo[8]!=sp))|| (jogo[2]==jogo[8] && (jogo[5]!=s1 && jogo[5]!=sp))||(jogo[5]==jogo[8] && (jogo[2]!=s1 && jogo[2]!=sp))){
        pos = rand()%3*3+2;
        while (jogo[pos] == 'X' || jogo[pos] == 'O')
            pos = rand()%3*3+2;

    }else if((jogo[0]==jogo[4] && (jogo[8]!=s1 && jogo[8]!=sp))|| (jogo[0]==jogo[8] && (jogo[4]!=s1 && jogo[4]!=sp))||(jogo[4]==jogo[8] && (jogo[0]!=s1 && jogo[0]!=sp))){
        pos = rand()%3*4;
        while (jogo[pos] == 'X' || jogo[pos] == 'O')
            pos = rand()%3*4;

    }else if((jogo[2]==jogo[4] && (jogo[6]!=s1 && jogo[6]!=sp))|| (jogo[2]==jogo[6] && (jogo[4]!=s1 && jogo[4]!=sp))||(jogo[4]==jogo[6] && (jogo[2]!=s1 && jogo[2]!=sp))){
        pos = rand()%3*2+2;
        while (jogo[pos] == 'X' || jogo[pos] == 'O')
            pos = rand()%3*2+2;

    }else{
        pos = rand()%9;
        while (jogo[pos] == 'X' || jogo[pos] == 'O')
            pos = rand()%9;
    }
}

void verifica(char jogo[9], char& simj, int& aux){ // Função que executa a verificação de vitória.
    int contl1=0, contl2=0, contl3=0, contc1=0, contc2=0, contc3=0, contd1=0, contd2=0; // Contadores das linhas, colunas e diagonais.
    for(int i=0; i<3; i++){
        if(jogo[i] == simj)
            contl1++;
        if(jogo[i+3] == simj)
            contl2++;
        if(jogo[i+6] == simj)
            contl3++;
        if(jogo[i*3] == simj)
            contc1++;
        if(jogo[i*3+1] == simj)
            contc2++;
        if(jogo[i*3+2] == simj)
            contc3++;
        if(jogo[i*4] == simj)
            contd1++;
        if(jogo[i*2+2] == simj)
            contd2++;
    }
    if(contc1 == 3)
        aux=1;
    else if(contc2 == 3)
        aux=1;
    else if(contc3 == 3)
        aux=1;
    else if(contl1 == 3)
        aux=1;
    else if(contl2 == 3)
        aux=1;
    else if(contl3 == 3)
        aux=1;
    else if(contd1 == 3)
        aux=1;
    else if(contd2 == 3)
        aux=1;
}

void jogar_dnv(char& escolha, char& alteraop, int& loop){
    loop = 0;
    cout << "\n\nDeseja jogar novamente? \n[S] SIM \n[N] NÃO\nEscolha: "; // Menu de opção de jogar novamente.
    cin >> escolha;
    fflush(stdin);
    while(escolha!='S' && escolha!='N'){ // Caso a opção for inválida aparecerar mensagem de erro.
        cout << "\n\nOpção inválida!\n(confira se a seleção está em maiusculo) \nEscolha [S] ou [N]: ";
        cin >> escolha;
        fflush(stdin);
    }
    if(escolha == 'S'){ // Menu segundario do menu da opção de jogar novamente.
        cout <<  "\n\nDeseja alterar alguma das opções de jogo? \n[S] SIM \n[N] NÃO\nEscolha: ";
        cin >> alteraop;
        fflush(stdin);
        cout << endl;
        while(alteraop!='S' && alteraop!='N'){ // Caso a opção for inválida aparecerar mensagem de erro.
            cout << "\n\nOpção inválida!\n(confira se a seleção está em maiusculo) \nEscolha [S] ou [N]: ";
            cin >> alteraop;
            fflush(stdin);
            cout << endl;
        }
        if(alteraop == 'S') // Checagem se as opções de menu principal serão alteradas.
            loop = 1;
        else
            loop = 2;
    }else{
        cout << "\nVocê saiu do jogo!" << endl;
        loop = 0;
    }
}

void nome_ranking(char& em, Dados jogadores[2]){
    int aux1=1, aux2=2;
    Dados pos[5];

    if(em == 'R'){
        imprimi_rank(); // Imprime o ranking.
        cout << endl;
        return ;

    }else if(em == 'D'){ // Deleta o ranking e começa um novo jogo.
        fstream ranks;
        ranks.open("velha.ini", ios::out);
        ranks << "Computador\0" << endl;
        ranks << 0 << endl;
        ranks << 0 << endl;
        ranks << 0 << endl;
        ranks.close();
        cout << "O ranking foi zerado com sucesso!" << endl << endl;
        return ;

    }else if(em == 'C'){ //Recebe o nomes dos jogadores.
        cout << "Digite o nome do jogador 1: ";
        cin >> jogadores[0].nome;
        fflush(stdin);

        strncpy(jogadores[1].nome, "Computador\0", x);

    }else if(em == 'P'){
        cout << "Digite o nome do jogador 1: ";
        cin >> jogadores[0].nome;
        fflush(stdin);

        cout << "Digite o nome do jogador 2: ";
        cin >> jogadores[1].nome;
        fflush(stdin);
    }
    cout << endl;

    fstream ranks;
    ranks.open("velha.ini", ios::in);

    for(int i=0; i<5; i++){
        strncpy (pos[i].nome, "\0", x);
        pos[i].vitorias = 0;
        pos[i].empates = 0;
        pos[i].derrotas = 0;
    }
    for(int i=0; i<5; i++){
        ranks >> pos[i].nome;
        ranks >> pos[i].vitorias;
        ranks >> pos[i].empates;
        ranks >> pos[i].derrotas;
    }
    ranks.close();

    for(int i=0; i<5; i++){ // Verifica se o jogador está no ranking.
        if(strcmp(jogadores[0].nome, pos[i].nome) == 0)
            aux1 = 0;
        if(strcmp(jogadores[1].nome, pos[i].nome) == 0)
            aux2 = 0;
    }
    for(int i=0; i<5; i++){ // Verifica se o jogador não está no ranking;
        if(strcmp(pos[i].nome, "\0")== 0){
            aux1 =0;
            aux2 =0;
        }
    }
    if(aux1==1 && aux2==2){
        cout << "\nOs jogadores não estão cadastrados no jogo: " << endl;
        nome_ranking(em, jogadores);
    }
    if(aux1==1 && aux2==0){
        cout << "\nO jogador " << jogadores[0].nome << " não está cadastrado no jogo: " << endl;
        nome_ranking(em, jogadores);
    }
    if(aux1==0 && aux2==2){
        cout << "\nO jogador " << jogadores[1].nome << " não está cadastrado no jogo: " << endl;
        nome_ranking(em, jogadores);
    }
}

void imprimi_rank(void){
    Dados pos[5];
    Dados aux;
    fstream ranks;

    ranks.open("velha.ini", ios::in);

    cout << "                     ..::RANKING::..    " << endl;
    cout << setw(15) << left <<"Nome";
    cout << setw(15) << right << "Vitórias";
    cout << setw(15) << "Empates";
    cout << setw(15) << "Derrotas" << endl;

    for(int i=0; i<5; i++){
        strncpy ( pos[i].nome, "\0", x);
        pos[i].vitorias = 0;
        pos[i].empates = 0;
        pos[i].derrotas = 0;
    }
    for(int i=0; i<5; i++){
        ranks >> pos[i].nome;
        ranks >> pos[i].vitorias;
        ranks >> pos[i].empates;
        ranks >> pos[i].derrotas;
    }
    ranks.close();

    for(int i = 0; i<5; i++){ // Estabelece a ordem do ranking;
        for(int j=i+1; j<5; j++){
            if((pos[i].vitorias < pos[j].vitorias)||((pos[i].vitorias == pos[j].vitorias)&&(pos[i].empates < pos[j].empates))||((pos[i].vitorias == pos[j].vitorias)&&(pos[i].empates == pos[j].empates)&&((strcmp(pos[i].nome, "\0")!=0)&&(strcmp(pos[j].nome, "\0")!=0))&&(pos[i].derrotas > pos[j].derrotas))){
                strncpy ( aux.nome, pos[i].nome, x);
                aux.vitorias = pos[i].vitorias;
                aux.empates = pos[i].empates;
                aux.derrotas = pos[i].derrotas;

                strncpy ( pos[i].nome, pos[j].nome, x);
                pos[i].vitorias = pos[j].vitorias;
                pos[i].empates = pos[j].empates;
                pos[i].derrotas = pos[j].derrotas;

                strncpy ( pos[j].nome, aux.nome, x);
                pos[j].vitorias = aux.vitorias;
                pos[j].empates = aux.empates;
                pos[j].derrotas = aux.derrotas;
            }
        }
    }
    for(int i=0; i<5; i++){
        cout << i+1 <<". " << setw(10) << left << pos[i].nome;
        cout << setw(15) << right << pos[i].vitorias;
        cout << setw(15) << pos[i].empates;
        cout << setw(15) << pos[i].derrotas << endl;
    }
    ranks.close();
}

int grava_arq(Dados jogadores[2], int& ganhador, int& velha){
    Dados pos[5];
    fstream ranks;

    ranks.open("velha.ini");
    if(ranks.fail()){
        cout << "\n\nO arquivo não existe!\n";
        return 0;
    }
    if(velha == 1){
        for (int i=0; i<2; i++){
            jogadores[i].vitorias = 0;
            jogadores[i].empates = 1;
            jogadores[i].derrotas = 0;
        }
    }
    if(ganhador == 1){
        jogadores[0].vitorias = 1;
        jogadores[0].empates = 0;
        jogadores[0].derrotas = 0;

        jogadores[1].vitorias = 0;
        jogadores[1].empates = 0;
        jogadores[1].derrotas = 1;
    }
    if(ganhador == 2){
        jogadores[0].vitorias = 0;
        jogadores[0].empates = 0;
        jogadores[0].derrotas = 1;

        jogadores[1].vitorias = 1;
        jogadores[1].empates = 0;
        jogadores[1].derrotas = 0;
    }

    for(int i=0; i<5; i++){
        strncpy ( pos[i].nome, "\0", x);
        pos[i].vitorias = 0;
        pos[i].empates = 0;
        pos[i].derrotas = 0;
    }

    for(int i=0; i<5; i++){
        ranks >> pos[i].nome;
        ranks >> pos[i].vitorias;
        ranks >> pos[i].empates;
        ranks >> pos[i].derrotas;
    }
    ranks.close();

    for(int i=0; i<2; i++){ // Gravação no arquivo.
        for(int j=0; j<5; j++){
            if(strcmp(jogadores[i].nome, pos[j].nome) == 0){
                pos[j].vitorias += jogadores[i].vitorias;
                pos[j].empates += jogadores[i].empates;
                pos[j].derrotas += jogadores[i].derrotas;
                j = 5;
            }
            else if( strcmp("\0", pos[j].nome) == 0){
                strncpy(pos[j].nome, jogadores[i].nome, x);
                pos[j].vitorias += jogadores[i].vitorias;
                pos[j].empates += jogadores[i].empates;
                pos[j].derrotas += jogadores[i].derrotas;
                j = 5;
            }
        }
    }
    ranks.open("velha.ini",ios::out);
    if(ranks.fail()){
        cout << "\nErro ao abrir o arquivo!\n";
        return 0;
    }
    for(int i=0; i<5; i++){
        if(strncmp("\0", pos[i].nome, x) == 0){
            ranks << "\n";
            ranks << "\n";
            ranks << "\n";
            ranks << "\n\n ";
        }
        else{
            ranks << pos[i].nome << "\n";
            ranks << pos[i].vitorias << "\n";
            ranks << pos[i].empates << "\n";
            ranks << pos[i].derrotas << "\n\n";
        }
    }
    ranks.close();
    return 0;
}
