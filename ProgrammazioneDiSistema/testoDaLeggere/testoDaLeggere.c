#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{

    FILE *sourceFile, *destFile;
    char carattere;
    char destFileName[] = "copia.txt";

    if (argc < 2)
    {
        printf("Uso: %s <nome_file_sorgente>\n", argv[0]);
        return 1;
    }

    sourceFile = fopen(argv[1], "r");
    if (sourceFile == NULL)
    {
        printf("Errore: impossibile aprire il file sorgente '%s'.\n", argv[1]);
        return 1;
    }

    destFile = fopen(destFileName, "w");
    if (destFile == NULL)
    {
        printf("Errore: impossibile creare il file '%s'.\n", destFileName);
        fclose(sourceFile);
        return 1;
    }

    while ((carattere = fgetc(sourceFile)) != EOF)  /* != -1 */
    { 
        fputc(carattere, destFile);
    }

    fclose(sourceFile);
    fclose(destFile);

    printf("Copia completata: '%s' -> '%s'\n", argv[1], destFileName);
    return 0;
}
