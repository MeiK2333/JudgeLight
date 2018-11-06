#include <stdio.h>
#include <stdlib.h>

int main() {
    char *p = (char *) malloc(sizeof(char *));
    *p = 'H';
    printf("%cello World!\n", *p);
    return 0;
}