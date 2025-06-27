#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "janela.h"
#include<stdbool.h>
#include<string.h>


#define LINHA 5
#define COLUNA 6

/*
*/

typedef enum {
    COR_VERMELHA,
    COR_VERDE,
    COR_AZUL,
    COR_AMARELA,
    COR_ROXA,
    COR_LARANJA,
    COR_ROSA,
    COR_MARROM
} colors;


typedef struct posAtual{
    int x; //coluna
    int y; //linha
}pos;

typedef struct aaaa{
    pos currentPos; //posicao do "cursor", onde o player está [y][x]
    int etapa;
    double tempo;
    double pontuacao;
    bool terminouJogo;
    int pecasRetiradas;
    double tempo_de_jogo;
    int board[LINHA][COLUNA];
}gameState;


//Utils

int tratarLimite(int valor, int limite, int direcao){
    valor+= direcao;
    if(valor < 0){
        return limite -1;
    }
    if(valor >= limite){
        return 0;
    }
    return valor;
}

bool primeiraLinhaVazia(gameState *game) {
    for (int col = 0; col < COLUNA; col++) {
        if (game->board[0][col] != -1) {
            return false;
        }
    }
    return true;
}



//MOVES

//Mover linhas
void rodarLinhaDireita(gameState *game) {
    int y = (int)game->currentPos.y; 
    int ultimo = game->board[y][COLUNA - 1];

    for (int i = COLUNA - 1; i > 0; i--) {
        game->board[y][i] = game->board[y][i - 1];
    }

    game->board[y][0] = ultimo;
}

void rodarLinhaEsquerda(gameState *game) {
    int y = game->currentPos.y; 
    int primeiro = game->board[y][0];

    for (int i = 0; i < COLUNA - 1; i++) {
        game->board[y][i] = game->board[y][i + 1];
    }

    game->board[y][COLUNA - 1] = primeiro;
}

void moveLinhas(gameState *game, tecla_t tec){

    if(tec == T_DIREITA){
        rodarLinhaDireita(game);
    }

    if(tec == T_ESQUERDA){
        rodarLinhaEsquerda(game);
    }

}

void moveColuna(gameState *game, tecla_t tec) {

    int origem = game->currentPos.x;
    int destino;

    if (tec == T_CIMA) {
        destino = tratarLimite(origem, COLUNA, +1); 
    } else if (tec == T_BAIXO) {
        destino = tratarLimite(origem, COLUNA, -1);
    } else {
        return;
    }

    //verificamos se temos algo na coluna de origem e se a na próxima a do topo tá vazio
    if (game->board[LINHA - 1][origem] != -1 && game->board[0][destino] == -1 ){
        game->board[0][destino] = game->board[LINHA - 1][origem];
        game->board[LINHA - 1][origem] = -1;
    }
    
}



//Mover Cursor
void moveCursorCima(gameState *game){
    game->currentPos.y = tratarLimite(game->currentPos.y, LINHA, -1);    
}

void moveCursorBaixo(gameState *game){
     game->currentPos.y = tratarLimite(game->currentPos.y, LINHA, +1);    
}

void moveCursorDireita(gameState *game){
    game->currentPos.x = tratarLimite(game->currentPos.x, COLUNA, +1);   
}
void moveCursorEsquerda(gameState *game){
    game->currentPos.x = tratarLimite(game->currentPos.x, COLUNA, -1);   
}

void moveCursor(gameState *game, tecla_t tec){
    switch (tec)
    {
    case T_W:
        moveCursorCima(game);
        break;
    case T_S:
        moveCursorBaixo(game);
        break;
    case T_A:
        moveCursorEsquerda(game);
        break;
    case T_D:
        moveCursorDireita(game);
        break;
    default:
        break;
    }


}


void limpaLinha(gameState *game){
    int linha = game->currentPos.y;
    for(int i = 0; i < COLUNA; i++){
        game->board[linha][i] = -1;
    }

    game->pontuacao -= 2;
}


