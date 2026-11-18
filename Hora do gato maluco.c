/*
ANTES DE INICIAR O PROGRAMA, execute esses passos:
	1. Abra a aba "Tools" (Ferramentas)
	2. Clique em "Compiler Options" (Opções do compilador)
	3. No espaço abaixo do marcador "Add following commands when following the linker" (que deve estar marcado), adicione ao final do texto o comando "-lwinmm" (copie o que está dentro das aspas)
	4. Clique em "Ok" para confirmar
*/



#include <stdio.h>
#include <stdlib.h>
#include <time.h>      // clock: marca o tempo da musica
#include <conio.h>     // _kbhit e _getch: leem uma tecla sem precisar do Enter
#include <windows.h>   // Sleep, PlaySoundA e a posicao do cursor no terminal
#define MAX_TITULO          50
#define MAX_JOGADORES       100
#define LARGURA_TELA        80   // colunas do terminal
#define ALTURA_TELA         24   // linhas do terminal
#define COLUNA_ALVO         10   // coluna da zona de acerto
#define JANELA_PERFEITO_MS  50   // tolerancia (ms) para acerto perfeito
#define JANELA_BOM_MS       120  // tolerancia (ms) para acerto bom; acima disso eh erro
#define PONTOS_PERFEITO     100  // pontos por acerto perfeito
#define PONTOS_BOM          50   // pontos por acerto bom

#define LARGURA_PISTA       78      // colunas da pista por onde as notas andam
#define MS_POR_COLUNA       20      // tempo (ms) que uma nota leva para andar uma coluna
#define MAX_NOTAS           800     // maximo de notas em um nivel
#define AJUSTE_AUDIO_MS     50      // atraso do som: aumente se as notas chegarem antes da batida, diminua se chegarem depois
#define TOTAL_NIVEIS        4       // quantas musicas (niveis) o jogo tem
 
// Nicknames e pontuacoes de quem ja jogou.
// Eles ficam guardados no arquivo "pontuacoes.txt": uma linha por jogador com o nickname
// e a melhor pontuacao dele em cada nivel (-1 = ainda nao jogou aquele nivel).
char nicknames[MAX_JOGADORES][21];
int pontuacoes[MAX_JOGADORES][TOTAL_NIVEIS];
int total_jogadores = 0;

int pontuacao_partida = 0;   // pontuacao da ultima partida jogada
int continuar = 1;           // 1 = o programa continua rodando, 0 = fecha o jogo
int nivel_escolhido = 0;     // nivel escolhido pelo jogador (0 = primeiro nivel da lista)

// Informacoes de cada nivel.
// A posicao 0 de cada vetor eh o nivel [1], a posicao 1 eh o nivel [2], e assim por diante.
char nome_nivel[TOTAL_NIVEIS][40] = {"Faded - Alan Walker", "Its Been So Long - The Living Tombstone", "In The End - Linkin Park", "Firestarter (8-bit) - K/DA"};
char arquivo_nivel[TOTAL_NIVEIS][20] = {"faded.wav", "TLT.wav", "in_the_end.wav", "firestarter.wav"};   // ficam na mesma pasta do jogo

// Dificuldade de 1 (muito facil) a 5 (muito dificil). Foi escolhida pela quantidade de notas
// por segundo e pela velocidade da musica: Faded tem 1,4 notas por segundo, It's Been So Long 1,7,
// In The End 2,0 e Firestarter 2,9 (com trechos de 4 notas por segundo).
int dificuldade_nivel[TOTAL_NIVEIS] = {2, 3, 4, 5};

int bpm_nivel[TOTAL_NIVEIS] = {90, 96, 105, 130};                      // batidas por minuto da musica
int primeira_batida_nivel[TOTAL_NIVEIS] = {523, 440, 1746, 461};       // momento (ms) em que comeca o compasso 0 dentro da musica
int duracao_nivel[TOTAL_NIVEIS] = {205000, 155400, 200000, 192500};    // duracao (ms) da musica
int compassos_nivel[TOTAL_NIVEIS] = {76, 62, 86, 103};                 // quantas linhas tem a matriz do nivel

// Os niveis: cada linha eh um compasso da musica (4 batidas) dividido em 8 partes iguais.
// 'X' = nota da pata esquerda, 'O' = nota da pata direita, '.' = sem nota.
// Para mudar um nivel basta trocar as letras.

