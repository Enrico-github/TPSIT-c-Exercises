/*Esercizio – Preparazione verifica
Scrivere un programma in C che dopo aver dichiarato una struttura denominata Persona con i
seguenti campi: Nome, Cognome, età, codice fiscale, reddito annuo, anno_nascita,residenza(città) ;
determini:
1) La persona con reddito annuo più alto e quella con reddito annuo più basso.Visualizzare
per entrambe: nome, cognome, città e reddito.
2) Ordinare la lista delle persone in base all’anno di nascita** (dal più anziano al più
giovane), e visualizzarla.
3) Consentire all’utente di cercare una persona inserendo il codice fiscale..
4) Visualizzare tutte le persone con reddito superiore o uguale a un valore scelto dall’utente.
5) Calcolare la media dei redditi delle persone registrate nel sistema.
6) Visualizzare tutte le persone nate in un anno specifico, scelto dall’utente;*/

//@author Enrico Prearo 5F 20/10/2025

// librerie
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// variabile globale per la struct
#define MAX 10

// struct persona
typedef struct
{
    char nome[10];
    char cognome[10];
    int età;
    char cf[16];
    int redditoAnnuo;
    int annoNascita;
    char cittàResidenza[15];
} Persona;

// inserimento
void Inserimento(Persona p[], int *nPersone)
{
    for (int i = 0; i < *nPersone; i++)
    {
        printf("\nInserisci i dati della persona %d:\n", i + 1);

        printf("Nome: ");
        scanf("%s", p[i].nome); 

        printf("Cognome: ");
        scanf("%s", p[i].cognome);

        printf("Età: ");
        scanf("%d", &p[i].età);

        printf("Codice fiscale: ");
        scanf("%s", p[i].cf);

        printf("Reddito annuo: ");
        scanf("%d", &p[i].redditoAnnuo);

        printf("Anno di nascita: ");
        scanf("%d", &p[i].annoNascita);

        printf("Città di residenza: ");
        scanf("%s", p[i].cittàResidenza);
    }
}

// visualizzazione
void Visualizza(Persona p[])
{
    printf("nome: %s - cognome: %s - età: %d - cf: %s - reddito annuo: %d - anno di nascita: %d - città di residenza: %s",
           p->nome, p->cognome, p->età, p->cf, p->redditoAnnuo, p->annoNascita, p->cittàResidenza);
}

// punto 1
void RedditoMaxeMin(Persona p[], int nPersone)
{
    int iMAX = 0, iMIN = 0;

    for (int i = 1; i < nPersone; i++)
    {
        if (p[i].redditoAnnuo > p[iMAX].redditoAnnuo)
        {
            iMAX = i;
        }
        if (p[i].redditoAnnuo < p[iMIN].redditoAnnuo)
        {
            iMIN = i;
        }
    }
    printf("il reddito minimo è: %d \nil reddito massimo è: %d\n", p[iMIN].redditoAnnuo, p[iMAX].redditoAnnuo);
}

// punto 2
void OrdinaPerDataDiNascita(Persona p[],int nPersone) // dal più anziano al più giovane
{
    Persona temp;
    for(int i = 0; i < nPersone - 1; i++)
    {
        for(int j = i + 1; j < nPersone; j++)
        {
            if(p[i].annoNascita > p[j].annoNascita)
            {
                temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }
}

// punto 3
void RicercaxCF(Persona p[], int nPersone, char cf[16])
{
    for(int i = 0; i < nPersone; i++)
    {
        if(p[i].cf == cf)
        {
            Visualizza(&p[i]);
        }
    }
}

// punto 4
void RicercaxReditoSuperiore(Persona p[], int nPersone, int soglia) //>= alla soglia inserita
{
    for(int i = 0; i < nPersone; i++)
    {
        if(p[i].redditoAnnuo >= soglia)
        {
            Visualizza(&p[i]);
        }
    }
}

// punto 5
float MediaRediti(Persona p[], int nPersone)
{
    float somma = 0;
    for (int i = 0; i < nPersone; i++) 
    {
    somma += p[i].redditoAnnuo;
    }
    return somma / nPersone;
}

// punto 6
void VisualizzaPersoneInAnnoX(Persona p[], int nPersone, int anno)
{
    for(int i = 0; i < nPersone; i++)
    {
        if(p[i].annoNascita == anno)
        {
            Visualizza(&p[i]);
        }
    }
}
int main(int argc, char *argv[])
{
    Persona persone[MAX];
    int scelta, nPersone, soglia, anno;
    char cf[16];
    printf("quante persone vuoi inserire?");
    scanf("%d", &nPersone);
    Inserimento(persone, &nPersone);

    do
    {
        printf("\n--- MENU ---\n");
        printf("1) Persona con reddito più alto e più basso\n");
        printf("2) Ordinare per anno di nascita\n");
        printf("3) Cercare per codice fiscale\n");
        printf("4) Visualizzare persone con reddito >= soglia\n");
        printf("5) Calcolare media redditi\n");
        printf("6) Persone nate in un anno specifico\n");
        printf("0) Esci\n");
        printf("Scelta: ");
        scanf("%d\n", &scelta);
        switch (scelta)
        {
        case 1:
            RedditoMaxeMin(persone, nPersone);
            break;
        case 2:
            OrdinaPerDataDiNascita(persone, nPersone);
            break;
        case 3:
            printf("Inserisci il cf che vuoi ricercare:\n");
            scanf("%s",cf);
            RicercaxCF(persone, nPersone, cf);
            break;
        case 4:
            RicercaxReditoSuperiore(persone, nPersone, soglia);
            break;
        case 5:
            float media = MediaRediti(persone, nPersone);
            printf("la media è: %f\n",media);
            break;
        case 6:
            VisualizzaPersoneInAnnoX(persone,nPersone,anno);
            break;
        case 0:
            printf("Programma terminato, premere invio per continuare...");
            break;
        default:
            printf("scelta non valida");
        }
    } while (scelta != 0);
    return 0;
}
