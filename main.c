#include<stdio.h>
#include<string.h>
#include"preprocessor.h"

int main(int argc,char** argv)
{
        FILE *src,*temp1,*temp2,*dest;
        char output[256];
        char temp_file1[]="temp1.txt";
        char temp_file2[]="temp2.txt";
        char *dot,*slash;
        if(argc!=2)
        {
                printf("usage:./mypreprocessor filename.c\n");
                return 1;
        }
        if(strlen(argv[1])>=sizeof(output))
        {
                printf("file name too long\n");
                return 1;
        }
        strcpy(output,argv[1]);
        dot=strrchr(output,'.');
        slash=strrchr(output,'/');
        if(dot!=NULL && (slash==NULL || dot>slash))
                strcpy(dot,".i");
        else
                strcat(output,".i");
        src=fopen(argv[1],"r");
        if(src==0)
        {
                printf("unable to open input file\n");
                return 1;
        }
        temp1=fopen(temp_file1,"w");
        if(temp1==0)
        {
                fclose(src);
                return 1;
        }
        remove_comments(src,temp1);

        fclose(src);
        fclose(temp1);

        temp1=fopen(temp_file1,"r");
        temp2=fopen(temp_file2,"w");
        if(temp1==0||temp2==0)
        {
                printf("file error\n");
                if(temp1 != 0)
                        fclose(temp1);
                if(temp2 != 0)
                        fclose(temp2);
                return 1;
        }


        include_files(temp1,temp2);

        fclose(temp1);
        fclose(temp2);
        temp2=fopen(temp_file2,"r");
        dest=fopen(output,"w");

        if(temp2==0 || dest==0)
        {
                printf("file error\n");
                if(temp2!=0)
                        fclose(temp2);
                if(dest!=0)
                        fclose(dest);
                return 1;
        }


        macro_handler(temp2,dest);


        fclose(temp2);
        fclose(dest);

        remove(temp_file1);
        remove(temp_file2);

        printf("preprocessing completed succesfully\n");
        printf("output file: %s\n",output);
        return 0;
}