// Nivel 1: Faded - Alan Walker (90 BPM)
char nivel_faded[76][9] = {
    "........",   //  0  silencio antes da musica
    "O...X...",   //  1  introducao: piano
    "O...X...",   //  2
    "O...X...",   //  3
    "O...X...",   //  4
    "O...X...",   //  5
    "O...X...",   //  6
    "O...X...",   //  7
    "O...X.X.",   //  8  a voz entra
    "O...X.X.",   //  9
    "O...X.X.",   // 10
    "O...X.X.",   // 11
    "O.X.O.X.",   // 12  primeira parte
    "O.X.O.X.",   // 13
    "O.X.O.X.",   // 14
    "O.X.O.X.",   // 15
    "X..OX..O",   // 16  preparacao: batida no contratempo
    "X..OX..O",   // 17
    "X..OX..O",   // 18
    "X..O....",   // 19
    ".X..X.X.",   // 20  subida
    "O.X.O.X.",   // 21  refrao (drop)
    "O.X.O.X.",   // 22
    "O.X.O.X.",   // 23
    "O.X.O.X.",   // 24
    "O.X.O.X.",   // 25
    "O.X.O.X.",   // 26
    "O.X.O.X.",   // 27
    "O.X.O.XX",   // 28
    "O.X.O.X.",   // 29
    "O.X.O.X.",   // 30
    "O.X.O.X.",   // 31
    "O.X.O.X.",   // 32
    "O.X.O.X.",   // 33
    "O.X.O.X.",   // 34
    "O.X.O.XX",   // 35
    "O.X.O.X.",   // 36  segunda parte
    "O.X.O.X.",   // 37
    "O.X.O.X.",   // 38
    "O.X.O.X.",   // 39
    "O.X.O.X.",   // 40
    "O.X.O.X.",   // 41
    "O.X.O.X.",   // 42
    "O.X.O.X.",   // 43
    "O.X.O.X.",   // 44
    "X..OX..O",   // 45  batida no contratempo
    "X..OX..O",   // 46
    "X..OX..O",   // 47
    "X..OX..O",   // 48
    "X..OX..O",   // 49
    "X..OX..O",   // 50
    "X..OX..O",   // 51
    "X..OX..O",   // 52
    "X..OX..O",   // 53  preparacao
    "X..OX..O",   // 54
    "X..OX..O",   // 55
    "X..OX...",   // 56
    "O.X.O.X.",   // 57  refrao final (drop)
    "O.X.O.X.",   // 58
    "O.X.O.X.",   // 59
    "O.X.O.X.",   // 60
    "O.X.O.X.",   // 61
    "O.X.O.X.",   // 62
    "O.X.O.X.",   // 63
    "O.X.O.XX",   // 64
    "O.X.O.X.",   // 65
    "O.X.O.X.",   // 66
    "O.X.O.X.",   // 67
    "O.X.O.X.",   // 68
    "O.X.O.X.",   // 69
    "O.X.O.X.",   // 70
    "O.X.O.X.",   // 71
    "O.X.O.XX",   // 72
    "O.......",   // 73  batida final
    "........",   // 74  fim
    "........"    // 75
};

// Nivel 2: FNF (96 BPM)
char nivel_gato[62][9] = {
    "........",   //  0  silencio antes da musica
    "....O.X.",   //  1  a musica comeca
    "O...X...",   //  2  introducao
    "O...X...",   //  3
    "O...X.O.",   //  4
    "O.X.O...",   //  5
    "X....X..",   //  6  parte calma
    "...X..X.",   //  7
    "..X..X..",   //  8
    "....O..O",   //  9  o baixo volta
    "O...X...",   // 10
    "O...X...",   // 11
    "O.X.O.X.",   // 12
    "O...X.XX",   // 13  virada da bateria
    "O.X.O.X.",   // 14  entra a bateria: O no bumbo, X na caixa
    "O.X.O.X.",   // 15
    "O.X.O.X.",   // 16
    "O.X.O.X.",   // 17
    "O.X.O.X.",   // 18
    "O.X.O.X.",   // 19
    "O.X.O.X.",   // 20
    "O.X.O.X.",   // 21
    "O.XOO.X.",   // 22  fica mais dificil
    "O.X.O.XO",   // 23
    "O.XOO.X.",   // 24
    "O.X.O.XO",   // 25
    "O.XOO.X.",   // 26
    "O.X.O.XO",   // 27
    "O.XOO.X.",   // 28
    "O.X.O.XO",   // 29
    "O.XOO.XO",   // 30  parte mais dificil
    "O.XOO.XO",   // 31
    "O.XOO.XO",   // 32
    "O.XOO.XO",   // 33
    "O.XOO.XO",   // 34
    "O.XOO.XO",   // 35
    "O.XOO.XO",   // 36
    "O.X.O...",   // 37  a bateria para
    "...X.X..",   // 38  parte calma
    "...X..X.",   // 39
    "..X.....",   // 40
    "....O..O",   // 41  o baixo volta
    "O...X...",   // 42
    "O...X...",   // 43
    "O.X.O.X.",   // 44
    "O...X.XX",   // 45  virada da bateria
    "O.XOO.X.",   // 46  a bateria volta
    "O.X.O.XO",   // 47
    "O.XOO.X.",   // 48
    "O.X.O.XO",   // 49
    "O.XOO.XO",   // 50  reta final
    "O.XOO.XO",   // 51
    "O.XOO.XO",   // 52
    "O.XOO.XO",   // 53
    "O.XOO.XO",   // 54
    "O.XOO.XO",   // 55
    "O.XOO.XO",   // 56
    "O.XOO.XO",   // 57
    "O.XOO.XO",   // 58
    "O.XOO.XO",   // 59
    "O.XOO.XO",   // 60
    "O......."    // 61  batida final
};

