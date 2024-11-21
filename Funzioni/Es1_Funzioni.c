#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
    int num1,num2,somma = 0;
    printf("\ninserisci il primo numero:\n");
    scanf("%d",&num1);
    printf("inserisci il secondo numero:\n");
    scanf("%d",&num2);
    somma = Somma(num1,num2);
    printf("la somma è: %d", somma);
}
int Somma(int num1, int num2)
{ 
    return num1 + num2;
}
