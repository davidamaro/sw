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
#define false 0
#define true 1

void eleccion(int *menu, int *c, time_t *actual);
void setTime(int *valor, char *tipo, struct tm *tiempo);

int main(int argc, char *argv[]) {
    struct tm *tiempo;
    time_t final;
    time_t *actual;
    int valor = 20;
    double diferencia = 0;

    actual = (time_t *)malloc(sizeof(time_t));
    int *sesion = NULL;
    sesion = (int *)malloc(sizeof(int));
    *sesion = 0;

    int i = 0;
    int longitud = 0;
    char tipo = 'm';
    char *tarea = NULL;
    for (i = 0; i < argc; i++) {
        if (strlen(argv[i]) > 2 ) {
            if (strncmp(argv[i], "-s", 2) == 0) {
                longitud = strlen(argv[i]);
                tarea = (char *)malloc(longitud + 1);
                strcpy(tarea, argv[i] + 2);
            }
            if (strncmp(argv[i], "-t", 2) == 0) {
                tipo = *(argv[i] + strlen(argv[i]) - 1);
                *(argv[i] + strlen(argv[i]) - 1) = 0;
                valor = atoi(argv[i] + 2);
            }
        }
        if (strlen(argv[i]) == 2 && // sea un identificador de argumento
            strlen(argv[i + 1])) {  // exista el siguiente argumento
            if (strcmp(argv[i], "-t") == 0) {
                tipo = *(argv[i + 1] + strlen(argv[i + 1]) - 1);
                *(argv[i + 1] + strlen(argv[i + 1]) - 1) = 0;
                valor = atoi(argv[i + 1]);
            }
            if (strcmp(argv[i], "-s") == 0) {
                longitud = strlen(argv[i + 1]);
                tarea = (char *)malloc(longitud + 1);
                strcpy(tarea, argv[i + 1]);
            }
            i++;
        }
    }

    initscr();
    noecho();
    cbreak();         // don't interrupt for user input el inverso rompe la terminal
    timeout(500);     // wait 500ms for key press
    *actual = time(NULL);
    tiempo = localtime(actual);
    setTime(&valor, &tipo, tiempo);
    final = mktime(tiempo);
    mvprintw(0,0,"Tiempo inicial: %s", ctime(actual));
    if (tarea == NULL)
        tarea = "Tarea no especificada.";
    mvprintw(5,0,"Tarea: %s", tarea);
    while ((diferencia = difftime(final, *actual)) > 0) {
        mvprintw(1,0,"Tiempo final:   %s", ctime(&final));
        *actual = time(NULL);
        int c = getch();
        eleccion(sesion, &c, actual);
        if (*sesion) {
            final = *actual + (int) diferencia;
        }
        mvprintw(2,0,"Segundos restantes: %d\n", (int)diferencia);
    }
    endwin();
    if (sesion != NULL)
        free(sesion);
    if (actual != NULL)
        free(actual);
    return 0;
}

void setTime(int *valor, char *tipo, struct tm *tiempo) {
    if (*tipo == 's') tiempo->tm_sec  += *valor;
    if (*tipo == 'm') tiempo->tm_min  += *valor;
    if (*tipo == 'h') tiempo->tm_hour += *valor;
}

void eleccion(int *menu, int *c, time_t *actual) {
    switch (*c) {
        case 's':
            endwin();
            if (actual != NULL)
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
