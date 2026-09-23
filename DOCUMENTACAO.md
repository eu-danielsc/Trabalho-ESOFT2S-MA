# Título do Projeto: Hora do gato maluco

`>1. Descrição do Sistema`
  
O projeto consiste em um jogo de ritmo desenvolvido em linguagem C e executado em terminal. O jogo apresentará elementos em ASCII art que se deslocaram da direita para a esquerda, de acordo com o ritmo. O jogador deverá pressionar uma determinada tecla no momento correto, de acordo com a posição dos elementos na tela. Como elemento visual, será utilizado um gato em ASCII Art, realizando movimentos de batida em um tambor, acompanhando o ritmo. O projeto também buscará explorar recursos sonoros no terminal, conforme as possibilidades técnicas da linguagem C e do ambiente de execução.

Além de proporcionar uma experiência interativa baseada em ritmo e tempo de resposta, o projeto apresenta uma proposta diferenciada de gamificação de trabalhos acadêmicos. Dessa forma, pretende-se verificar se a utilização de uma dinâmica gamificada pode contribuir para a consolidação e fixação dos conhecimentos trabalhados em atividades acadêmicas, tornando o processo de aprendizagem mais interativo e dinâmico.

`2. Fluxo de Utilização Esperado para o Sistema`

2.1. Ao iniciar o programa, o usuário irá visualizar um menu principal com as seguintes opções:

  `1.` Jogar  
  `2.` Como jogar  
  `3.` Sobre o jogo  
  `4.` Resultados  
  `5.` Sair  

2.2. Caso o usuário escolha a opção `1`, o programa irá exibir uma tela de jogadores, onde constará os nicknames de quem já jogou o jogo. Se for a primeira vez do usuário jogando, irá solicitar para ele inserir o nickname para poder começar a jogar.

2.3. Se o usuário escolher a opção `2`, irá mostrar um manual de como jogar e como a pontuação será contabilizada.

2.4. Se escolher a opção `3`, irá mostrar as informações sobre a proposta e finalidade do jogo.

2.5. Em caso de escolha a opção `4`, irá mostrar os nicknames cadastrados e suas respectivas pontuações.

2.6. Por fim, caso escolha a opção `5`, o jogo irá fechar.

`3. Fluxograma da Lógica do Sistema`

`4. Estrutura de Dados`

A estrutura de dados do jogo foi pensada para representar os quatro elementos centrais do sistema: as notas que se deslocam pela tela, pontuação e combo do jogador, o gato animado e a tela desenhada em ASCII. Os dados persistentes como fases e ranking ficam em arquivos de texto. Todos os tipos ficam em tipos.h, incluído pelos demais módulos.

4.1 Constantes

Antes de definir as estruturas, o projeto estabelece um conjunto de constantes simbólicas no arquivo tipos.h. Essa centralização evita "números mágicos" espalhados pelo código e facilita ajustes futuros: para mudar a dificuldade do jogo ou o tamanho da tela, basta alterar um único valor, sem procurar cada ocorrência nos módulos. A seguir o exemplo das constantes que serão ultilizadas:

- #define MAX_NOME            30
- #define MAX_TITULO          50
- #define MAX_RANKING         10
- #define LARGURA_TELA        80
- #define ALTURA_TELA         24
- #define COLUNA_ALVO         10
- #define JANELA_PERFEITO_MS  50
- #define JANELA_BOM_MS       120

As constantes "MAX_NOME" e "MAX_TITULO" definem o tamanho máximo das strings usadas para o nome do jogador e o título das fases, enquanto "MAX_RANKING" limita quantos registros o ranking mantém. LARGURA_TELA e ALTURA_TELA correspondem às dimensões do terminal padrão e determinam o tamanho da matriz que armazena o quadro a ser desenhado. "COLUNA_ALVO" indica a coluna em que fica a zona de acerto, ou seja, o ponto da tela em que a nota deve estar quando o jogador pressionar a tecla.

Por fim, "JANELA_PERFEITO_MS" e "JANELA_BOM_MS" regulam a precisão exigida do jogador. Elas expressam, em milissegundos, a diferença máxima aceita entre o instante em que a tecla é pressionada e o instante ideal da nota. Se a diferença for de até 50 ms, o acerto é considerado perfeito. Entre 50 ms e 120 ms, é considerado bom. Acima disso, conta como erro.

4.2 Estruturas heterogêneas (struct)

