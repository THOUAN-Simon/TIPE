#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

int main(){
    int* tab1 = malloc(sizeof(int)*10);
    for (int i=0; i<10;i++){
        tab1[i] = 10;
    }
    int* tab2 = malloc(sizeof(int)*10);
    memcpy(tab2,tab1,40);
    for (int i=0; i<10;i++){
        printf("la valeur est : %d \n", tab2[i]);
    }
    return 0;
}