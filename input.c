#include<stdio.h>
#include"preprocessor.h"
#define MAX 100
#define char int
#define SUM(a,b) a+b
int main
{
/*
preprocessor checking
comment removal
file inclusion
macros replacement
conditional compiltion
*/
char a,b;//macros
a='a';
printf("%d",a);
//macros with arguments
int x;
int m=10,n=20;
x=SUM(m,n);//macros
printf("%d",x+MAX);


}
