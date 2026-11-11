#include<stdio.h>
#include<string.h>
#include"preprocessor.h"

void include_files(FILE*src,FILE*dest)
{
        char line[1024];
        char filename[256];
        int i,ch;
        FILE *header;
        while(fgets(line,sizeof(line),src)!=0)
        {
                if(strncmp(line,"#include",8)==0)
                {
                        i=0;
                        while(line[i] != '"' && line[i]!= '<' && line[i] != '\0')
                        {
                                i++;
                        }
                        if(line[i] == '"' || line[i] == '<')
                        {
                                char end;
                                end=(line[i] == '"') ? '"' : '>';
                                i++;
                                int j=0;
                                while(line[i] != end && line[i] != '\0' && j<255)
                                {
                                        filename[j++]=line[i++];
                                }
                                filename[j]='\0';
                                header = fopen(filename,"r");
                                if(header != 0)
                                {
                                        while((ch=fgetc(header))!=-1)
                                        {
                                                fputc(ch,dest);
                                        }
                                        fclose(header);
                                        continue;
                                }
                        }
                }
                fputs(line,dest);
        }
}