// Nivel 3: In The End - Linkin Park (105 BPM)
char nivel_in_the_end[86][9] = {
    "........",   //  0  silencio antes da musica
    "X.....X.",   //  1  introducao: piano
    "X.X.X.X.",   //  2
    "X.X.X.X.",   //  3
    "X.X.X.X.",   //  4
    "X.X.X.X.",   //  5
    "X.X.X.X.",   //  6
    "X..X..O.",   //  7  a bateria entra
    "O.X..OX.",   //  8  primeira parte: bumbo (O) e caixa (X)
    "O.X..OX.",   //  9
    "O.X..OX.",   // 10
    "O.X..OX.",   // 11
    "O.X..OX.",   // 12
    "O.X..OX.",   // 13
    "O.X..OX.",   // 14
    "O.X..OX.",   // 15
    "O.X..OX.",   // 16
    "O.X..OX.",   // 17
    "O.X..OX.",   // 18
    "O.X..OX.",   // 19
    "O.X..OX.",   // 20
    "O.X..OX.",   // 21
    "O.X..OX.",   // 22
    "O.X..OX.",   // 23
    "O.X.XOX.",   // 24  refrao
    "O.X.XOX.",   // 25
    "O.X.XOX.",   // 26
    "O.X.XOX.",   // 27
    "O.X.XOX.",   // 28
    "O.X.XOX.",   // 29
    "O.X.XOX.",   // 30
    "O.X.XOX.",   // 31
    "O.XO.OX.",   // 32  segunda parte
    "O.XO.OX.",   // 33
    "O.XO.OX.",   // 34
    "O.XO.OXX",   // 35
    "O.XO.OX.",   // 36
    "O.XO.OX.",   // 37
    "O.XO.OX.",   // 38
    "O.XO.OXX",   // 39
    "O.XO.OX.",   // 40
    "O.XO.OX.",   // 41
    "O.XO.OX.",   // 42
    "O.XO.OXX",   // 43
    "O.XO.OX.",   // 44
    "O.XO.OX.",   // 45
    "O.XO.OX.",   // 46
    "O.XO.OXX",   // 47
    "O.X.XOX.",   // 48  refrao
    "O.X.XOX.",   // 49
    "O.X.XOX.",   // 50
    "O.X.XOX.",   // 51
    "O.X.XOX.",   // 52
    "O.X.XOX.",   // 53
    "O.X.XOX.",   // 54
    "O.X.XOX.",   // 55
    "OOX...X.",   // 56  ponte
    "OOX...X.",   // 57
    "OOX...X.",   // 58
    "OOX...X.",   // 59
    "OOX...X.",   // 60
    "OOX...X.",   // 61
    "OOX...X.",   // 62
    "OOX...X.",   // 63
    "O.X.XOX.",   // 64  parte final
    "O.X.XOX.",   // 65
    "O.X.XOX.",   // 66
    "O.X.XOX.",   // 67
    "O.X.XOX.",   // 68
    "O.X.XOX.",   // 69
    "O.X.XOX.",   // 70
    "O.X.XOX.",   // 71
    "O.X.XOX.",   // 72
    "O.XXXOX.",   // 73  ultimo refrao
    "O.XXXOX.",   // 74
    "O.XXXOX.",   // 75
    "O.XXXOX.",   // 76
    "O.XXXOX.",   // 77
    "O.XXXOX.",   // 78
    "O.XXXOX.",   // 79
    "O.XXXOX.",   // 80
    "O.......",   // 81  fim da bateria
    "X...X...",   // 82  piano final
    "X...X...",   // 83
    "X...X...",   // 84
    "X......."    // 85
};

// Nivel 4: Firestarter (8-bit) - K/DA (130 BPM)
char nivel_firestarter[103][9] = {
    "........",   //  0  silencio antes da musica
    "........",   //  1
    "....X.X.",   //  2  introducao
    "X.......",   //  3
    ".....XO.",   //  4
    "O.X.O.X.",   //  5  primeira parte
    "O.X.O.X.",   //  6
    "O.X.O.X.",   //  7
    "O.X.O.X.",   //  8
    "O.XOO.XO",   //  9
    "O.XOO.XO",   // 10
    "O.XOO.XO",   // 11
    "O.XOO.XO",   // 12
    "O.XOO.XO",   // 13
    "O.XOO.XO",   // 14
    "O.XOO.XO",   // 15
    "O.XOO.XO",   // 16
    "OXO.OXO.",   // 17  refrao (drop): bumbo (O) e chimbal (X)
    "OXO.OXO.",   // 18
    "OXO.OXO.",   // 19
    "OXO.OXO.",   // 20
    "OXO.OXO.",   // 21
    "OXO.OXO.",   // 22
    "OXO.OXOX",   // 23
    "XO.X.XO.",   // 24  batida quebrada
    "XO.X.XO.",   // 25
    "XO.X.XO.",   // 26
    "XO.X.XO.",   // 27
    "XO.X.XO.",   // 28
    "XO.X.XO.",   // 29
    "XO.X.XO.",   // 30
    "XO.X.XO.",   // 31
    "XO.X.XO.",   // 32
    "X...X.O.",   // 33  parte calma
    "X...X.O.",   // 34
    "X...X.O.",   // 35
    "X...X.O.",   // 36
    "O.O.O.O.",   // 37  subida
    "OXOXOXOX",   // 38
    "OXOXOX..",   // 39
    "X..OO.O.",   // 40
    "OXO.OXO.",   // 41  refrao (drop)
    "OXO.OXO.",   // 42
    "OXO.OXO.",   // 43
    "OXOXOXO.",   // 44
    "OXOXOXO.",   // 45
    "OXOXOXO.",   // 46
    "OXOXOXOX",   // 47
    "O.......",   // 48  pausa
    "........",   // 49
    "....X.X.",   // 50
    "O...O...",   // 51
    "O.O.OXOX",   // 52  segunda parte
    "OXO.OXOX",   // 53
    "OXO.OXOX",   // 54
    "OXO.OXOX",   // 55
    "OXO.OXOX",   // 56
    "OXO.OXOX",   // 57
    "OXO.OXOX",   // 58
    "OXO.OXOX",   // 59
    "OXOXOXOX",   // 60
    "OXOXOXOX",   // 61
    "OXOXOXOX",   // 62
    "OXOXOXOX",   // 63
    "O.O.X...",   // 64  parte calma
    "X...X...",   // 65
    "X...X...",   // 66
    "X...X...",   // 67
    "....O.OO",   // 68  subida
    "O.OXO.OO",   // 69
    "OXOXOXOX",   // 70
    "OXOX....",   // 71
    "X.......",   // 72  pausa
    "....X.X.",   // 73
    "OXO.OXO.",   // 74  refrao final
    "OXO.OXO.",   // 75
    "OXO.OXO.",   // 76
    "OXO.OXO.",   // 77
    "OXO.OXO.",   // 78
    "OXO.OXO.",   // 79
    "OXO.OXO.",   // 80
    "OXO.OXOX",   // 81
    "OXO.OXOX",   // 82
    "OXO.OXOX",   // 83
    "OXO.OXOX",   // 84
    "OXO.OXOX",   // 85
    "OXO.OXOX",   // 86
    "OXO.OXOX",   // 87
    "OXOXOXOX",   // 88  reta final
    "OXOXOXOX",   // 89
    "OXOXOXOX",   // 90
    "OXOXOXOX",   // 91
    "OXOXO...",   // 92
    "OXOXOXOX",   // 93
    "OXOXOXOX",   // 94
    "OXOXOXOX",   // 95
    "OXOXO...",   // 96  final
    "........",   // 97
    "....X...",   // 98
    "........",   // 99
    "O...X...",   // 100
    "........",   // 101
    ".OXOX..."    // 102  batida final
};

