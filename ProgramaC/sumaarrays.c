#include<stdio.h>

int main(){


int  array []  = {10,20,30};
int sum=0;
for(int i=0;i<3;++i){
sum= sum + array[i];
} 

printf("Suma total del arreglo %d",sum);

return 0;

}
