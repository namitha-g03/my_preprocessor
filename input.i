#include<stdio.h>
#ifndef PREPROCESSOR_H

#include<stdio.h>

typedef struct
{
 char name[60];
 char value[512];
int is_function;
int arg_count;
char args[10][50];
}Macro;

void remove_comments(FILE *src,FILE *dest);
void include_files(FILE *src,FILE *dest);
void macro_handler(FILE *src,FILE *dest);




#endif
int main
{







int a,b;
a='a';
printf("%d",a);

int x;
int m=10,n=20;
x=m+n;
printf("%d",x+100);


}
