/* 
 * Simple stopwatch written in C
 * sw.c
 */
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <malloc.h>
#include <curses.h>
#include <getopt.h>

void setTime(int *valor, char *tipo, struct tm *tiempo) {
  if (*tipo == 's' || *tipo == 'm' || *tipo == 'h') {
    if (*tipo == 's') tiempo->tm_sec  += *valor;
    if (*tipo == 'm') tiempo->tm_min  += *valor;
    if (*tipo == 'h') tiempo->tm_hour += *valor;
  }
  // si no se ingresa el tipo, segundos
  else tiempo->tm_sec  += *valor;
}

void eleccion(int *menu, int *c, time_t *actual) {
  switch (*c) {
    case 's':
      endwin();
      if (actual != NULL) free(actual);
      exit(EXIT_FAILURE);
    case ' ':
      if (*menu) {
        mvprintw(3,0,"     ");
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

static struct option long_options[] = {
  {"tiempo", required_argument, 0, 't'},
  {"tarea",  required_argument, 0, 's'},
  {0,      0,         0,  0}
};

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

  int long_index = 0;
  int opt = 0;
  int longitud = 0;
  char tipo = 'm';
  char *tarea = NULL;
  while ((opt = getopt_long(argc, argv, "t:s:",
          long_options, &long_index)) != -1) {
    switch(opt) {
      case 't':
        valor = atoi(optarg);
        tipo = *(optarg + strlen(optarg) - 1);
        break;
      case 's':
        longitud = strlen(optarg);
        tarea = (char *)malloc(longitud + 1);
        strcpy(tarea, optarg);
      default:
        break;
    }
  }
  initscr();
  noecho();
  cbreak();     // don't interrupt for user input el inverso rompe la terminal
  timeout(500);   // wait 500ms for key press
  *actual = time(NULL);
  tiempo = localtime(actual);
  setTime(&valor, &tipo, tiempo);
  final = mktime(tiempo);
  mvprintw(0,0,"Tiempo inicial: %s", ctime(actual));
  if (tarea == NULL) {
    longitud = strlen("Tarea no especificada.");
    tarea = (char *)malloc(longitud + 1);
    strcpy(tarea, "Tarea no especificada.");
  }
  mvprintw(5,0,"Tarea: %s", tarea);
  while ((diferencia = difftime(final, *actual)) > 0) {
    mvprintw(1,0,"Tiempo final:   %s", ctime(&final));
    *actual = time(NULL);
    int c = getch();
    eleccion(sesion, &c, actual);
    if (*sesion) {
      final = *actual + (int) diferencia;
    }
    mvprintw(2,0,"Tiempo actual:  %s", ctime(actual));
  }
  endwin();
  if (sesion != NULL) free(sesion);
  if (actual != NULL) free(actual);
  exit(EXIT_SUCCESS);
}
