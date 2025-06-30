#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "janela.h"
#include <stdbool.h>
#include <string.h>


#define ARQUIVO_RECORDES "recordes.txt"
#define LINHA 5
#define COLUNA 6

/*
 */

typedef struct {
    char nome[5];
    int pontuacao;
} Recorde;

typedef struct posAtual
{
    int x; // coluna
    int y; // linha
} pos;

typedef struct aaaa
{

    //
    ponto_t origemTabuleiro; // canto superior esquerdo do tabuleiro
    tamanho_t tamanhoCelula; // largura e altura de uma célula
    // jogo
    pos currentPos; // posicao do "cursor", onde o player está [y][x]
    int etapa;
    int board[LINHA][COLUNA];
    int cronometro; // tempo de jogo em segundos
    int pontuacao;
    int pecasNecessariasParaAvancar;
    int pecasRetiradas;
    bool terminouJogo;
    bool encerrarPrograma;
    char nome[5];
    
    double tempoTotalDeJogo; // n sei
    
} gameState;

// Utils

Recorde *lerRecordes(int *qtd);

void gravarRecordes(Recorde *lista, int qtd);

void inserirNovoRecorde(Recorde **lista, int *qtd, char *nome, int pontos);

void exibirRecordes( Recorde *lista, int qtd);

void liberarRecordes(Recorde *lista);

int tratarLimite(int valor, int limite, int direcao)
{
    valor += direcao;
    if (valor < 0)
    {
        return limite - 1;
    }
    if (valor >= limite)
    {
        return 0;
    }
    return valor;
}

bool primeiraLinhaVazia(gameState *game)
{
    for (int col = 0; col < COLUNA; col++)
    {
        if (game->board[0][col] != -1)
        {
            return false;
        }
    }
    return true;
}

// MOVES

// Mover linhas
void rodarLinhaDireita(gameState *game)
{
    int y = (int)game->currentPos.y;
    int ultimo = game->board[y][COLUNA - 1];

    for (int i = COLUNA - 1; i > 0; i--)
    {
        game->board[y][i] = game->board[y][i - 1];
    }

    game->board[y][0] = ultimo;
}

void rodarLinhaEsquerda(gameState *game)
{
    int y = game->currentPos.y;
    int primeiro = game->board[y][0];

    for (int i = 0; i < COLUNA - 1; i++)
    {
        game->board[y][i] = game->board[y][i + 1];
    }

    game->board[y][COLUNA - 1] = primeiro;
}

void moveLinhas(gameState *game, tecla_t tec)
{

    if (tec == T_DIREITA)
    {
        rodarLinhaDireita(game);
    }

    if (tec == T_ESQUERDA)
    {
        rodarLinhaEsquerda(game);
    }
}

void moveColuna(gameState *game, tecla_t tec)
{

    int origem = game->currentPos.x;
    int destino;

    if (tec == T_CIMA)
    {
        destino = tratarLimite(origem, COLUNA, +1);
    }
    else if (tec == T_BAIXO)
    {
        destino = tratarLimite(origem, COLUNA, -1);
    }
    else
    {
        return;
    }

    // verificamos se temos algo na coluna de origem e se a na próxima a do topo tá vazio
    if (game->board[LINHA - 1][origem] != -1 && game->board[0][destino] == -1)
    {
        game->board[0][destino] = game->board[LINHA - 1][origem];
        game->board[LINHA - 1][origem] = -1;
    }
}

// Mover Cursor
void moveCursorCima(gameState *game)
{
    game->currentPos.y = tratarLimite(game->currentPos.y, LINHA, -1);
}

void moveCursorBaixo(gameState *game)
{
    game->currentPos.y = tratarLimite(game->currentPos.y, LINHA, +1);
}

void moveCursorDireita(gameState *game)
{
    game->currentPos.x = tratarLimite(game->currentPos.x, COLUNA, +1);
}
void moveCursorEsquerda(gameState *game)
{
    game->currentPos.x = tratarLimite(game->currentPos.x, COLUNA, -1);
}

