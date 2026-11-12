#include <stdio.h>
#include <stdlib.h>
#define MAX_TITULO          50
#define MAX_JOGADORES       100
#define LARGURA_TELA        80   // colunas do terminal
#define ALTURA_TELA         24   // linhas do terminal
#define COLUNA_ALVO         10   // coluna da zona de acerto
#define JANELA_PERFEITO_MS  50   // tolerancia (ms) para acerto perfeito
#define JANELA_BOM_MS       120  // tolerancia (ms) para acerto bom; acima disso eh erro
#define PONTOS_PERFEITO     100  // pontos por acerto perfeito
#define PONTOS_BOM          50   // pontos por acerto bom 
 
// Nicknames de quem ja jogou.
// Eles ficam guardados no arquivo "nicknames.txt".
char nicknames[100][21];
int total_jogadores = 0;
 
void TELA_JOGAR(){
 
    FILE *arquivo;
    char resposta[2], nick[21];
    int primeira_vez = -1, pronto = 0, achou = 0, igual, i, j;
 
    // Le os nicknames salvos no arquivo (se o arquivo ainda nao existe, a lista fica vazia)
    arquivo = fopen("nicknames.txt", "r");
    if (arquivo != NULL) {
        while (total_jogadores < 100 && fscanf(arquivo, "%20s", nicknames[total_jogadores]) == 1) {
            total_jogadores++;
        }
        fclose(arquivo);
    }
 
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
        scanf("%s", resposta);
 
        if ((resposta[0] == 'S' || resposta[0] == 's') && resposta[1] == '\0') primeira_vez = 1;
        else if ((resposta[0] == 'N' || resposta[0] == 'n') && resposta[1] == '\0') primeira_vez = 0;
        else printf("Resposta invalida\n");
    }
 
    // se ninguem jogou ainda nao existe nickname anterior para escolher
    if (primeira_vez == 0 && total_jogadores == 0) {
        printf("Ainda nao ha nicknames registrados\n");
        primeira_vez = 1;
    }
 
    // Jogador insere o nickname (novo se respondeu S, anterior se respondeu N)
    while (pronto == 0) {
        if (primeira_vez == 1) printf("Digite um nickname (ate 20 letras, sem espacos): ");
        else printf("Digite o seu nickname anterior: ");
        scanf("%20s", nick);
 
        // procura o nickname digitado na lista
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
 
    // se for jogador novo salva o nickname no final do arquivo
    if (primeira_vez == 1) {
        arquivo = fopen("nicknames.txt", "a");
        if (arquivo != NULL) {
            fprintf(arquivo, "%s\n", nick);
            fclose(arquivo);
        }
    }
 
    printf("Boa sorte, %s!\n", nick);
 

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
	printf("Os comandos para acertar as notas sao respectivamente o X para acionar a pata esquerda do\ngato maluco e o O para acionar a pata direita.");
 
}
void TELA_SOBRE_O_JOGO(){

	printf("============================================================== SOBRE =============================================================\n");
	printf("O projeto consiste em um jogo de ritmo desenvolvido em linguagem C e executado em terminal. ");
	printf("O jogo apresentara elementos em ASCII art que se deslocaram da direita para a esquerda, de acordo com o ritmo. ");
	printf("O jogador devera pressionar uma determinada tecla no momento correto, de acordo com a posicao dos elementos na tela. ");
	printf("Como elemento visual, sera utilizado um gato em ASCII Art, realizando movimentos de batida em um tambor, acompanhando o ritmo. ");
	printf("O projeto tambem buscara explorar recursos sonoros no terminal, conforme as possibilidades tecnicas da linguagem C e do ambiente de execucao.");
 
}
void TELA_RESULTADOS(){
 
}
void TELA_SAIR(){
 
}
 
 
int main (){
 
    int menu;
 
 //Opcoes de menu
 
 
 //exibe na tela a imagem do gatinho batendo no tambor
 
 
    printf("\t\tHora do Gato Maluco");
    printf("\n...........................................................");
    printf("\n...........................................................");
	printf("\n...........................=*:.............................");
    printf("\n.........................-%:.%=............................");
    printf("\n.....................:+*+-....=*+-.........................");
    printf("\n..................-#*:............-#%+:....................");
	printf("\n..........--:..:+*:...................:=**=................");
    printf("\n........+=..:*#+..........................:=#+:+#*+*.......");
    printf("\n.......-+...:.--..................................:*.......");
    printf("\n.......-=..:........:#@:..........................=+.......");
	printf("\n*%#+-:.-+...............-#%%-..........:+*+:......*:.......");
    printf("\n.....:-=+=:.................:-.....=*:*=...-+....=*........");
    printf("\n....::.......::::..................=+-*......=...:#:.......");
    printf("\n....+=:......::::......:+**+=:.......-*..:........:*:......");
	printf("\n...:*##**+=::.........:==....::::::==-*............-%......");
    printf("\n...-:::-+*#%%%%@%%%%%@%*-:::::-===-::::-***+=-...:..++.....");
    printf("\n......::::---=*%%%%**++#+-:::::::::::::::-....:=*+*+-=::...");
    printf("\n......::::----=+=*+***#####**+==--::::::-+=..........:==++*");
	printf("\n.....:::::-----==++++=+=--=+#%%@@%%%%###*+:................");
    printf("\n....==::::----==++++=----:::--==*%%%%+-:...................");
    printf("\n....++*=------==++++=----:::---===+=-:::...................");
    printf("\n....:..:=*##%%##%%%#-::--:=*+--==-::::::...................");
	printf("\n..............=+......:==--++====-::::-....................");
    printf("\n......................*%%*----===-:::-*:...................");
    printf("\n......................=-.-*###**#=++++-....................");
    printf("\n..............................::**:........................");
	printf("\n................................=-.........................");
	
//exibe as opcoes do menu inicial


    printf("\n\n\t\tTela Inicial:");
    printf("\n[1] Jogar");
    printf("\n[2] Como Jogar");
	printf("\n[3] Sobre o jogo");
    printf("\n[4] Resultados");
    printf("\n[5] Sair");
	printf("\n\n");

    scanf("%d", &menu);
 
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
    }
 
    return 0;
}
 
