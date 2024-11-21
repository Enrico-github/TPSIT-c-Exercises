/*Progettare un algoritmo che effettui le seguenti operazioni:

• continui a leggere da tastiera una serie di terne di valori interi A ,B e C finchè non vengono inseriti dei valori tali per cui A + B < C
• conteggi il numero di volte in cui la differenza tra A e B è pari, dispari, e quando è nulla
• prima di terminare, visualizzi il valore dei valori conteggiati*/
#include<stdio.h>

int main(int argc, char *argv[])
{
    //dichiaraione variabili
    int A,B,C,diff,contP = 0,contD = 0,contN = 0; //contP = contatore pari - contD = contatore dispari - contN = contatore nullo
    //inserimento
    do{
        printf("inserisci A:\n");
        scanf("%d",&A);
        printf("inserisci B:\n");
        scanf("%d",&B);
        printf("inserisci C:\n");
        scanf("%d",&C);
        diff=A-B;
        if(diff % 2 == 0)
        {
            contP++;
        }
        else if(diff == 0)
        {
            contN++;
        }
        else
        {
            contD++;
        }
    }while(A+B>=C);
    printf("\ncontatore pari %d",contP);
    printf("\ncontatore dispari %d",contD);
    printf("\ncontatore nullo %d",contN);
return 0;
}
