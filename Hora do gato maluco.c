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
#define TOTAL_COMPASSOS     62      // quantos compassos tem o nivel
#define MAX_NOTAS           500
#define MS_POR_BATIDA       625     // a musica tem 96 batidas por minuto: 60000 / 96 = 625 ms
#define PRIMEIRA_BATIDA_MS  440     // momento (ms) da primeira batida dentro do arquivo da musica
#define DURACAO_MUSICA_MS   155400  // duracao da musica (2 min 35 s)
#define AJUSTE_AUDIO_MS     50      // atraso do som: aumente se as notas chegarem antes da batida, diminua se chegarem depois
#define ARQUIVO_MUSICA      "musica.wav"

// Nicknames e pontuacoes de quem ja jogou.
// Eles ficam guardados no arquivo "resultados.txt" (uma linha por jogador: nickname pontuacao).
char nicknames[MAX_JOGADORES][21];
int pontuacoes[MAX_JOGADORES];
int total_jogadores = 0;

int pontuacao_partida = 0;   // pontuacao da ultima partida jogada
int continuar = 1;           // 1 = o programa continua rodando, 0 = fecha o jogo

// O nivel: cada linha eh um compasso da musica (4 batidas) dividido em 8 partes iguais.
// 'X' = nota da pata esquerda, 'O' = nota da pata direita, '.' = sem nota.
// Para mudar o nivel basta trocar as letras.
char nivel[TOTAL_COMPASSOS][9] = {
    "........",   //  0  
    "....O.X.",   //  1 
    "O...X...",   //  2 
    "O...X...",   //  3
    "O...X.O.",   //  4
    "O.X.O...",   //  5
    "X....X..",   //  6  
    "...X..X.",   //  7
    "..X..X..",   //  8
    "....O..O",   //  9  
    "O...X...",   // 10
    "O...X...",   // 11
    "O.X.O.X.",   // 12
    "O...X.XX",   // 13 
    "O.X.O.X.",   // 14  
    "O.X.O.X.",   // 15
    "O.X.O.X.",   // 16
    "O.X.O.X.",   // 17
    "O.X.O.X.",   // 18
    "O.X.O.X.",   // 19
    "O.X.O.X.",   // 20
    "O.X.O.X.",   // 21
    "O.XOO.X.",   // 22  
    "O.X.O.XO",   // 23
    "O.XOO.X.",   // 24
    "O.X.O.XO",   // 25
    "O.XOO.X.",   // 26
    "O.X.O.XO",   // 27
    "O.XOO.X.",   // 28
    "O.X.O.XO",   // 29
    "O.XOO.XO",   // 30  
    "O.XOO.XO",   // 31
    "O.XOO.XO",   // 32
    "O.XOO.XO",   // 33
    "O.XOO.XO",   // 34
    "O.XOO.XO",   // 35
    "O.XOO.XO",   // 36
    "O.X.O...",   // 37  
    "...X.X..",   // 38  
    "...X..X.",   // 39
    "..X.....",   // 40
    "....O..O",   // 41  
    "O...X...",   // 42
    "O...X...",   // 43
    "O.X.O.X.",   // 44
    "O...X.XX",   // 45  
    "O.XOO.X.",   // 46  
    "O.X.O.XO",   // 47
    "O.XOO.X.",   // 48
    "O.X.O.XO",   // 49
    "O.XOO.XO",   // 50  
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
    "O......."    // 61  
};

// Le os nicknames e as pontuacoes salvos no arquivo (se o arquivo ainda nao existe, a lista fica vazia)
void CARREGAR_RESULTADOS(){

    FILE *arquivo;

    total_jogadores = 0;
    arquivo = fopen("resultados.txt", "r");
    if (arquivo != NULL) {
        while (total_jogadores < MAX_JOGADORES && fscanf(arquivo, "%20s %d", nicknames[total_jogadores], &pontuacoes[total_jogadores]) == 2) {
            total_jogadores++;
        }
        fclose(arquivo);
    }
}

