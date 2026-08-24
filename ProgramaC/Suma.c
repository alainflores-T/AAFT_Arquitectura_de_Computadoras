#include <stdio.h>
#include <stdint.h>
typedef union{
unsigned  char byte;
struct{
unsigned char b0:1;
unsigned char b1:1;
unsigned char b2:1;
unsigned char b3:1;
unsigned char b4:1;
unsigned char b5:1;
unsigned char b6:1;
unsigned char b7:1;
}bits;
}Reg8Bits;
int main(){
int8_t a = 120 , b=20;
int8_t resultado=a+b;
Reg8Bits midata;
midata.byte = resultado;

printf("%d + %d = %d\n", a , b , resultado);

if(a>0 && b>0){
if(a> INT8_MAX - b)
{
printf("Desborde\n");
}
}
if(a<0 && b<0){
if(a<INT8_MIN +b ){
printf("Desborde\n");
}
}
printf("%d%d%d%d%d%d%d%d", midata.bits.b7,midata.bits.b6,midata.bits.b5,midata.bits.b4,
midata.bits.b3,midata.bits.b2,midata.bits.b1,midata.bits.b0);
printf("\n");
return 0;
}
