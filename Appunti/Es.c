#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
    int p; //var intera che gestisce padre e figlio - p nel padre = PID del figlio - p nel figlio = 0 
    p = fork(); // padre genera un processo figlio
    if (p > 0) // padre
    {
        printf("sono il padre p=%d, PID=%d-padre PID=%d\n", p, getpid(), getppid()); // stampa p e PID del padre
    }
    else if(p == 0) // figlio
    {
        printf("sono il figlio p=%d, PID=%d-padre PID=%d\n", p, getpid(), getppid()); // stampa p e PID del figlio
    }
    else
    {
        printf("errore nella generazione del processo");  //errore
    }
    return 0;   //processo init processo padre generato dal kernel
}
