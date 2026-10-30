#include <stdio.h>
#include <stdlib.h>

#include <stdio.h>
#include <stdlib.h>
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
 
// Nicknames de quem ja jogou.
// Eles ficam guardados no arquivo "nicknames.txt".
char nicknames[100][21];
int total_jogadores = 0;
 
void TELA_JOGAR(){
 
    FILE *arquivo;
    char resposta[100];
    char nick[21];
    int primeira_vez;
    int pronto;
    int achou;
    int igual;
    int i;
    int j;
 
    // Le os nicknames salvos no arquivo (se o arquivo ainda nao existe, a lista fica vazia)
    total_jogadores = 0;
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
        printf("Nenhum jogador registrado ainda.\n");
    } else {
        for (i = 0; i < total_jogadores; i++) {
            printf("%s\n", nicknames[i]);
        }
    }
    printf("=================================\n");
 
    // Pergunta ate o jogador responder S ou N
    primeira_vez = -1;
    while (primeira_vez == -1) {
        printf("E a sua primeira vez jogando? (S/N): ");
        scanf("%s", resposta);
 
        if ((resposta[0] == 'S' || resposta[0] == 's') && resposta[1] == '\0') {
            primeira_vez = 1;
        } else if ((resposta[0] == 'N' || resposta[0] == 'n') && resposta[1] == '\0') {
            primeira_vez = 0;
        } else {
            printf("Resposta invalida. Digite S ou N.\n");
        }
    }
 
    // se ninguem jogou ainda nao existe nickname anterior para escolher
    if (primeira_vez == 0 && total_jogadores == 0) {
        printf("Ainda nao ha nicknames registrados. Vamos criar o seu!\n");
        primeira_vez = 1;
    }
 
    // Jogador insere o nickname (novo se respondeu S, anterior se respondeu N)
    pronto = 0;
    while (pronto == 0) {
        if (primeira_vez == 1) {
            printf("Digite um nickname (ate 20 letras, sem espacos): ");
        } else {
            printf("Digite o seu nickname anterior: ");
        }
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
 
}
void TELA_SOBRE_O_JOGO(){
 
}
void TELA_RESULTADOS(){
 
}
void TELA_SAIR(){
 
}
 
 
int main (){
 
    int menu;
 
 //Opções de menu
 
    printf("Escolha uma opcao (1 a 5): ");
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
 
