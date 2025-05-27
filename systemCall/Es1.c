#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_LEN 256

int main() {
    pid_t pid1, pid2;
    int status;

    // Primo figlio: scrive frase.txt
    pid1 = fork();
    if (pid1 == 0) {
        // Codice del figlio F1
        char frase[MAX_LEN];
        printf("Inserisci una frase: ");
        fgets(frase, MAX_LEN, stdin);

        FILE *fp = fopen("frase.txt", "w");
        if (fp == NULL) {
            perror("Errore nell'apertura di frase.txt");
            exit(1);
        }
        fputs(frase, fp);
        fclose(fp);
        exit(0);
    }

    waitpid(pid1, &status, 0); // Attende la fine di F1

    // Secondo figlio: crea frase_verificata.txt
    pid2 = fork();
    if (pid2 == 0) {
        // Codice del figlio F2
        char frase[MAX_LEN];
        FILE *fp_in = fopen("frase.txt", "r");
        if (fp_in == NULL) {
            perror("Errore nell'apertura di frase.txt");
            exit(1);
        }
        fgets(frase, MAX_LEN, fp_in);
        fclose(fp_in);

        // Rimuove l'eventuale \n e aggiunge " Verificata"
        frase[strcspn(frase, "\n")] = '\0'; // Rimuove newline
        strcat(frase, " Verificata");

        FILE *fp_out = fopen("frase_verificata.txt", "w");
        if (fp_out == NULL) {
            perror("Errore nell'apertura di frase_verificata.txt");
            exit(1);
        }
        fputs(frase, fp_out);
        fclose(fp_out);
        exit(0);
    }

    waitpid(pid2, &status, 0); // Attende la fine di F2

    // Codice del padre: scrive report.txt
    FILE *fp_frase = fopen("frase.txt", "r");
    FILE *fp_verificata = fopen("frase_verificata.txt", "r");
    FILE *fp_report = fopen("report.txt", "w");

    if (!fp_frase || !fp_verificata || !fp_report) {
        perror("Errore nell'apertura dei file nel padre");
        exit(1);
    }

    char buffer[MAX_LEN];

    fputs("Contenuto di frase.txt:\n", fp_report);
    while (fgets(buffer, MAX_LEN, fp_frase)) {
        fputs(buffer, fp_report);
    }

    fputs("\nContenuto di frase_verificata.txt:\n", fp_report);
    while (fgets(buffer, MAX_LEN, fp_verificata)) {
        fputs(buffer, fp_report);
    }

    fclose(fp_frase);
    fclose(fp_verificata);
    fclose(fp_report);

    printf("Processo completato. Controlla il file report.txt.\n");

    return 0;
}
