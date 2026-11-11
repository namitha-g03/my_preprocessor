#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include"preprocessor.h"


Macro macros[MAX_MACROS];
int macro_count=0;
// checking charachter is part of an identifier
int is_id_char(int ch)
{
        return isalnum((unsigned char)ch) || ch=='_';
}
// finding macro
int find_macro(char name[])
{
        int i;
        for(i=0;i<macro_count;i++)
        {
                if(strcmp(macros[i].name,name)==0)
                        return i;
        }
        return -1;
}
// add macro
void add_macro(char line[])
{
        char name[50];
        char *p=line;
        Macro *m;
        int i=0,j,index;
        while(isspace((unsigned char)*p))
                p++;
        if(strncmp(p,"#define",7)!=0)
                return;
        p+=7;
        while(isspace((unsigned char)*p))
                p++;
        // read macro names
        while(is_id_char(*p) && i < 49)
                name[i++]=*p++;

        name[i]='\0';

        if(i==0)
                return;

        index=find_macro(name);

        if(index== -1)
        {
                if(macro_count >= MAX_MACROS)
                        return;
                index=macro_count++;
        }
        m=&macros[index];

        strcpy(m->name,name);
        m->arg_count=0;
        // function like macro

        if(*p=='(')
        {
                m->is_function = 1;
                p++;

                while(*p && *p != ')')
                {
                        while(isspace((unsigned char)*p) || *p == ',')
                                p++;
                        if(*p == ')')
                                break;
                        if(m->arg_count >= 10)
                                return;

                        j=0;

                        while(is_id_char(*p) && j < 49)
                                m->args[m->arg_count][j++]=*p++;

                        if(j==0)
                                return;

                        m->args[m->arg_count][j]='\0';
                        m->arg_count++;
                }
                if(*p!=')')
                        return;
                p++;
        }
        else
        {
                // object like macro
                m->is_function = 0;
        }
        // skip spaces before macro value

        while(*p == ' ' || *p == '\t')
                p++;

        strncpy(m->value,p,sizeof(m->value)-1);
        m->value[sizeof(m->value)-1]='\0';
        // remove new line
        j=strlen(m->value);

        while(j > 0 && (m->value[j-1] == '\r'))
        {
                m->value[--j]='\0';
        }
}
// read function macro arguments

int read_arguments(char **p,char args[10][200],int *count)
{
        int n=0,j=0;
        int brackets=0;
        int in_string=0,in_char=0;
        int escape=0;
        while(isspace((unsigned char)**p))
                (*p)++;
        if(**p != '(')
                return 0;
        (*p)++;

        if(**p == ')')
        {
                (*p)++;
                *count=0;
                return 1;
        }

        while(**p)
        {
                int ch=(unsigned char)**p;

                if(escape)
                {
                        if(j<199)
                                args[n][j++]=ch;

                        escape=0;
                        (*p)++;
                        continue;
                }
                if((in_string || in_char) && ch == '\\')
                {
                        if(j<199)
                                args[n][j++]=ch;

                        escape=1;
                        (*p)++;
                        continue;
                }
                if(ch == '"' && !in_char)
                        in_string = !in_string;
                else if(ch == '\'' && !in_string)
                        in_char = !in_char;

                if(!in_string && !in_char)
                {
                        if(ch == '(')
                                brackets++;
                        else if(ch == ')')
                        {
                                if(brackets == 0)
                                {
                                        args[n][j]='\0';
                                        (*p)++;
                                        *count=n+1;
                                        return 1;
                                }
                                brackets--;
                        }
                        else if(ch == ',' && brackets == 0)
                        {
                                args[n][j]='\0';
                                n++;

                                if(n>=10)
                                        return 0;

                                j=0;
                                (*p)++;
                                continue;
                        }
                }
                if(j<199)
                        args[n][j++]=ch;
                (*p)++;

        }
        return 0;
}
// replace parameters in function macro

void expand_function(int index, char args[10][200],FILE *dest)
{
        char *p=macros[index].value;
        char word[100];
        int i,j,found;

        while(*p)
        {
                if(isalpha((unsigned char)*p) || *p == '_')
                {
                        i=0;
                        while(is_id_char(*p) && i<99)
                                word[i++]=*p++;

                        word[i]='\0';

                        found=0;

                        for(j=0;j<macros[index].arg_count;j++)
                        {
                                if(strcmp(word,macros[index].args[j])==0)
                                {
                                        fputs(args[j],dest);
                                        found=1;
                                        break;
                                }
                        }
                        if(!found)
                                fputs(word,dest);
                }
                else
                {
                        fputc(*p,dest);
                        p++;
                }

        }
}
//substitute macros
void substitute_line(char line[],FILE *dest)
{
        char word[100];
        char args[10][200];

        char *p=line;
        char *q;

        int i,index,count;
        int in_string=0,in_char=0;
        int escape=0;

        while(*p)
        {
                if(escape)
                {
                        fputc(*p,dest);
                        escape=0;
                        p++;
                        continue;
                }
                if((in_string || in_char) && *p == '\\')
                {
                        fputc(*p,dest);
                        escape=1;
                        p++;
                        continue;
                }
                if(*p == '"' && !in_char)
                        in_string= !in_string;
                else if(*p == '\'' && !in_string)
                        in_char= !in_char;
                if(in_string || in_char)
                {
                        fputc(*p,dest);
                        p++;
                        continue;
                }
                if(isalpha((unsigned char)*p) || *p == '_')
                {
                        i=0;
                        while(is_id_char(*p) && i<99)
                                word[i++]=*p++;
                        word[i]= '\0';

                        index=find_macro(word);
                        if(index == -1)
                        {
                                fputs(word,dest);
                                continue;
                        }
                        // object like_macro
                        if(!macros[index].is_function)
                        {
                                fputs(macros[index].value,dest);
                        }
                        else
                        {
                                q=p;
                                if(!read_arguments(&q,args,&count))
                                {
                                        fputs(word,dest);
                                        continue;
                                }
                                if(count != macros[index].arg_count)
                                {
                                        fputs(word,dest);
                                        continue;
                                }

                                expand_function(index,args,dest);
                                p=q;
                        }
                }
                else
                {
                        fputc(*p,dest);
                        p++;
                }
        }
}

// main macro handler
void macro_handler(FILE*src,FILE*dest)
{
        char line[4096];
        char *p;

        while(fgets(line,sizeof(line),src)!=0)
        {
                p=line;

                while(isspace((unsigned char)*p))
                        p++;

                if(strncmp(p,"#define",7) == 0 && isspace((unsigned char)p[7]))
                {
                        add_macro(p);
                }
                else
                {
                        substitute_line(line,dest);
                }
        }

}