// Le os nicknames e as pontuacoes salvos no arquivo (se o arquivo ainda nao existe, a lista fica vazia)
void CARREGAR_RESULTADOS(){

    FILE *arquivo;
    int n;

    total_jogadores = 0;
    arquivo = fopen("pontuacoes.txt", "r");
    if (arquivo != NULL) {
        // le o nickname e depois uma pontuacao para cada nivel
        while (total_jogadores < MAX_JOGADORES && fscanf(arquivo, "%20s", nicknames[total_jogadores]) == 1) {
            for (n = 0; n < TOTAL_NIVEIS; n++) {
                if (fscanf(arquivo, "%d", &pontuacoes[total_jogadores][n]) != 1) {
                    pontuacoes[total_jogadores][n] = -1;   // se faltar o numero, conta como "ainda nao jogou"
                }
            }
            total_jogadores++;
        }
        fclose(arquivo);
    }
}

// Escreve todos os nicknames e pontuacoes no arquivo
void SALVAR_RESULTADOS(){

    FILE *arquivo;
    int i, n;

    arquivo = fopen("pontuacoes.txt", "w");
    if (arquivo != NULL) {
        for (i = 0; i < total_jogadores; i++) {
            fprintf(arquivo, "%s", nicknames[i]);
            for (n = 0; n < TOTAL_NIVEIS; n++) {
                fprintf(arquivo, " %d", pontuacoes[i][n]);
            }
            fprintf(arquivo, "\n");
        }
        fclose(arquivo);
    }
}

// Leva o cursor para o canto de cima do terminal.
// Assim o proximo desenho fica por cima do anterior e a tela nao pisca.
void CURSOR_NO_TOPO(){

    COORD posicao;

    posicao.X = 0;
    posicao.Y = 0;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), posicao);
}

void TELA_SAIR(){

    printf("\nObrigado por jogar Hora do Gato Maluco! Ate a proxima.\n");
    continuar = 0;
}

// Mostra a opcao "Voltar ao menu?". Se o jogador responder N, o jogo fecha.
void VOLTAR_AO_MENU(){

    char resposta[100];
    int respondeu = 0;

    while (respondeu == 0) {
        printf("\nVoltar ao menu? (S/N): ");
        scanf("%99s", resposta);

        if ((resposta[0] == 'S' || resposta[0] == 's') && resposta[1] == '\0') {
            respondeu = 1;
        } else if ((resposta[0] == 'N' || resposta[0] == 'n') && resposta[1] == '\0') {
            respondeu = 1;
            TELA_SAIR();
        } else {
            printf("Resposta invalida\n");
        }
    }
}

