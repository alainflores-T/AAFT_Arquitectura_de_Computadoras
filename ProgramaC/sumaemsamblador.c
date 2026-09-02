#include<stdio.h>

int main(){

    int array[]= {0,5,10,15};
    int longitud = 4;
    int sum_total = 0;

    __asm__(
        "xor %%eax, %%eax \n\t"
        "xor %%rcx, %%rcx \n\t"

        ".bucle_acumular: \n\t"
        "cmp %2, %%ecx \n\t"
        "jge .fin_acumular \n\t"          

        "addl (%1, %%rcx, 4), %%eax \n\t"  

        "inc %%rcx \n\t"
        "jmp .bucle_acumular \n\t"

        ".fin_acumular: \n\t"
        "movl %%eax, %0 \n\t"

        :"=r" (sum_total) 
        :"r" (array), "r" (longitud) 
        :"rax","rcx"
    );

    printf("La suma total del arreglo es de %d\n", sum_total);

    return 0;
}

