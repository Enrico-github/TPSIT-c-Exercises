#include <stdio.h>
#include <string.h>
int main(int argc, char *argv[])
{
    int *t = 0;
    if(argc != 3)
{
    printf("errore nel numero degli argomenti");
    exit(0);
}
    Ricerca(argv[1],argv[2],&t);
    printf("la lettera %c compare %d in %c",argv[2],&t,argv[1]);
	return 0;
}
void Ricerca(char string[], char carattere, int *t)
{
    for(int i = 0; i < strlen(string); i++)
    {
        if(string[i == carattere])
        *t++;
    }
}