void moveCursor(gameState *game, tecla_t tec)
{
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

void limpaLinha(gameState *game)
{
    int linha = game->currentPos.y;
    for (int i = 0; i < COLUNA; i++)
    {
        game->board[linha][i] = -1;
    }
    game->pontuacao -= 2;
}

void movimentacao(gameState *game)
{

    if (j_tem_tecla())
    {
        tecla_t tec = j_tecla();
        if (tec == T_ESC)
        {
            game->encerrarPrograma = true;
            return;
        }
        moveLinhas(game, tec);
        moveColuna(game, tec);
        moveCursor(game, tec);
        if (j_shift() && tec == T_Q)
        {
            limpaLinha(game);
        }
    }
}

// automatismo

void tempo(gameState *game) {
    int tempoAtual = (int)(j_relogio());
    int tempoDecorrido = tempoAtual - (int)game->tempoTotalDeJogo;

    if (tempoDecorrido > game->cronometro) {
        game->cronometro = tempoDecorrido;
    }
}

void removerColunas(gameState *game, int col)
{
    for (int i = 0; i < LINHA; i++)
    {
        game->board[i][col] = -1;
    }

    game->pecasRetiradas += 5;
}

void addPontos(gameState *game)
{

    game->pontuacao += (game->etapa + 1) * 5;
}

bool verificaSeColunaPodeSerLimpa(gameState *game, int col){ // pra limpar

    int primeiro = game->board[0][col];
    if (primeiro == -1) return false;

    for (int i = 0; i < LINHA; i++)
    {
        if (game->board[i][col] != primeiro)
        {
            return false;
        }
    }
    return true;
}

void varreColunas(gameState *game)
{

    for (int i = 0; i < COLUNA; i++)
    {
        if (verificaSeColunaPodeSerLimpa(game, i))
        {
            removerColunas(game, i);
            addPontos(game);
        }
    }
}

void passagemDeEtapa(gameState *game){
    game->cronometro = 0;
    game->pecasRetiradas = 0;
    game->etapa++;
    game->tempoTotalDeJogo = j_relogio();
}

void seAvancaEtapa(gameState *game)
{

    int tempoLimite = 5;
    int minimoPecas = (LINHA * COLUNA) / 2;
        
    bool tempoEsgotado = game->cronometro >= tempoLimite;
    bool atingiuMeta = game->pecasRetiradas >= minimoPecas;

    if (tempoEsgotado || atingiuMeta) {
        if (atingiuMeta) {
            passagemDeEtapa(game);
        } else {
            game->terminouJogo = true;
        }
    }
}

void colocaNovaCorNaPrimeiraLinha(gameState *game)
{
    int pos = rand() % COLUNA;
    int cor = rand() % (4 + game->etapa + 1);
    game->board[0][pos] = cor;
}

void gravidade(gameState *game)
{
    bool houveMovimento;
    do
    {
        houveMovimento = false;
        for (int coluna = 0; coluna < COLUNA; coluna++)
        {
            for (int linha = LINHA - 2; linha >= 0; linha--)
            {
                if (game->board[linha][coluna] != -1 && game->board[linha + 1][coluna] == -1)
                {

                    game->board[linha + 1][coluna] = game->board[linha][coluna]; // troca o valor de baixo pelo de cima
                    game->board[linha][coluna] = -1;                             // coloca o de cima como -1, perdi o valor de cima / n importa
                    houveMovimento = true;
                }
            }
        }
    } while (houveMovimento);
}

void automatismo(gameState *game)
{
    varreColunas(game);
    gravidade(game);
    if (primeiraLinhaVazia(game) == true)
    {
        colocaNovaCorNaPrimeiraLinha(game);
    }

    tempo(game);
    seAvancaEtapa(game);

}

// Inicializa
void preencherBoard(gameState *game)
{

    for (int i = 0; i < LINHA; i++)
    {
        for (int j = 0; j < COLUNA; j++)
        {
            game->board[i][j] = rand() % (4 + game->etapa); // game->etapa
        }
    }
}

void inicializaGame(gameState *game)
{

    preencherBoard(game);
    game->etapa = 0;
    game->pecasRetiradas = 0;
    game->cronometro = 0;
    game->pontuacao = 0;
    game->currentPos.x = 0;
    game->currentPos.y = 0;
    game->tempoTotalDeJogo = j_relogio();
    game->origemTabuleiro = (ponto_t){600 - 60 * 6, 150}; // canto superior esquerdo
    game->tamanhoCelula = (tamanho_t){60, 60};            // 60x60 px por célula
    game->terminouJogo = false;
    game->encerrarPrograma = false;
    game->pecasNecessariasParaAvancar = (LINHA * COLUNA) / 2;
}

// Desenho tabuleiro
void printBoard(gameState *game)
{
    printf("\033[H");

    for (int i = 0; i < LINHA; i++)
    {
        for (int j = 0; j < COLUNA; j++)
        {
            if ((int)game->currentPos.x == j && (int)game->currentPos.y == i)
            {
                printf("[%d]\t", game->board[i][j]);
            }
            else
            {
                printf(" %d \t", game->board[i][j]);
            }
        }
        printf("\n");
    }

    printf("Etapa: %d\nPecas retiradas nesta etapa: %d\n", game->etapa + 1, game->pecasRetiradas);
    printf("Tempo em segundos: %d", game->cronometro);
}

void desenhaComandos()
{
    ponto_t pos;
    pos.x = 200;
    pos.y = 475;
    char texto[100];
    cor_t cor = {1, 1, 1, 1};

    
    sprintf(texto, "Comandos");
    j_texto(pos, cor, texto);

    sprintf(texto, "Cima - Mover ultimo bloco da coluna para a próxima coluna");
    pos.y += 20;
    j_texto(pos, cor, texto);

    sprintf(texto, "Baixo - Mover ultimo bloco da coluna para a coluna anterior");
    pos.y += 20;
    j_texto(pos, cor, texto);

    sprintf(texto, "WASD - Mover cursor");
    pos.y += 20;
    j_texto(pos, cor, texto);

    sprintf(texto, "Esquerda e Direita - rodar linha");
    pos.y += 20;
    j_texto(pos, cor, texto);
}

void desenharHUD(gameState *game)
{
    ponto_t pos;
    cor_t branco = {1, 1, 1, 1};
    char texto[150];

    sprintf(texto, "Etapa: %d | Pontos: %02d | Tempo: %02d | Peças retiradas: %02d | Peças necessárias: %02d",
            game->etapa + 1, game->pontuacao, game->cronometro,
            game->pecasRetiradas, game->pecasNecessariasParaAvancar);

    //dá pra pegar o tamanho certinho pra ficar no meio
    retangulo_t bbox = j_texto_contorno(texto);
    pos.x = (800 - bbox.tamanho.largura) / 2;
    pos.y = 50;

    j_texto(pos, branco, texto);
    desenhaComandos();
}

cor_t corPorIndice(int corIdx)
{
    cor_t cores[] = {
        {0.8, 0.1, 0.1, 1}, // vermelho
        {0.1, 0.8, 0.1, 1}, // verde
        {0.1, 0.1, 0.8, 1}, // azul
        {0.9, 0.9, 0.3, 1}, // amarelo
        {0.7, 0.3, 0.7, 1}, // roxo
        {1.0, 0.6, 0.2, 1}, // laranja
        {1.0, 0.5, 0.7, 1}, // rosa
        {0.5, 0.3, 0.2, 1}, // marrom
    };
    if (corIdx >= 0 && corIdx < 8)
        return cores[corIdx];

    return (cor_t){0, 0, 0, 1};
}

void desenhaTabuleiro(gameState *game)
{

    int larguraTabuleiro = game->tamanhoCelula.largura * COLUNA;

    int larguraTela = 800;

    game->origemTabuleiro.x = (larguraTela - larguraTabuleiro) / 2;

    for (int lin = 0; lin < LINHA; lin++)
    {
        for (int col = 0; col < COLUNA; col++)
        {
            int corIdx = game->board[lin][col];

            ponto_t pos;
            pos.x = game->origemTabuleiro.x + col * game->tamanhoCelula.largura;
            pos.y = game->origemTabuleiro.y + lin * game->tamanhoCelula.altura;

            retangulo_t celula;
            celula.inicio = pos;
            celula.tamanho = game->tamanhoCelula;

            cor_t corCelula;
            if (corIdx == -1)
            {
                corCelula = (cor_t){0.2, 0.2, 0.2, 1.0}; // cinza
            }
            else
            {
                corCelula = corPorIndice(corIdx);
            }

            cor_t borda = {1, 1, 1, 1}; // branco

            j_retangulo(celula, 1.0, borda, corCelula);

            if (game->currentPos.x == col && game->currentPos.y == lin)
            {
                j_retangulo(celula, 3, (cor_t){1, 1, 1, 1}, (cor_t){0, 0, 0, 0});
            }
        }
    }
}

// saidas

void desenhaTelaFim(gameState *game) {

    //j_seleciona_fonte(NULL, 24);
    ponto_t pos = {350, 200};
    cor_t amarelo = {1.0, 1.0, 0.0, 1.0};
    retangulo_t ret;
    char msg[100];

    sprintf(msg, "FIM DE JOGO");
    ret = j_texto_contorno(msg);
    pos.x = (800 - ret.tamanho.largura) / 2;
    j_texto(pos, amarelo, msg);

    sprintf(msg, "Etapa: %d | Pontos: %d", game->etapa + 1, game->pontuacao);
    ret = j_texto_contorno(msg);
    pos.x = (800 - ret.tamanho.largura) / 2;
    pos.y += 100;
    j_texto(pos, amarelo, msg);

}

void lerApelido(gameState *game) {
    int maxTamanho = sizeof(game->nome);
    int len = 0;
    game->nome[0] = '\0';

    while (true) {

        ponto_t pos = {300, 250};
        cor_t branco = {1, 1, 1, 1};
        char texto[100];

        
        desenhaTelaFim(game);

        sprintf(texto, "Digite seu nome: %s_", game->nome);
        j_texto(pos, branco, texto);
        j_mostra();

        if (j_tem_tecla()) {
            tecla_t t = j_tecla();

            if (t == T_ENTER && len > 0) break;

            if (t == T_BACKSPACE && len > 0) {
                len--;
                game->nome[len] = '\0';
            }

            char c = t;
            if (c >= 32 && c <= 126 && len < maxTamanho - 1) {
                game->nome[len++] = c;
                game->nome[len] = '\0';
            }
        }

        j_cochila(0.05);
    }
}



void desenhaTelaDeJogo(gameState *game)
{
    desenhaComandos();
    desenharHUD(game);
    desenhaTabuleiro(game);
    j_mostra();
    
}

void loopRodada(gameState *game)
{
    while (!game->terminouJogo && !game->encerrarPrograma)
    {
        movimentacao(game);
        automatismo(game);
        desenhaTelaDeJogo(game);
        j_cochila(0.03);
    }
}

void recorde(gameState *game){

    int qtd;
    Recorde *lista = lerRecordes(&qtd);
    inserirNovoRecorde(&lista, &qtd, game->nome, game->pontuacao);
    gravarRecordes(lista, qtd);
    liberarRecordes(lista);
}




void loopPrincipal(gameState *game){
    while (!game->encerrarPrograma){
        inicializaGame(game);
        loopRodada(game);
        if (game->terminouJogo){
            lerApelido(game);
            recorde(game);
        }
    }
}


void telaInicio(){

    char texto[100];
    cor_t cor = {1,1,1,1};
    ponto_t pos;
    pos.y = 150;
    //dá pra pegar o tamanho certinho pra ficar no meio
    retangulo_t bbox = j_texto_contorno(texto);
    pos.x = (800 - bbox.tamanho.largura) / 2 - 5;
    sprintf(texto, "Novo Jogo");
    j_texto(pos, cor, texto);
}




int main()
{
    srand(time(NULL));
    gameState game;
    tamanho_t tamanho = {800, 600};
    j_inicializa(tamanho, "Meu Jogo");
    inicializaGame(&game);
    loopPrincipal(&game);


    

    j_finaliza();

    return 0;
}



Recorde *lerRecordes(int *qtd) {
    FILE *f = fopen(ARQUIVO_RECORDES, "r");
    if (!f) {
        *qtd = 0;
        return NULL;
    }

    Recorde *lista = NULL;
    *qtd = 0;

    char linha[100];
    while (fgets(linha, sizeof(linha), f)) {
        char *sep = strchr(linha, '|');
        if (!sep) continue;

        *sep = '\0';
        Recorde r;
        strncpy(r.nome, linha, 5);
        r.nome[5 - 1] = '\0';
        r.pontuacao = atoi(sep + 1);

        lista = realloc(lista, (*qtd + 1) * sizeof(Recorde));
        lista[*qtd] = r;
        (*qtd)++;
    }

    fclose(f);
    return lista;
}

void gravarRecordes(Recorde *lista, int qtd) {
    FILE *f = fopen(ARQUIVO_RECORDES, "w");
    if (!f) return;

    for (int i = 0; i < qtd; i++) {
        fprintf(f, "%s|%d\n", lista[i].nome, lista[i].pontuacao);
    }

    fclose(f);
}

void inserirNovoRecorde(Recorde **lista, int *qtd, char *nome, int pontos) {
    *lista = realloc(*lista, (*qtd + 1) * sizeof(Recorde));

    Recorde novo;
    strncpy(novo.nome, nome, 5);
    novo.nome[5 - 1] = '\0';
    novo.pontuacao = pontos;

    (*lista)[*qtd] = novo;
    (*qtd)++;

    for (int i = 0; i < *qtd - 1; i++) {
        for (int j = i + 1; j < *qtd; j++) {
            if ((*lista)[j].pontuacao > (*lista)[i].pontuacao) {
                Recorde tmp = (*lista)[i];
                (*lista)[i] = (*lista)[j];
                (*lista)[j] = tmp;
            }
        }
    }
}


void exibirRecordes(Recorde *lista, int qtd) {
    printf("===== RECORDES =====\n");
    for (int i = 0; i < qtd; i++) {
        printf("%d. %s - %d\n", i + 1, lista[i].nome, lista[i].pontuacao);
    }
}

void liberarRecordes(Recorde *lista) {
    free(lista);
}