Os dados do jogo são organizados em estruturas heterogêneas, que reúnem em um único tipo campos de naturezas diferentes (textos, números, estados). 

```c
typedef struct {
    long       tempo_ms;   /* instante do acerto, em ms desde o início da fase */
    char       tecla;      /* tecla exigida do jogador */
    int        coluna;     /* posição horizontal atual na tela */
    EstadoNota estado;
} Nota;
```
A estrutura Nota representa cada elemento em ASCII art que se desloca da direita para a esquerda.
```c
  typedef struct {
      char  titulo[MAX_TITULO];
      int   bpm;
      int   velocidade;
      int   total_notas;
      Nota *notas;
  } Fase;
```
A estrutura Fase representa uma música ou desafio completo, com título, andamento (bpm) e velocidade, que define quantas colunas a nota percorre por segundo e, portanto, a dificuldade.
```c
  typedef struct {
      char nome[MAX_NOME];
      int  pontuacao;
      int  combo;
      int  maior_combo;
      int  perfeitos;
      int  bons;
      int  erros;
  } Jogador;
```
A estrutura Jogador concentra tudo o que se acumula durante uma partida. Além do nome, ela guarda a pontuacao total e o combo, que é a sequência atual de acertos consecutivos e zera a cada erro. Já maior_combo preserva o melhor combo da partida. Os contadores perfeitos, bons e erros alimentam a tela de resultado ao final da fase e permitem ao jogador avaliar seu desempenho.
```c
  typedef struct {
      EstadoGato estado;
      int        quadro;
      long       ultima_batida_ms;
  } Gato;
```
A estrutura Gato controla a animação do personagem que bate no tambor. O campo estado define a pose atual, quadro indica qual quadro da animação em ASCII art deve ser desenhado, e ultima_batida_ms registra o momento da última batida.
```c
  typedef struct {
      char nome[MAX_NOME];
      char fase[MAX_TITULO];
      int  pontuacao;
  } RegistroRanking;
```
A estrutura RegistroRanking corresponde a uma linha do ranking gravado em arquivo, associando o nome do jogador, a fase jogada e a pontuação obtida.
```c
  typedef struct {
      EstadoJogo estado;
      Jogador    jogador;
      Fase       fase;
      Gato       gato;
      char       tela[ALTURA_TELA][LARGURA_TELA + 1];
      long       tempo_inicio_ms;
  } Jogo;
```
Por fim, a estrutura Jogo reúne todas as demais em um único registro que representa o estado completo da aplicação: a tela atual, o jogador, a fase carregada, o gato, a matriz de caracteres usada como quadro de desenho e o instante em que a partida começou.

´Resumo da estrutura e Função no sistema´

- Nota:	Cada elemento ASCII que anda da direita para a esquerda; guarda quando e qual tecla deve ser pressionada.
- Fase:	Uma música/desafio: título, andamento e o vetor de notas.
- Jogador:	Pontuação, combo e estatísticas de acertos e erros.
- Gato:	Estado da animação do gato batendo no tambor.
- RegistroRanking:	Uma linha do ranking salvo em arquivo.
- Jogo:	Agrupa todo o estado da partida, passado por ponteiro às funções.

4.3 Estruturas homogêneas (vetores, strings e matrizes)
char tela[ALTURA_TELA][LARGURA_TELA + 1]: matriz de caracteres que funciona como buffer de vídeo. A cada quadro ela é limpa, recebe notas, gato e placar, e é impressa de uma só vez, o que evita cintilação no terminal.
const char *sprite_gato[NUM_QUADROS][LINHAS_GATO]: matriz de strings com os quadros da animação do gato em ASCII art.
Nota *notas: vetor de estruturas, ordenado por tempo_ms. Um índice da próxima nota evita percorrer o vetor inteiro a cada quadro.
RegistroRanking ranking[MAX_RANKING]: vetor de estruturas mantido em ordem decrescente de pontuação.
Strings (nome, titulo): vetores de char para nomes e títulos.

4.4 Organização (Arquivos e Conteúdos)

- main.c:	Laço principal e máquina de estados (EstadoJogo).
- tipos.h:	Constantes, enumerações e structs.
- jogo.h/.c:	Inicialização, atualização das notas, julgamento do acerto, pontuação e combo.
- fase.h/.c:	carregar_fase() e liberar_fase().
- render.h/.c:	Limpeza da tela, desenho de notas, gato e placar, impressão do buffer.
- ranking.h/.c:	Carregar, inserir ordenado e salvar o ranking.
