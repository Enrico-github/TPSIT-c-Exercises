/*Dato N un numero intero positivo, generare e visualizzare il numero successivo.*/

#include<stdio.h>

int main(int argc, char * argv[]) {
  int N;
  printf("\ndai un valore ad N:");
  scanf("\n%d", & N);
  for (int i = 0; i < 1; i++) {
    N++;
  }
  printf("\nil valore di N è: %d", N);
  return 0;
}