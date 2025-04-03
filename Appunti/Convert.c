#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdbool.h>
#include <ctype.h>
int main(int argc, char *argv[])
{
    int n;
    bool continua;
    char carattere;
    do
    {
        printf("inserisci il numero del quale vuoi vedere la corrispondenza nel codice ascii:\n");
        scanf("%d", &n);
        printf("simbolo:");
        printf("%c\n", n);
        printf("vuoi continuare?\n");
        getchar();
        scanf("%c", &carattere);
        if (toupper(carattere) == 'S')
        {
            continua = true;
        }
        else if (toupper(carattere) == 'N')
        {
            continua = false;
        }
        else
        {
            printf("ERRORE\n");
            return -1;
        }
    } while (continua);
    return 0;
}
