/*Si scriva un programma che calcoli l’elevazione a potenza del valore intero
passato come primo argomento per il secondo argomento intero.

Esempio:

$./es 2 3 
2 ^ 3 = 8*/
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
    int N = atoi(argv[1]), esponente = atoi(argv[2]),risultato = 1;
    if(argc != 3)
    {
        printf("errore nel numero di argomenti");
        exit(0);
    }
    for(int i = 0; i < esponente; i++)
    {
        risultato = N * risultato;
    }
    printf("il risultato di %d ^ %d = %d", N, esponente, risultato);
    return 0;
}
