#include <stdio.h>

int main(int argc, char *argv[]) {
    // dichiarazione variabili
    int num1, num2, rapporto;

    do {
        // inserimento dei numeri da tastiera
        printf("inserisci il primo numero: ");
        scanf("%d", &num1);
        printf("inserisci il secondo numero: ");
        scanf("%d", &num2);

        // calcolo del rapporto
        if (num1 > num2) {
            rapporto = num2 / num1;
        } else {
            rapporto = num1 / num2;
        }

        // stampa del rapporto
        printf("il rapporto è: %d\n", rapporto);
    } while (num1 != 0 && num2 != 0);

    return 0;
}
