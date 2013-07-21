/* sw2.c
 */
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <termios.h>
#include <malloc.h>
#include <curses.h>
#define BUFSIZE 80
#define false 0
#define true 1

int filtro(char *cadena, int *tipo);
void setTime(int valor, int *tipo, struct tm *tiempo);

int main(int argc, char *argv[]) {
    struct tm *fh;
    time_t segundos;
    time_t *inicio;
    int valor, *tipo;
    int c = 0;
    double a = 0;
    char read;
    char buffer[BUFSIZE];
    tipo = (int *)malloc(sizeof(int));
    inicio = (time_t *)malloc(sizeof(time_t));

    if (argc != 2) {
        printf("Ingrese el tiempo: ");
        fgets(buffer, 80, stdin);
        if (buffer[strlen(buffer) - 1] == '\n')
                buffer[strlen(buffer) - 1] = 0;
        valor = filtro(buffer, tipo);
    }
    else {
        printf("Tiempo leído: %s\n", argv[1]);
        valor = filtro(argv[1], tipo);
    }

    // Código para curses
    initscr();
    noecho();
    cbreak();         // don't interrupt for user input el inverso rompe la terminal
    timeout(500);     // wait 500ms for key press
    // Termina código para curses
    *inicio = time(NULL);
    fh = localtime(inicio);
    setTime(valor, tipo, fh);
    segundos = mktime(fh);
    int s = 0;
    mvprintw(0,0,"Tiempo inicial: %s", ctime(inicio));
    while ((a = difftime(segundos, *inicio)) > 0) {
        mvprintw(1,0,"Tiempo final:   %s", ctime(&segundos));
        *inicio = time(NULL);
        int c = getch();
        switch (c) {
            case 's':
                endwin();
                nocbreak();
                return 0;
            case ' ':
                if (s == 1) {
                    mvprintw(3,0,"       ");
                    s = 0;
                }
                else {
                    s = 1;
                    mvprintw(3,0,"Pausado");
                }
                break;
            default:
                break;
        }
        if (s) {
            segundos = *inicio + (int) a;
        }
        mvprintw(2,0,"Segundos restantes: %d\n", (int)a);
    }
    endwin();
    return 0;
}

int filtro(char *cadena, int *tipo) {
    int i = 0, num = 0;
    if (strlen(cadena) != 3)
        return 1;
    int segundo = false, minuto = false,
        hora = false;
    for (i = 0; i < 3; i++) {
        if (cadena[2] == 's') segundo = true;
        if (cadena[2] == 'm') minuto  = true;
        if (cadena[2] == 'h') hora    = true;
    }
    cadena[2] = 0;
    num = atoi(cadena);
    if (segundo)
        *tipo = 1;
    if (minuto)
        *tipo = 2;
    if (hora)
        *tipo = 3;
    return num;
}

void setTime(int valor, int *tipo, struct tm *tiempo) {
    time_t saludo;
    if (*tipo == 1)
        tiempo->tm_sec += valor;
    if (*tipo == 2)
        tiempo->tm_min += valor;
    if (*tipo == 3)
        tiempo->tm_hour += valor;
    saludo = mktime(tiempo);
}
