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

int main()
{
    struct sockaddr_in servizio;
    char stringa[20];
    int socketfd, fromlen = sizeof(servizio);

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

    if ((connect(socketfd, (struct sockaddr *)&servizio, sizeof(servizio))) == -1)
    {
        printf("Chiamata fallita alla system call connect");
        exit(0);
    }

    printf("Inserisci una stringa\n");
    scanf("%s", stringa);

    // Invio la stringa al server
    write(socketfd, stringa, sizeof(stringa));
    read(socketfd, stringa, sizeof(stringa));

    printf("Stringa ricevuta: %s  ", stringa);

    close(socketfd);
    return 0;
}
