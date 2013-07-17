/* sw2.c
 */
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <termios.h>
#include <malloc.h>
#define BUFSIZE 80
#define false 0
#define true 1

static struct termios old, new;

/* Initialize new terminal i/o settings */
void initTermios(int echo) 
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  new = old; /* make new settings same as old settings */
  new.c_lflag &= ~ICANON; /* disable buffered i/o */
  new.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
  tcsetattr(0, TCSANOW, &new); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) 
{
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) 
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* Read 1 character without echo */
char getch(void) 
{
  return getch_(0);
}

/* Read 1 character with echo */
char getche(void) 
{
  return getch_(1);
}

int filtro(char *cadena, int *tipo);
void setTime(int valor, int *tipo, struct tm *tiempo);

int main(int argc, char *argv[]) {
    struct tm *fh;
    time_t segundos;
    time_t *inicio;
    int valor, *tipo;
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

    *inicio = time(NULL);
    fh = localtime(inicio);
    setTime(valor, tipo, fh);
    segundos = mktime(fh);
    while (difftime(segundos, *inicio) > 0) {
        *inicio = time(NULL);
    }
    system("mplayer -really-quiet mus.mp3");
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