// Escreve todos os nicknames e pontuacoes no arquivo
void SALVAR_RESULTADOS(){

    FILE *arquivo;
    int i;

    arquivo = fopen("resultados.txt", "w");
    if (arquivo != NULL) {
        for (i = 0; i < total_jogadores; i++) {
            fprintf(arquivo, "%s %d\n", nicknames[i], pontuacoes[i]);
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
    char tecla, olho_esquerdo, olho_direito;
    int inicio, agora = 0, diferenca, distancia, coluna;
    int pata_x_ate = 0, pata_o_ate = 0;   // ate que momento (ms) cada pata fica abaixada
    int aviso = 0, aviso_ate = 0;         // aviso na tela: 1 = perfeito, 2 = bom, 3 = errou
    int sair = 0, c, s, i;

    pontuacao_partida = 0;

    // Transforma o desenho do nivel em uma lista de notas com o tempo de cada uma.
    // Cada parte do compasso dura meia batida.
    for (c = 0; c < TOTAL_COMPASSOS; c++) {
        for (s = 0; s < 8; s++) {
            if (nivel[c][s] == 'X' || nivel[c][s] == 'O') {
                tempo_notas[total_notas] = PRIMEIRA_BATIDA_MS + AJUSTE_AUDIO_MS + (c * 8 + s) * MS_POR_BATIDA / 2;
                tipo_notas[total_notas] = nivel[c][s];
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
    arquivo = fopen(ARQUIVO_MUSICA, "r");
    if (arquivo == NULL) {
        printf("Aviso: o arquivo %s nao foi encontrado nesta pasta.\n", ARQUIVO_MUSICA);
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
    PlaySoundA(ARQUIVO_MUSICA, NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
    inicio = clock();

    while (sair == 0 && agora < DURACAO_MUSICA_MS) {

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
        printf("   Pontuacao: %d\n", pontuacao_partida);
        printf("\n");
        printf("                                                         /\\___/\\\n");
        printf("                                                        | %c   %c |\n", olho_esquerdo, olho_direito);
        printf("                                                        |  =^=  |\n");

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

// Tela final: mostra os resultados de todos os jogadores, da maior pontuacao para a menor
void TELA_RESULTADOS(){

    int ja_mostrou[MAX_JOGADORES];
    int posicao, melhor, i;

    CARREGAR_RESULTADOS();

    printf("\n=========== RESULTADOS ===========\n");
    if (total_jogadores == 0) {
        printf("Nenhum jogador registrado\n");
    }

    for (i = 0; i < total_jogadores; i++) {
        ja_mostrou[i] = 0;
    }

    for (posicao = 1; posicao <= total_jogadores; posicao++) {
        // procura a maior pontuacao entre os jogadores que ainda nao foram mostrados
        melhor = -1;
        for (i = 0; i < total_jogadores; i++) {
            if (ja_mostrou[i] == 0) {
                if (melhor == -1 || pontuacoes[i] > pontuacoes[melhor]) {
                    melhor = i;
                }
            }
        }
        printf("%2d. %-20s %6d\n", posicao, nicknames[melhor], pontuacoes[melhor]);
        ja_mostrou[melhor] = 1;
    }
    printf("==================================\n");
}

void TELA_JOGAR(){

    char resposta[100], nick[21];
    int primeira_vez = -1, pronto = 0, achou, igual, i, j;
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
        printf("Ainda nao ha nicknames registrados\n");
        primeira_vez = 1;
    }

    // se a lista esta cheia nao cabe mais um jogador novo
    if (primeira_vez == 1 && total_jogadores == MAX_JOGADORES) {
        printf("O limite de jogadores foi atingido\n");
        return;
    }

    // Jogador insere o nickname (novo se respondeu S, anterior se respondeu N)
    while (pronto == 0) {
        if (primeira_vez == 1) printf("Digite um nickname (ate 20 letras, sem espacos): ");
        else printf("Digite o seu nickname anterior: ");
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
            printf("Esse nickname ja existe. Escolha outro.\n");
        } else if (primeira_vez == 0 && achou == 0) {
            printf("Nickname nao encontrado. Tente de novo.\n");
        } else {
            pronto = 1;
        }
    }

    printf("Boa sorte, %s!\n", nick);

    // O jogo comeca
    JOGO();

    // Ao final, o programa atrela a pontuacao ao nickname e registra nos resultados
    printf("\n%s, voce fez %d pontos!\n", nick, pontuacao_partida);

    if (primeira_vez == 1) {
        // jogador novo: guarda o nickname e a pontuacao no final da lista
        for (j = 0; j < 21; j++) {
            nicknames[total_jogadores][j] = nick[j];
            if (nick[j] == '\0') {
                break;
            }
        }
        pontuacoes[total_jogadores] = pontuacao_partida;
        total_jogadores++;
        SALVAR_RESULTADOS();
    } else if (pontuacao_partida > pontuacoes[jogador]) {
        // jogador antigo: so registra se a pontuacao nova for melhor que a antiga
        printf("Novo recorde! A sua pontuacao antiga era %d.\n", pontuacoes[jogador]);
        pontuacoes[jogador] = pontuacao_partida;
        SALVAR_RESULTADOS();
    } else {
        printf("A sua melhor pontuacao continua sendo %d.\n", pontuacoes[jogador]);
    }

    // Tela final mostrando a pontuacao e os resultados de todos os jogadores
    TELA_RESULTADOS();
}
void TELA_COMO_JOGAR(){

 	printf("======================================== COMO JOGAR ========================================\n");
 	printf("O jogo gato maluco consiste em um jogo de ritmo, os seus principais elementos do jogo sao: \n\n");
	printf("Notas;\n");
	printf("Janela de acerto;\n");
	printf("Janela de acerto perfeito;\n");
	printf("Comandos do usuario.\n\n");
	printf("As notas aparecem em uma linha e vao da direita a esquerda e passam pelas janelas de acerto.\n");
	printf("A janela de acerto gera a pontuacao basica do usuario e eh acionada quando uma nota\nentra em seu espaco e o usuario aperta o respectivo comando. ");
	printf("\nA janela de acerto perfeito funciona quase da mesma maneira que a basica, porem sua janela\nde acerto eh menor e pontuacao maior quando ha o acerto.\n");
	printf("Os comandos para acertar as notas sao respectivamente o X para acionar a pata esquerda do\ngato maluco e o O para acionar a pata direita.\n\n");
	printf("Pontuacao:\n");
	printf("Acerto perfeito (nota na zona marcada com #####): %d pontos;\n", PONTOS_PERFEITO);
	printf("Acerto bom (nota na zona marcada com ====): %d pontos;\n", PONTOS_BOM);
	printf("Nota fora da zona, tecla trocada ou nota que passou sem ser tocada: 0 pontos.\n");
	printf("A tecla ESC encerra a musica antes do fim.\n");

}
void TELA_SOBRE_O_JOGO(){

	printf("============================================================== SOBRE =============================================================\n");
	printf("O projeto consiste em um jogo de ritmo desenvolvido em linguagem C e executado em terminal. ");
	printf("O jogo apresentara elementos em ASCII art que se deslocaram da direita para a esquerda, de acordo com o ritmo. ");
	printf("O jogador devera pressionar uma determinada tecla no momento correto, de acordo com a posicao dos elementos na tela. ");
	printf("Como elemento visual, sera utilizado um gato em ASCII Art, realizando movimentos de batida em um tambor, acompanhando o ritmo. ");
	printf("O projeto tambem buscara explorar recursos sonoros no terminal, conforme as possibilidades tecnicas da linguagem C e do ambiente de execucao.\n");

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

        printf("\t\tHora do Gato Maluco\n");
        puts("...........................................................");
        puts("...........................................................");
        puts("...........................=*:.............................");
        puts(".........................-%:.%=............................");
        puts(".....................:+*+-....=*+-.........................");
        puts("..................-#*:............-#%+:....................");
        puts("..........--:..:+*:...................:=**=................");
        puts("........+=..:*#+..........................:=#+:+#*+*.......");
        puts(".......-+...:.--..................................:*.......");
        puts(".......-=..:........:#@:..........................=+.......");
        puts("*%#+-:.-+...............-#%%-..........:+*+:......*:.......");
        puts(".....:-=+=:.................:-.....=*:*=...-+....=*........");
        puts("....::.......::::..................=+-*......=...:#:.......");
        puts("....+=:......::::......:+**+=:.......-*..:........:*:......");
        puts("...:*##**+=::.........:==....::::::==-*............-%......");
        puts("...-:::-+*#%%%%@%%%%%@%*-:::::-===-::::-***+=-...:..++.....");
        puts("......::::---=*%%%%**++#+-:::::::::::::::-....:=*+*+-=::...");
        puts("......::::----=+=*+***#####**+==--::::::-+=..........:==++*");
        puts(".....:::::-----==++++=+=--=+#%%@@%%%%###*+:................");
        puts("....==::::----==++++=----:::--==*%%%%+-:...................");
        puts("....++*=------==++++=----:::---===+=-:::...................");
        puts("....:..:=*##%%##%%%#-::--:=*+--==-::::::...................");
        puts("..............=+......:==--++====-::::-....................");
        puts("......................*%%*----===-:::-*:...................");
        puts("......................=-.-*###**#=++++-....................");
        puts("..............................::**:........................");
        puts("................................=-.........................");

//exibe as opcoes do menu inicial

        printf("\n\t\tTela Inicial:");
        printf("\n[1] Jogar");
        printf("\n[2] Como Jogar");
        printf("\n[3] Sobre o jogo");
        printf("\n[4] Resultados");
        printf("\n[5] Sair");
        printf("\n\n");

        if (opcao_invalida == 1) {
            printf("Opcao invalida\n");
        }
        printf("Escolha uma opcao: ");

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
                opcao_invalida = 1;  
            break;
        }

        // Depois das telas 1 a 4 aparece a opcao "Voltar ao menu?"
        if (menu >= 1 && menu <= 4) {
            VOLTAR_AO_MENU();
        }
    }

    return 0;
}
