#include<stdio.h>
#include "preprocessor.h"
void remove_comments(FILE* src,FILE*dest)
{
        int ch,next;

        while((ch=fgetc(src))!=-1)
        {
                if(ch=='/')
                {
                        next=fgetc(src);
                        //single line comment
                        if(next=='/')
                        {
                                while((ch=fgetc(src)) != '\n' && ch != -1);
                                if(ch=='\n')
                                        fputc('\n',dest);
                        }
                        // multi line comments
                        else if(next=='*')
                        {
                                while((ch=fgetc(src))!=-1)
                                {
                                        if(ch=='*')
                                        {
                                                next=fgetc(src);
                                                if(next=='/')
                                                        break;
                                                if(next != -1)
                                                        ungetc(next,src);
                                        }
                                        if(ch=='\n')
                                                fputc('\n',dest);
                                }
                        }
                        // normal divsion operator
                        else
                        {
                                fputc(ch,dest);
                                if(next != -1)
                                        ungetc(next,src);
                        }
                }
                else
                {
                        fputc(ch,dest);
                }

        }
}
