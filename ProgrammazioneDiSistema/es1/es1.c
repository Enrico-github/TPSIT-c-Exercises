#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc != 2) // controllo errori nelgi argomenti
    {
        printf("Numero argomenti sbagliato\n");
        exit(1);
    }
    int p1p0[2], pid;

    pipe(p1p0); // apertura pipe

    pid = fork(); // primo processo

    if (pid == 0) // figlio
    {
        close(p1p0[0]);
        close(1);
        dup(p1p0[1]);
        close(p1p0[1]);
        execl("/bin/cat", "cat", argv[1], NULL);
        return -1;
    }
    else if (pid < 0) // gestione errori
    {
        perror("errore nella creazione del figlio");
    }

    pid = fork(); // secondo processo

    if (pid == 0) // figlio
    {
        close(p1p0[1]);
        close(0);
        dup(p1p0[0]);
        close(p1p0[0]);
        execl("/bin/more", "more", NULL);
        return -1;
    }
    else if (pid < 0) // gestione errori
    {
        perror("errore nella creazione del figlio");
    }
    // chiudo i canali di comunicazione usati nella pipe
    close(p1p0[1]);
    close(p1p0[0]);
    // attendo la morte dei processi figlio
    wait(&pid);
    wait(&pid);
    return 0;
}
