# Desesenvolvimento de um Jogo

Neste repositório está presente o código fonte de um jogo, inspirado em Candy Crush, desenvolvido em C.

## Sobre o Jogo

### Regras do Jogo

O jogo é um desafio de tabuleiro onde o objetivo principal é ganhar pontos removendo peças coloridas. O tabuleiro é composto por uma grade retangular com casas que podem estar vazias ou conter peças coloridas. O jogador pode realizar movimentos para alinhar e remover essas peças, com o intuito de acumular o maior número de pontos possível.

#### Estrutura do Tabuleiro
- O tabuleiro possui uma grade com **5 linhas** e **6 colunas** (definidas como constantes no código com `#define`).
- Cada casa no tabuleiro pode estar vazia ou conter uma peça colorida.
- O tabuleiro possui vizinhos em direções verticais e horizontais:
  - Cada casa tem **4 vizinhos**: acima, abaixo, à esquerda e à direita.
  - Exceções:
    - Casas da **primeira linha** não têm vizinho acima.
    - Casas da **última linha** não têm vizinho abaixo.
    - Casas da **primeira coluna** têm como vizinho a **última coluna**, e vice-versa (tabuleiro é "circular" horizontalmente).

#### Objetivo do Jogo
- O objetivo é remover peças coloridas do tabuleiro alinhando-as nas colunas.
- **Remoção de Peças**: Quando todas as casas de uma coluna contêm peças da mesma cor, essas peças são removidas do tabuleiro, deixando as casas dessa coluna vazias.

#### Dinâmica de Jogo
- **Queda das Peças**: Quando uma peça colorida encontra uma casa vazia na mesma coluna, ela "cai" para a casa vazia, movendo-se para a linha seguinte.
  - Isso garante que não haverá casas preenchidas logo acima de uma casa vazia.

- **Preenchimento Aleatório**: Quando todas as casas da **primeira linha** estiverem vazias, uma peça de cor aleatória será inserida em uma casa da linha, escolhida aleatoriamente.

#### Movimentos Disponíveis
- **Rodar uma Linha**: O jogador pode "rodar" uma linha para a esquerda ou para a direita. Todas as peças dessa linha são movidas para a casa vizinha, com a primeira e última colunas sendo consideradas vizinhas.

- **Rodar uma Coluna**: O jogador pode rodar uma coluna, desde que:
  1. A coluna tenha pelo menos uma casa ocupada.
  2. A coluna vizinha (à esquerda ou à direita) tenha pelo menos uma casa vazia.
  
  Ao rodar a coluna, a peça na última linha da coluna atual será movida para a primeira linha da coluna vizinha.

- **Limpar uma Linha**: O jogador pode optar por "limpar" uma linha, removendo todas as peças dessa linha do tabuleiro. As peças acima da linha removida irão cair, e novas peças serão inseridas na primeira linha. **Nota**: Limpar uma linha custa 2 pontos ao jogador.

#### Etapas do Jogo
- O jogo é composto por várias **etapas**, cada uma com **cores diferentes de peças**.
  - Na **primeira etapa**, existem **4 cores possíveis** para as peças.
  - Em cada etapa subsequente, uma cor adicional é adicionada.
  - Uma partida pode ter um **limite de etapas** (mínimo de 5 etapas). Caso o jogador passe da última etapa, ele reinicia a última etapa.

#### Requisitos para Avançar de Etapa
- Cada etapa do jogo termina por tempo.
- Para avançar para a próxima etapa, o jogador deve remover peças suficientes. O número mínimo de peças removidas deve ser **pelo menos a metade da capacidade total do tabuleiro**.
  - Exemplo: Se o tabuleiro tiver 30 casas, o jogador precisa remover no mínimo 15 peças para avançar para a próxima etapa.

#### Pontuação
- Cada peça removida no tabuleiro vale pontos de acordo com a etapa:
  - **Primeira etapa**: 1 ponto por peça removida.
  - **Segunda etapa**: 2 pontos por peça removida.
  - E assim por diante, aumentando 1 ponto por etapa.
  
- **Pontuação Extra ao Avançar de Etapa**: O jogador ganha pontos ao passar para a próxima etapa:
  - Passar da **primeira para a segunda etapa**: 50 pontos.
  - Passar da **segunda para a terceira etapa**: 100 pontos.
  - E assim por diante, com 50 pontos a mais por cada etapa.

#### Fim da Partida
- O jogo termina se o jogador não conseguir remover as peças suficientes para avançar para a próxima etapa.
- O programa deve exibir a **classificação** do jogador no final da partida, levando em consideração o histórico de partidas jogadas.

#### Recomeço de Jogo
- Ao final de uma partida, o jogador pode escolher jogar uma **nova partida** sem precisar encerrar o programa.

#### Resumo das Ações do Jogador:
- **Mover linhas**: Rodar uma linha para a esquerda ou para a direita.
- **Mover colunas**: Rodar uma coluna para a esquerda ou para a direita (desde que as condições sejam atendidas).
- **Limpar uma linha**: Remover todas as peças de uma linha, perdendo 2 pontos no processo.

### Configuração do Tabuleiro
- O tabuleiro deve ter **5 linhas e 6 colunas**. Esses valores podem ser ajustados no código por meio de constantes `#define`.

#### Início da Partida
- No início de cada partida, o tabuleiro será **preenchido aleatoriamente** com peças das cores disponíveis para a etapa atual.
