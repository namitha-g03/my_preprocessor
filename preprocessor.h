#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include<stdio.h>
#define MAX_MACROS 100

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
