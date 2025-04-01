#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <ctype.h>

int vocale(char lettera)
{
    lettera = tolower(lettera);
    if (lettera == 'a' || lettera == 'i' || lettera == 'u' || lettera == 'o' || lettera == 'e')
    {
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Numero argomenti sbagliato\n");
        exit(1);
    }

    int p1p2[2], p2p0[2], pid;
    int vocali = 0;

    pipe(p1p2);
    pid = fork();
    if (pid == 0)
    {
        close(p1p2[0]);

        close(1);
        dup(p1p2[1]);
        close(p1p2[1]);

        execl("/usr/bin/cat", "cat", argv[1], NULL);
        exit(1);
    }

    close(p1p2[1]);
    pipe(p2p0);
    pid = fork();
    if (pid == 0)
    {
        close(p2p0[0]);

        char carattere;
        while (read(p1p2[0], &carattere, sizeof(carattere)) > 0)
        {
            if (vocale(carattere))
            {
                vocali++;
            }
        }

        close(p1p2[0]);
        write(p2p0[1], &vocali, sizeof(vocali));

        close(p2p0[1]);
        exit(0);
    }

    close(p1p2[0]);
    close(p2p0[1]);

    read(p2p0[0], &vocali, sizeof(vocali));
    close(p2p0[0]);

    wait(&pid);
    wait(&pid);

    printf("Nel file sono presenti %d vocali.\n", vocali);
    return 0;
}
