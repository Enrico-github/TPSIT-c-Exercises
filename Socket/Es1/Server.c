#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>

#define SERVERPORT 1450

void sostituzione(char stringa[])
{
    for (int i = 0; i < strlen(stringa); i++)
    {
        if (stringa[i] == 'a' || stringa[i] == 'e' || stringa[i] == 'i' || stringa[i] == 'o' || stringa[i] == 'u')
            stringa[i] = 'X';
    }
}

int main()
{

    struct sockaddr_in servizio;
    char stringa[20];
    int socketfd, soa, fromlen = sizeof(servizio);

    // Assegnazione valori alla struttura (dominio, indirizzo, porta)
    servizio.sin_family = AF_INET;
    servizio.sin_addr.s_addr = htonl(INADDR_ANY);
    servizio.sin_port = htons(SERVERPORT);

    // Creazione della socket
    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("Chiamata fallita alla system call socket");
        exit(0);
    }

    if (bind(socketfd, (struct sockaddr *)&servizio, sizeof(servizio)) == -1)
    {
        printf("Chiamata fallita alla system call bind");
        exit(0);
    }

    if (listen(socketfd, 10) == -1)
    {
        printf("Chiamata fallita alla system call listen");
        exit(0);
    }

    for (;;)
    {
        printf("Server in ascolto...\n");

        // Accettazione del collegamento
        if ((soa = accept(socketfd, (struct sockaddr *)&servizio, &fromlen)) == -1)
        {
            printf("Chiamata fallita alla system call accept");
            exit(0);
        }

        // Leggo la stringa proveniente dalla socket
        read(soa, stringa, sizeof(stringa));
        printf("Stringa ricevuta: %s", stringa);

        sostituzione(stringa);
        printf("Stringa convertita: %s    ", stringa);
        write(soa, stringa, sizeof(stringa));

        close(soa);
    }
    return 0;
}
