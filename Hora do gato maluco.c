#include <stdio.h>
#include <stdlib.h>

void TELA_JOGAR(){

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

}