#include <stdio.h>

int main(int argc, char *argv[]) {
    // dichiarazione variabili
    int num1, num2, prodotto;
    int somma = 0;

    do {
        // inserimento dei numeri da tastiera
        printf("inserisci il primo numero: ");    
        scanf("%d", &num1);
        printf("inserisci il secondo numero: ");
        scanf("%d", &num2);
        
        // calcolo del prodotto
        prodotto = num1 * num2;
        printf("il prodotto è: %d\n", prodotto);

        somma = somma + prodotto; 
    } while (num1 != 0 && num2 != 0);

    // stampa della somma
    printf("la somma è: %d\n", somma);

    return 0;
}