// O jogo em si: toca a musica, move as notas e conta os pontos
void JOGO(){

    FILE *arquivo;
    int tempo_notas[MAX_NOTAS];    // momento (ms) em que cada nota chega na COLUNA_ALVO
    char tipo_notas[MAX_NOTAS];    // 'X' ou 'O'
    int total_notas = 0;
    int proxima = 0;               // primeira nota que ainda nao foi acertada nem perdida
    int perfeitos = 0, bons = 0, erros = 0;
    int colunas_perfeito = JANELA_PERFEITO_MS / MS_POR_COLUNA;
    int colunas_bom = JANELA_BOM_MS / MS_POR_COLUNA;
    char borda[LARGURA_PISTA + 1], pista[LARGURA_PISTA + 1];
    char tecla, olho_esquerdo, olho_direito, letra;
    int inicio, agora = 0, diferenca, distancia, coluna;
    int pata_x_ate = 0, pata_o_ate = 0;   // ate que momento (ms) cada pata fica abaixada
    int aviso = 0, aviso_ate = 0;         // aviso na tela: 1 = perfeito, 2 = bom, 3 = errou
    int sair = 0, c, s, i;

    pontuacao_partida = 0;

    // Transforma o desenho do nivel escolhido em uma lista de notas com o tempo de cada uma.
    // Cada parte do compasso dura meia batida: 60000 / bpm / 2 = 30000 / bpm milissegundos.
    // (a multiplicacao vem antes da divisao para a conta com int nao perder precisao)
    for (c = 0; c < compassos_nivel[nivel_escolhido]; c++) {
        for (s = 0; s < 8; s++) {

            // pega a letra na matriz do nivel escolhido
            if (nivel_escolhido == 0) letra = nivel_faded[c][s];
            else if (nivel_escolhido == 1) letra = nivel_gato[c][s];
            else if (nivel_escolhido == 2) letra = nivel_in_the_end[c][s];
            else letra = nivel_firestarter[c][s];

            if ((letra == 'X' || letra == 'O') && total_notas < MAX_NOTAS) {
                tempo_notas[total_notas] = primeira_batida_nivel[nivel_escolhido] + AJUSTE_AUDIO_MS + (c * 8 + s) * 30000 / bpm_nivel[nivel_escolhido];
                tipo_notas[total_notas] = letra;
                total_notas++;
            }
        }
    }

    // Monta as bordas da pista: '#' marca a zona do perfeito e '=' a zona do bom
    for (i = 0; i < LARGURA_PISTA; i++) {
        distancia = i - COLUNA_ALVO;
        if (distancia < 0) distancia = -distancia;

        if (distancia <= colunas_perfeito) borda[i] = '#';
        else if (distancia <= colunas_bom) borda[i] = '=';
        else borda[i] = '-';
    }
    borda[LARGURA_PISTA] = '\0';

    printf("\n");

    // Confere se o arquivo da musica esta na pasta
    arquivo = fopen(arquivo_nivel[nivel_escolhido], "r");
    if (arquivo == NULL) {
        printf("Aviso: o arquivo %s nao foi encontrado nesta pasta.\n", arquivo_nivel[nivel_escolhido]);
        printf("O jogo vai rodar sem som.\n\n");
    } else {
        fclose(arquivo);
    }

    printf("Teclas:  X = pata esquerda    O = pata direita    ESC = sair\n\n");
    printf("Aperte a tecla quando a nota estiver na zona marcada com #####.\n\n");
    printf("Pressione qualquer tecla para comecar...\n");
    _getch();
    system("cls");

    // Comeca a musica e marca o momento do inicio
    PlaySoundA(arquivo_nivel[nivel_escolhido], NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
    inicio = clock();

    while (sair == 0 && agora < duracao_nivel[nivel_escolhido]) {

        // tempo (ms) que passou desde o inicio da musica
        agora = (clock() - inicio) * 1000 / CLOCKS_PER_SEC;

        // Le as teclas apertadas
        while (_kbhit()) {
            tecla = _getch();
            if (tecla == 'x') tecla = 'X';
            if (tecla == 'o') tecla = 'O';

            if (tecla == 27) sair = 1;   // 27 eh a tecla ESC
            if (tecla == 'X') pata_x_ate = agora + 120;
            if (tecla == 'O') pata_o_ate = agora + 120;

            if ((tecla == 'X' || tecla == 'O') && proxima < total_notas) {
                // distancia (ms) entre o momento da tecla e o momento certo da proxima nota
                diferenca = tempo_notas[proxima] - agora;
                if (diferenca < 0) diferenca = -diferenca;

                if (diferenca <= JANELA_BOM_MS) {
                    if (tecla != tipo_notas[proxima]) {
                        erros++;        // tecla trocada
                        aviso = 3;
                    } else if (diferenca <= JANELA_PERFEITO_MS) {
                        pontuacao_partida = pontuacao_partida + PONTOS_PERFEITO;
                        perfeitos++;
                        aviso = 1;
                    } else {
                        pontuacao_partida = pontuacao_partida + PONTOS_BOM;
                        bons++;
                        aviso = 2;
                    }
                    aviso_ate = agora + 400;
                    proxima++;
                }
            }
        }

        // Notas que passaram da zona de acerto sem serem tocadas contam como erro
        while (proxima < total_notas && agora - tempo_notas[proxima] > JANELA_BOM_MS) {
            erros++;
            aviso = 3;
            aviso_ate = agora + 400;
            proxima++;
        }

        // Monta a pista: espacos, as divisorias da zona de acerto e as notas
        for (i = 0; i < LARGURA_PISTA; i++) {
            pista[i] = ' ';
        }
        pista[LARGURA_PISTA] = '\0';
        pista[COLUNA_ALVO - colunas_bom - 1] = '|';
        pista[COLUNA_ALVO - colunas_perfeito - 1] = '|';
        pista[COLUNA_ALVO + colunas_perfeito + 1] = '|';
        pista[COLUNA_ALVO + colunas_bom + 1] = '|';

        for (i = proxima; i < total_notas; i++) {
            coluna = COLUNA_ALVO + (tempo_notas[i] - agora) / MS_POR_COLUNA;
            if (coluna >= LARGURA_PISTA) {
                break;   // esta nota e as seguintes ainda nao entraram na tela
            }
            if (coluna >= 0) {
                pista[coluna] = tipo_notas[i];
            }
        }

        // Olhos do gato: ele pisca do lado da pata que bateu
        olho_esquerdo = 'o';
        olho_direito = 'o';
        if (agora < pata_x_ate) olho_esquerdo = '>';
        if (agora < pata_o_ate) olho_direito = '<';

        // Desenha a tela
        CURSOR_NO_TOPO();
        printf("   Pontuacao: %-8d   Nivel: %s\n", pontuacao_partida, nome_nivel[nivel_escolhido]);
        printf("\n");
        printf("                                                         /\\___/\\\n");
        printf("                                                        ( %c   %c )\n", olho_esquerdo, olho_direito);
        printf("                                                        (  =^=  )\n");

        // patas: levantadas ou batendo no tambor
        printf("                                                     ");
        if (agora < pata_x_ate) printf("   |   "); else printf("  (\")  ");
        if (agora < pata_o_ate) printf("    | \n"); else printf("   (\")\n");

        printf("                                                     ");
        if (agora < pata_x_ate) printf("[=(\")=] "); else printf("[=====] ");
        if (agora < pata_o_ate) printf("[=(\")=]\n"); else printf("[=====]\n");

        printf("                                                      \\___/   \\___/\n");
        printf("\n");
        printf("      perfeito\n");
        printf("%s\n", borda);
        printf("%s\n", pista);
        printf("%s\n", borda);
        printf("    bom       bom\n");
        printf("\n");

        if (agora < aviso_ate && aviso == 1) printf("      PERFEITO!\n");
        else if (agora < aviso_ate && aviso == 2) printf("        BOM!   \n");
        else if (agora < aviso_ate && aviso == 3) printf("       ERROU!  \n");
        else printf("               \n");

        printf("\n");
        printf("                          HORA DO GATO MALUCO !!!!\n");
        printf("   X = pata esquerda     O = pata direita     ESC = sair\n");

        Sleep(10);   // espera 10 ms antes do proximo desenho
    }

    // Para a musica
    PlaySoundA(NULL, NULL, 0);

    // Espera um pouco e joga fora as teclas que ainda foram apertadas
    Sleep(1000);
    while (_kbhit()) {
        _getch();
    }

    system("cls");
    printf("===== FIM DA MUSICA =====\n");
    printf("Perfeitos: %d\n", perfeitos);
    printf("Bons:      %d\n", bons);
    printf("Erros:     %d\n", erros);
}

// Tela de escolha do nivel: mostra cada musica com a dificuldade dela e le a escolha do jogador
void TELA_ESCOLHER_NIVEL(){

    char opcao[100];
    int escolheu = 0, n, i;

    system("cls");
    printf("=================== ESCOLHA O NIVEL ===================\n\n");

    for (n = 0; n < TOTAL_NIVEIS; n++) {
        printf("[%d] %s\n", n + 1, nome_nivel[n]);

        // dificuldade: um * para cada ponto, completando com - ate 5
        printf("    Dificuldade: [");
        for (i = 1; i <= 5; i++) {
            if (i <= dificuldade_nivel[n]) printf("*");
            else printf("-");
        }
        printf("] ");
        if (dificuldade_nivel[n] == 1) printf("Muito facil\n");
        else if (dificuldade_nivel[n] == 2) printf("Facil\n");
        else if (dificuldade_nivel[n] == 3) printf("Medio\n");
        else if (dificuldade_nivel[n] == 4) printf("Dificil\n");
        else printf("Muito dificil\n");

        // duracao em minutos e segundos (%02d escreve o numero com 2 digitos: 5 vira 05)
        printf("    %d BPM   |   %d:%02d min\n\n", bpm_nivel[n], duracao_nivel[n] / 60000, (duracao_nivel[n] / 1000) % 60);
    }

    // Pergunta ate o jogador digitar um numero de nivel que existe
    while (escolheu == 0) {
        printf("Escolha o nivel (1 a %d): ", TOTAL_NIVEIS);
        scanf("%99s", opcao);

        if (opcao[0] >= '1' && opcao[0] <= '0' + TOTAL_NIVEIS && opcao[1] == '\0') {
            nivel_escolhido = opcao[0] - '1';   // o nivel [1] fica na posicao 0 dos vetores
            escolheu = 1;
        } else {
            printf("Opcao invalida\n");
        }
    }
}

// Tela de resultados: mostra o ranking de cada nivel, da maior pontuacao para a menor
void TELA_RESULTADOS(){

    int ja_mostrou[MAX_JOGADORES];
    int n, posicao, melhor, i;

    CARREGAR_RESULTADOS();

    printf("\n================== RESULTADOS ==================\n");

    if (total_jogadores == 0) {
        printf("Nenhum jogador registrado\n");
    } else {
        for (n = 0; n < TOTAL_NIVEIS; n++) {
            printf("\nNivel %d: %s\n", n + 1, nome_nivel[n]);

            // quem ainda nao jogou este nivel (pontuacao -1) fica fora do ranking
            for (i = 0; i < total_jogadores; i++) {
                if (pontuacoes[i][n] == -1) ja_mostrou[i] = 1;
                else ja_mostrou[i] = 0;
            }

            posicao = 1;
            melhor = 0;
            while (melhor != -1) {
                // procura a maior pontuacao entre os jogadores que ainda nao foram mostrados
                melhor = -1;
                for (i = 0; i < total_jogadores; i++) {
                    if (ja_mostrou[i] == 0) {
                        if (melhor == -1 || pontuacoes[i][n] > pontuacoes[melhor][n]) {
                            melhor = i;
                        }
                    }
                }

                if (melhor != -1) {
                    printf("  %2d. %-20s %6d\n", posicao, nicknames[melhor], pontuacoes[melhor][n]);
                    ja_mostrou[melhor] = 1;
                    posicao++;
                }
            }

            if (posicao == 1) {
                printf("  Ninguem jogou este nivel ainda\n");
            }
        }
    }
    printf("\n================================================\n");
}

void TELA_JOGAR(){

    char resposta[100], nick[21];
    int primeira_vez = -1, pronto = 0, achou, igual, i, j, n;
    int jogador = 0;   // posicao do jogador na lista (quando ele ja jogou antes)

    // Le os nicknames salvos no arquivo (se o arquivo ainda nao existe, a lista fica vazia)
    CARREGAR_RESULTADOS();

    // Tela com os nicknames de quem ja jogou
    printf("===== JOGADORES REGISTRADOS =====\n");
    if (total_jogadores == 0) {
        printf("Nenhum jogador registrado\n");
    } else {
        for (i = 0; i < total_jogadores; i++) {
            printf("%s\n", nicknames[i]);
        }
    }
    printf("=================================\n");

    // Pergunta ate o jogador responder S ou N
    while (primeira_vez == -1) {
        printf("Eh a sua primeira vez jogando? (S/N): ");
        scanf("%99s", resposta);

        if ((resposta[0] == 'S' || resposta[0] == 's') && resposta[1] == '\0') primeira_vez = 1;
        else if ((resposta[0] == 'N' || resposta[0] == 'n') && resposta[1] == '\0') primeira_vez = 0;
        else printf("Resposta invalida\n");
    }

    // se ninguem jogou ainda nao existe nickname anterior para escolher
    if (primeira_vez == 0 && total_jogadores == 0) {
        printf("Ainda nao ha gatos registrados\n");
        primeira_vez = 1;
    }

    // se a lista esta cheia nao cabe mais um jogador novo
    if (primeira_vez == 1 && total_jogadores == MAX_JOGADORES) {
        printf("O limite de jogadores foi atingido\n");
        return;
    }

    // Jogador insere o nickname (novo se respondeu S, anterior se respondeu N)
    while (pronto == 0) {
        if (primeira_vez == 1) printf("Crie o nome do seu gato (ate 20 letras, sem espacos): ");
        else printf("Digite o nome do seu gato: ");
        scanf("%20s", nick);

        // procura o nickname digitado na lista
        achou = 0;
        for (i = 0; i < total_jogadores; i++) {
            igual = 1;
            for (j = 0; j < 21; j++) {
                if (nick[j] != nicknames[i][j]) {
                    igual = 0;
                }
                if (nick[j] == '\0') {
                    break;
                }
            }
            if (igual == 1) {
                achou = 1;
                jogador = i;
            }
        }

        if (primeira_vez == 1 && achou == 1) {
            printf("Esse gato ja tem dono. Escolha outro.\n");
        } else if (primeira_vez == 0 && achou == 0) {
            printf("Gato nao encontrado. Tente de novo.\n");
        } else {
            pronto = 1;
        }
    }

    // Tela de escolha do nivel, mostrando a dificuldade de cada musica
    TELA_ESCOLHER_NIVEL();

    printf("\nBoa sorte, %s!\n", nick);

    // O jogo comeca
    JOGO();

    // Ao final, o programa atrela a pontuacao ao nickname e registra nos resultados do nivel jogado
    printf("\n%s, voce fez %d pontos no nivel %s!\n", nick, pontuacao_partida, nome_nivel[nivel_escolhido]);

    if (primeira_vez == 1) {
        // jogador novo: guarda o nickname no final da lista
        for (j = 0; j < 21; j++) {
            nicknames[total_jogadores][j] = nick[j];
            if (nick[j] == '\0') {
                break;
            }
        }
        // ele ainda nao jogou os outros niveis
        for (n = 0; n < TOTAL_NIVEIS; n++) {
            pontuacoes[total_jogadores][n] = -1;
        }
        pontuacoes[total_jogadores][nivel_escolhido] = pontuacao_partida;
        total_jogadores++;
        SALVAR_RESULTADOS();
    } else if (pontuacoes[jogador][nivel_escolhido] == -1) {
        // jogador antigo jogando este nivel pela primeira vez
        pontuacoes[jogador][nivel_escolhido] = pontuacao_partida;
        SALVAR_RESULTADOS();
    } else if (pontuacao_partida > pontuacoes[jogador][nivel_escolhido]) {
        // jogador antigo: so registra se a pontuacao nova for melhor que a antiga neste nivel
        printf("Novo recorde! A sua pontuacao antiga neste nivel era %d.\n", pontuacoes[jogador][nivel_escolhido]);
        pontuacoes[jogador][nivel_escolhido] = pontuacao_partida;
        SALVAR_RESULTADOS();
    } else {
        printf("A sua melhor pontuacao neste nivel continua sendo %d.\n", pontuacoes[jogador][nivel_escolhido]);
    }

    // Tela final mostrando a pontuacao e os resultados de todos os jogadores
    TELA_RESULTADOS();
}
void TELA_COMO_JOGAR(){

 	printf("======================================== COMO JOGAR ========================================\n\n");
 	printf("O jogo Hora do Gato Maluco consiste em um jogo de ritmo, sendo seus principais elementos do jogo: \n\n");
	printf("- Notas;\n");
	printf("- Janela de acerto;\n");
	printf("- Janela de acerto perfeito;\n");
	printf("- Comandos do usuario.\n\n");
	printf("As notas aparecem em uma linha, vao da direita para a esquerda e passam pelas janelas de acerto.\n");
	printf("A janela de acerto gera a pontuacao basica do usuario e eh acionada quando uma nota\nentra em seu espaco e o usuario aperta o respectivo comando. ");
	printf("\nA janela de acerto perfeito funciona quase da mesma maneira que a basica, porem sua janela\nde acerto eh menor gera uma pontuacao maior.\n");
	printf("Os comandos para acertar as notas sao, respectivamente, o X para acionar a pata esquerda do\nseu gato maluco e o O para acionar a pata direita.\n\n");
	printf("Pontuacao:\n");
	printf("- Acerto perfeito (nota na zona marcada com #####): %d pontos;\n", PONTOS_PERFEITO);
	printf("- Acerto bom (nota na zona marcada com ====): %d pontos;\n", PONTOS_BOM);
	printf("- Nota fora da zona, tecla trocada ou nota que passou sem ser tocada: 0 pontos.\n");
	printf("A tecla ESC encerra a musica antes do fim.\n\n");
	printf("Niveis:\n");
	printf("Depois de digitar o nome do seu gato, voce escolhe a musica. Cada nivel mostra a sua dificuldade,\n");
	printf("de [*----] (muito facil) a [*****] (muito dificil): quanto mais dificil, mais notas por segundo.\n");
	printf("O recorde de cada jogador eh guardado separado para cada nivel.\n");

}
void TELA_SOBRE_O_JOGO(){

	printf("========================================== SOBRE ===========================================\n\n");
	printf("O projeto consiste em um jogo de ritmo desenvolvido em linguagem C e executado em terminal.\n");
	printf("O jogo apresenta elementos em ASCII art que se deslocaram da direita para a esquerda, de acordo\n");
	printf("com o ritmo. O jogador devera pressionar uma determinada tecla no momento correto, de acordo com\n");
	printf("a posicao dos elementos na tela. Como elemento visual, sera utilizado um gato em ASCII Art,\n");
	printf("realizando movimentos de batida em um tambor, acompanhando o ritmo. O projeto tambem busca explorar\n");
	printf("recursos sonoros no terminal, conforme as possibilidades tecnicas da linguagem C e do ambiente de\nexecucao.\n");

}


int main (){

    int menu;
    char opcao[100];
    int opcao_invalida = 0;

    // O menu aparece de novo ate o jogador escolher sair
    while (continuar == 1) {

        system("cls");

 //exibe na tela a imagem do gatinho batendo no tambor
 // (puts escreve o texto exatamente como esta, sem tratar o % como codigo do printf)

        printf("\t\t\t\t\tHora do Gato Maluco\n\n\n");
        puts("                         *:*                        \t\t\t[1] Jogar");
        puts("                      =*:   :=                      \t\t\t[2] Como Jogar");
        puts("                  :#+.        .=#=                  \t\t\t[3] Sobre o jogo");
        puts("                #                   %@@       =@@.  \t\t\t[4] Resultados");
        puts("        =-  @ @                          @@@*:::#.  \t\t\t[5] Sair");
        puts("      .#   . #:                              ..:#.  ");
        puts("      .# ..   :     :@-                       .:#.  ");
        puts("      .#                   |           ###    .+-   ");
        puts("      .@###               =|=   -@:  #..  -=  -=    ");
        puts("               @@@*                 %.     -+  +-   ");
        puts("   .@@#--------@@#    *@@@          %  ..     .:#.  ");
        puts(":#                 :+.       .###+::%        .:::%  ");
        puts("@                .  -%##---------##: ::::###+:::::@ ");
        puts("@-@#          :: -@*..             ..@          *@@ ");
        puts("@%----=%%%%%%%%::+*                   @             ");
        puts("@::-@@%------::@@@@*                 @@             ");
        puts("@::::::::::::::::+#--*@@        -@@+--@             ");
        puts(" @:::::::::::::::+#-#*==-------::==*%=@             ");
        puts(" :#::::::::::::-=+#:::::========-:::==@             ");
        puts("  @=%%#::-=====@@#+#::::::::::::::::=@              ");
        puts("  .%----------::-=@#:::::::::::::::-=@              ");
        puts("      .%%%%%%%%    .@*:::::::::====#@               ");
        puts("                   .#---@@@@@@@@*::-@               ");
        if (opcao_invalida == 1) puts("                     -##=======-=##:                \t\t\tOpcao invalida");
        else puts("                     -##=======-=##:                ");
        puts("                        .%%%%%%%%                   \t\t\tEscolha uma opcao:");

        // Le a opcao como texto: assim uma letra digitada por engano nao trava o programa
        scanf("%99s", opcao);

        menu = 0;   // 0 = opcao invalida
        if (opcao[0] >= '1' && opcao[0] <= '5' && opcao[1] == '\0') {
            menu = opcao[0] - '0';   // transforma o caractere '1' no numero 1, '2' em 2...
        }

        opcao_invalida = 0;
        system("cls");

        switch (menu) {

            case 1:
                TELA_JOGAR();
            break;

            case 2:
                TELA_COMO_JOGAR();
            break;

            case 3:
                TELA_SOBRE_O_JOGO();
            break;

            case 4:
                TELA_RESULTADOS();
            break;

            case 5:
                TELA_SAIR();
            break;

            default:
                opcao_invalida = 1;   // a mensagem aparece junto com o menu
            break;
        }

        // Depois das telas 1 a 4 aparece a opcao "Voltar ao menu?"
        if (menu >= 1 && menu <= 4) {
            VOLTAR_AO_MENU();
        }
    }

    return 0;
}
