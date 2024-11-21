#include <stdio.h>
#include <stdlib.h>
int Ricerca(int n, char *argv[], int r)
{
    int i;
for(int i = 1; i < n; i++)
{
    if(r == atoi(argv[i]))
    return i;
}
return -1; 
}
int main(int argc, char *argv[])
{
    int ricercato;
    printf("\ninserisci il numero da cercare: ");
    scanf("%d",&ricercato);
    int posizione = Ricerca(argc,argv,ricercato);
    if(posizione != -1)
    printf("il numero ricercato è in posizione %d",posizione);
    else
    {
    printf("\nil numero ricercato non è presente fra gli argomentio di argc");
    }
}
