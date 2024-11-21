/*Dato N un numero intero positivo, generare e visualizzare in ordine crescente i numeri pari minori o uguali a N.*/ 

#include<stdio.h>

int main(int argc, char * argv[]) {
  int N;
  do {
    printf("\ndai un valore ad N:");
    scanf("\n%d", & N);
  } while (N < 0);
  for (int i = -1; i < N; i++) {
    i++;
    printf("\n%d", i);
  }
  return 0;
}
