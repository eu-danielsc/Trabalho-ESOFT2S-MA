#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define LINHAS 6

int main(void) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    CONSOLE_CURSOR_INFO cursor;
    COORD c;

    /* Todas as linhas precisam ter o MESMO tamanho (aqui, 40 caracteres) */
    const char *mapa[LINHAS] = {
        "===============================================",
        "               |O|   |X|  |X|               |O|",
        "          |O|               |O|  |X|   |X|     ",
        "     |X|     |X|    |O|  |O|                   ",
        " |X|     |O|     |X|        |O|                ",
        "==============================================="
    };

    char frame[LINHAS * 512];
    int mapaLarg, largura;
    int offset = 0;
    int i, r, x, p;

    /* se quiser, chame aqui a janelaPixels(hOut, 1280, 720); */

    system("cls");

    mapaLarg = strlen(mapa[0]);

    GetConsoleScreenBufferInfo(hOut, &info);
    largura = info.srWindow.Right - info.srWindow.Left;   /* um a menos, evita quebra de linha */
    if (largura > 500) largura = 500;

    GetConsoleCursorInfo(hOut, &cursor);
    cursor.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &cursor);

    for (i = 0; i < 2000; i++) {
        p = 0;
        for (r = 0; r < LINHAS; r++) {
            for (x = 0; x < largura; x++) {
                frame[p++] = mapa[r][(x + offset) % mapaLarg];
            }
            if (r < LINHAS - 1) frame[p++] = '\n';
        }
        frame[p] = '\0';

        c.X = 0;
        c.Y = 0;
        SetConsoleCursorPosition(hOut, c);
        printf("%s", frame);

        offset = (offset + 1) % mapaLarg;   /* +1 = mapa anda para a esquerda */

        Sleep(40);
    }

    cursor.bVisible = TRUE;
    SetConsoleCursorInfo(hOut, &cursor);
    printf("\n");
    return 0;
}
