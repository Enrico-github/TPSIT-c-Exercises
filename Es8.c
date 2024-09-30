/*Progettare un algoritmo che risolve il seguente problema. 
Si desidera calcolare la somma delle radici quadrate di N valori numerici inseriti dall’utente, con N inserito in input. 
L’algoritmo deve stampare la somma calcolata. 
L’algoritmo deve terminare con un messaggio di errore quando viene inserito un numero che non permette di effettuare il calcolo (nel dominio dei numeri reali).*/

#include<stdio.h>
#include<math.h>

int main(int argc, char *argv[]){

    int N,num;
    double somma = 0;
    printf("\nQuanti numeri si desiderano inserire? ");
    scanf("%d", &N);

    for (int i = 0; i < N; i++) {
        printf("\nInserisci un numero: ");
        scanf("%d", &num);

        if (num < 0) {
            printf("\nIl numero %d è negativo e non è un numero reale.\n", num);
            return 1;
        }

        somma += sqrt((double)num); 
        printf("\nLa somma è %f\n", somma);
    }

    return 0;
}