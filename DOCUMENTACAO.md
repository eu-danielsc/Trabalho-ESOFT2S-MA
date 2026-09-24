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

4.1 Constantes e enumeradores

Antes de definir as estruturas, o projeto estabelece um conjunto de constantes simbólicas no arquivo tipos.h. Essa centralização evita "números mágicos" espalhados pelo código e facilita ajustes futuros. A seguir o exemplo das constantes que serão ultilizadas:
```c
#define MAX_NICKNAME        20
#define MAX_TITULO          50
#define MAX_JOGADORES       50
#define LARGURA_TELA        80   // colunas do terminal
#define ALTURA_TELA         24   // linhas do terminal
#define COLUNA_ALVO         10   // coluna da zona de acerto
#define JANELA_PERFEITO_MS  50   // tolerância (ms) para acerto perfeito
#define JANELA_BOM_MS       120  // tolerância (ms) para acerto bom; acima disso é erro
#define PONTOS_PERFEITO     100  // pontos por acerto perfeito
#define PONTOS_BOM          50   // pontos por acerto bom 
```
Além das constantes, o arquivo tipos.h define tipos enumerados, que representam os estados possíveis de cada entidade do sistema.
```c
typedef enum {
    NOTA_AGUARDANDO,  // ainda não entrou na tela
    NOTA_ATIVA,       // em movimento na tela
    NOTA_ACERTADA,    // jogador acertou a tecla a tempo
    NOTA_PERDIDA      // passou da zona de acerto sem resposta
} EstadoNota;

typedef enum { GATO_PARADO, GATO_BATE_ESQUERDA, GATO_BATE_DIREITA } EstadoGato;

typedef enum {
    TELA_MENU,
    TELA_JOGADORES,   // lista de jogadores / cadastro de nickname
    TELA_JOGO,
    TELA_FIM_PARTIDA, // resultado da partida
    TELA_COMO_JOGAR,
    TELA_SOBRE,
    TELA_RESULTADOS,  // nicknames e pontuações
    TELA_SAIR
} EstadoJogo;
```
4.2 Estruturas heterogêneas (struct)

Os dados do jogo são organizados em estruturas heterogêneas, que reúnem em um único tipo campos de naturezas diferentes (textos, números, estados). 
```c
typedef struct {
    long       tempo_ms;      // instante do acerto, em ms desde o início da fase
    char       tecla;         // tecla exigida do jogador
    int        coluna;        // posição horizontal atual na tela
    EstadoNota estado;
} Nota;
```
A estrutura Nota representa cada elemento em ASCII art que se desloca da direita para a esquerda.
```c
typedef struct {
    char  titulo[MAX_TITULO];
    int   bpm;                // batidas por minuto (andamento da música)
    int   velocidade;         // colunas que a nota percorre por segundo
    int   total_notas;        // quantidade de notas da fase
    Nota *notas;              // vetor de notas
} Fase;
```
A estrutura Fase representa uma música ou desafio completo, com título, andamento (bpm) e velocidade, que define quantas colunas a nota percorre por segundo e, portanto, a dificuldade.
```c
typedef struct {
    char nickname[MAX_NICKNAME];
    int  pontuacao;
    int  combo;                   // acertos consecutivos (zera ao errar)
    int  maior_combo;             // melhor combo da partida
    int  perfeitos;               // total de acertos perfeitos
    int  bons;                    // total de acertos bons
    int  erros;                   // total de erros
} Jogador;
```
A estrutura Jogador concentra tudo o que se acumula durante uma partida. Além do nickname, ela guarda a pontuacao total e o combo, que é a sequência atual de acertos consecutivos e zera a cada erro. Já maior_combo preserva o melhor combo da partida. Os contadores perfeitos, bons e erros alimentam a tela de resultado ao final da fase e permitem ao jogador avaliar seu desempenho.
```c
typedef struct {
    EstadoGato estado;
    int        quadro;                // quadro atual da animação
    long       ultima_batida_ms;      // instante da última batida no tambor
} Gato;
```
A estrutura Gato controla a animação do personagem que bate no tambor. O campo estado define a pose atual, quadro indica qual quadro da animação em ASCII art deve ser desenhado, e ultima_batida_ms registra o momento da última batida.
```c
 typedef struct {
    char nickname[MAX_NICKNAME];
    int  melhor_pontuacao;        // maior pontuação já obtida pelo jogador
} RegistroJogador;
```
A estrutura RegistroJogador corresponde a uma linha do cadastro gravado em arquivo, associando o nickname do jogador à sua melhor pontuação. Ela é a base tanto da tela de jogadores (opção 1 do menu) quanto da tela de resultados (opção 4).
```c
typedef struct {
    EstadoJogo      estado;                               // tela em que o programa está
    Jogador         jogador;                              // jogador da partida atual
    Fase            fase;                                 // fase carregada
    Gato            gato;
    RegistroJogador jogadores[MAX_JOGADORES];             // cadastro de jogadores
    int             total_jogadores;                      // quantos jogadores estão cadastrados
    char            tela[ALTURA_TELA][LARGURA_TELA + 1];  // quadro de desenho (buffer da tela)
    long            tempo_inicio_ms;                      // instante em que a partida começou
} Jogo;
```
Por fim, a estrutura Jogo reúne todas as demais em um único registro que representa o estado completo da aplicação: a tela atual, o jogador, a fase carregada, o gato, o cadastro de jogadores, a matriz de caracteres usada como quadro de desenho e o instante em que a partida começou.