bool movimentacao(gameState *game){

    if(j_tem_tecla()){
        tecla_t tec = j_tecla();
        if (tec == T_ESC) return false;
        moveLinhas(game, tec);
        moveColuna(game, tec);
        moveCursor(game, tec);
        if(j_shift() && tec == T_Q){
                limpaLinha(game);
        }


    }
    return true;
}

//automatismo

void tempo(gameState *game){
    int agora = (int)j_relogio();

    if (agora != (int)game->tempo_de_jogo) {
        game->tempo_de_jogo = agora;
        game->tempo += 1;
    }
}



void removerColunas(gameState *game, int col){
    for(int i = 0; i < LINHA; i++){
        game->board[i][col] = -1;
    }

    game->pecasRetiradas += 5;
}

void addPontos(gameState *game){
    
    //Cada peça removida vale um ponto na primeira etapa, dois na segunda, etc. A passagem da primeira para segunda etapa vale 50 pontos, da segunda para a terceira vale 100, etc.
    game->pontuacao += (game->etapa +1) * 5;
    

}

bool verificaSeColunaPodeSerLimpa(gameState *game, int col){ //pra limpar 

    int primeiro = game->board[0][col];
    if(primeiro == -1) return false;

    for(int i = 0; i < LINHA; i++){
        if(game->board[i][col] != primeiro){
            return false;
        }
    }
    return true;

}

void varreColunas(gameState *game){    

    for(int i = 0; i < LINHA; i++){
        if(verificaSeColunaPodeSerLimpa(game, i)){
            removerColunas(game, i);
            addPontos(game);
            
        }
    }

}


void seAvancaEtapa(){
    //eh por tempo né, tem que implementar ainda
}



void colocaNovaCorNaPrimeiraLinha(gameState *game) {
    int pos = rand() % COLUNA;
    int cor = rand() % (4 + game->etapa + 1);
    game->board[0][pos] = cor;
}


void gravidade(gameState *game) {
    bool houveMovimento;
    do {
        houveMovimento = false;
        for (int coluna = 0; coluna < COLUNA; coluna++) {
            for (int linha = LINHA - 2; linha >= 0; linha--) {
                if (game->board[linha][coluna] != -1 && game->board[linha + 1][coluna] == -1) {
      
                    game->board[linha + 1][coluna] = game->board[linha][coluna]; //troca o valor de baixo pelo de cima
                    game->board[linha][coluna] = -1; // coloca o de cima como -1, perdi o valor de cima / n importa
                    houveMovimento = true;
                }
            }
        }
    } while (houveMovimento);
}

void automatismo(gameState *game){
    varreColunas(game);
    gravidade(game);
    if(primeiraLinhaVazia(game) == true){
        colocaNovaCorNaPrimeiraLinha(game);
    }

    tempo(game);

    

}



//Inicializa
void preencherBoard(gameState *game){
    for (int i = 0; i < LINHA; i++) {
        for (int j = 0; j < COLUNA; j++) {
            game->board[i][j] = rand() % 8;
        }
    }
    game->tempo = j_relogio();
    game->pecasRetiradas = 0;
}

//Desenho tabuleiro
void printBoard(gameState *game) {
    printf("\033[H");
    
    for (int i = 0; i < LINHA; i++) {
        for (int j = 0; j < COLUNA; j++) {
            if ((int)game->currentPos.x == j && (int)game->currentPos.y == i) {
                printf("[%d]\t", game->board[i][j]); 
            } else {
                printf(" %d \t", game->board[i][j]);
            }
        }
        printf("\n");
    }


    printf("\n");
    printf("\n");
    printf("00:%d", (int)game->tempo);


}


int main(){
    srand(time(NULL));

    gameState game;
    memset(&game, -1, sizeof(game));
    tamanho_t tamanho = {800, 600};
    system("clear");
    j_inicializa(tamanho, "Meu Jogo");
    preencherBoard(&game);
    game.currentPos.x = 0;
    game.currentPos.y = 3;
    

    bool rodando = true;
    while (rodando) {
        rodando = movimentacao(&game);  
        automatismo(&game);
        printBoard(&game);
        j_mostra();
        j_cochila(0.03);
    }

    j_finaliza();



    return 0;
}