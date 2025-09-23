#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>

#define DIM_BUFFER 1024

int main(int argc, char *argv[])
{
    if (argc != 2) // controllo errori negli argomenti
    {
        printf("Numero argomenti sbagliato\n");
        exit(1);
    }
    int p1p0[2], p2p0[2], pid, nread, fd;
    char buff[DIM_BUFFER];

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
    pipe(p2p0);
    pid = fork(); // secondo processo

    if (pid == 0) // figlio
    {
        close(p1p0[1]);
        close(0);
        dup(p1p0[0]);
        close(p1p0[0]);

        close(p2p0[0]);
        close(1);
        dup(p2p0[1]);
        close(p2p0[1]);

        execl("/bin/wc", "wc", (char *)0);  // restituisce righe - parole - caratteri
    }
    else if (pid < 0) // gestione errori
    {
        perror("errore nella creazione del figlio");
    }
    // chiudo i canali di comunicazione usati nelle pipe
    close(p1p0[1]);
    close(p1p0[0]);
    close(p2p0[1]);

    fd = open("wc.txt", O_WRONLY | O_CREAT, 0777); // apro il file wc.txt e lo creo nel caso non ci sia
    while ((nread = read(p2p0[0], buff, sizeof(buff))) > 0)
    {
        write(fd, buff, nread); // scrivo su wc.txt
    }
    close(p2p0[0]); // chiudo la lettura
    close(fd);      // chiudo il file

    return 0;
}
