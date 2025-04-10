#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    FILE *inputFile, *pariFile, *dispariFile;
    int num;

    if (argc < 2)
    {
        printf("Uso: %s <nome_file_input>\n", argv[0]);
        return 1;
    }

    inputFile = fopen(argv[1], "r");
    if (inputFile == NULL)
    {
        printf("Errore: impossibile aprire il file '%s'.\n", argv[1]);
        return 1;
    }

    pariFile = fopen("pari.txt", "w");
    dispariFile = fopen("dispari.txt", "w");
    if (pariFile == NULL || dispariFile == NULL)
    {
        printf("Errore nella creazione dei file di output.\n");
        fclose(inputFile);
        return 1;
    }

    while (fscanf(inputFile, "%d", &num) == 1)
    {
        if (num % 2 == 0)
        {
            fprintf(pariFile, "%d ", num);
        }
        else
        {
            fprintf(dispariFile, "%d ", num);
        }

        fclose(inputFile);
        fclose(pariFile);
        fclose(dispariFile);

        printf("Numeri separati in pari.txt e dispari.txt.\n");
        return 0;
    }
