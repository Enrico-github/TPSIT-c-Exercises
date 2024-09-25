/*Progettare un algoritmo che effettui le seguenti operazioni:
• legga da tastiera una coppia di valori interi A e B con A<B;
• continui a leggere da tastiera una serie di valori interi, terminando quando il valore letto è al di fuori dell’intervallo [A, B];
• conteggi la media dei valori letti;
• prima di terminare, stampi il valore calcolato.*/

#include <stdio.h>

int main(int argc, char *argv[])
{

    int A,B,num,media,somma = 0,i = 0;
    do
    {
        printf("\ninserisci A");
        scanf("%d", &A);
        printf("\ninserisci B");
        scanf("%d", &B);
    } while (B < A);

    while (1)
    {
        printf("\ninserisci un numero");
        scanf("%d", &num);

        if (num >= A && num <= B)
        {
            somma = somma + num;
            i++;
        }
        else
        {
            break;
        }
    }

    media = somma / i;

    printf("\nla media è: %d", media);

    return 0;
}