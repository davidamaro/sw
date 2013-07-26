/* Simple stopwatch written in C
 * sw.c
 */
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <malloc.h>
#include <curses.h>
#define BUFSIZE 80
#define false 0
#define true 1

void eleccion(int *menu, int *c, int *tipo, time_t *actual);
int filtro(char *cadena, int *tipo);
void setTime(int *valor, int *tipo, struct tm *tiempo);
void getValue(char *buffer);

int main(int argc, char *argv[]) {
    struct tm *tiempo;
    time_t final;
    time_t *actual;
    int valor = 20, *tipo = NULL;
    double diferencia = 0;
    char buffer[BUFSIZE];

    tipo = (int *)malloc(sizeof(int));
    actual = (time_t *)malloc(sizeof(time_t));
    int *s = NULL;
    s = (int *)malloc(sizeof(int));
    *s = 0;

    if (argc != 3) {
        if (argc != 2) {
            //printf("Ingrese el tiempo: ");
            //getValue(buffer);
            //valor = filtro(buffer, tipo);
            //printf("Ingrese la tarea:  ");
            //getValue(buffer);
            strcpy(buffer, "Tarea no definida.");
            valor = 20;
            *tipo = 2;
        }
        else {
//            valor = filtro(argv[1], tipo);
//            printf("Ingrese la tarea:  ");
//            getValue(buffer);
            strcpy(buffer, "Tarea no definida.");
        }
    }
    else {
        valor = filtro(argv[1], tipo);
        strcpy(buffer, argv[2]);
    }

    initscr();
    noecho();
    cbreak();         // don't interrupt for user input el inverso rompe la terminal
    timeout(500);     // wait 500ms for key press
    *actual = time(NULL);
    tiempo = localtime(actual);
    setTime(&valor, tipo, tiempo);
    final = mktime(tiempo);
    mvprintw(0,0,"Tiempo inicial: %s", ctime(actual));
    mvprintw(5,0,"Tarea: %s", buffer);
    while ((diferencia = difftime(final, *actual)) > 0) {
        mvprintw(1,0,"Tiempo final:   %s", ctime(&final));
        *actual = time(NULL);
        int c = getch();
        eleccion(s, &c, tipo, actual);
        if (*s) {
            final = *actual + (int) diferencia;
        }
        mvprintw(2,0,"Segundos restantes: %d\n", (int)diferencia);
    }
    //system("zenity --info --text='El tiempo se acabó'");
    endwin();
    free(s);
    free(tipo);
    free(actual);
    return 0;
}

int filtro(char *cadena, int *tipo) {
    int num = 0;
    if (strlen(cadena) != 3)
        return 1;
    int segundo = false, minuto = false,
        hora = false;
    if (cadena[2] == 's') segundo = true;
    if (cadena[2] == 'm') minuto  = true;
    if (cadena[2] == 'h') hora    = true;
    cadena[2] = 0;
    num = atoi(cadena);
    if (segundo) *tipo = 1;
    if (minuto)  *tipo = 2;
    if (hora)    *tipo = 3;
    return num;
}

void setTime(int *valor, int *tipo, struct tm *tiempo) {
    if (*tipo == 1) tiempo->tm_sec  += *valor;
    if (*tipo == 2) tiempo->tm_min  += *valor;
    if (*tipo == 3) tiempo->tm_hour += *valor;
}

void getValue(char *buffer) {
    fgets(buffer, BUFSIZE, stdin);
    if (buffer[strlen(buffer) - 1] == '\n')
            buffer[strlen(buffer) - 1] = 0;
}

void eleccion(int *menu, int *c, int *tipo, time_t *actual) {
    switch (*c) {
        case 's':
            endwin();
            free(tipo);
            free(actual);
            exit(-1);
        case ' ':
            if (*menu == 1) {
                mvprintw(3,0,"       ");
                *menu = 0;
            }
            else {
                *menu = 1;
                mvprintw(3,0,"Pausado");
            }
            break;
        default:
            break;
    }
}
