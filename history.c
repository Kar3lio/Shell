#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "history.h"
#include <fcntl.h>
#include <unistd.h>


int min(int a, int b)
{
    return a<b ? a : b;
}

int SaveLine(char* line)
{
    //FILE* input_file = fopen(path,"a+");
    //fputs(line,input_file);
    //fputs("\n",input_file);
    //int fd = fileno(input_file);
    int fd = open(path,O_APPEND | O_WRONLY,0777);
    write(fd,line,strlen(line));
    close(fd);
    //fclose(input_file);
    return 0;
}

//devuelve la linea ubicada count_l(partiendo del final) en el history
char* GetHistoryLine(int count_l)
{
    FILE* input_file = fopen(path,"r");
    fseek(input_file,0,SEEK_END);

    int count_cl = 0;
    while (count_cl <= count_l)
    {
        fseek(input_file,-1,SEEK_CUR);
        
        long isbeg = ftell(input_file);
        if (!isbeg)
        {
            fseek(input_file,0,SEEK_END);
            break;
        }

        int c = fgetc(input_file);  //leer el caracter //en ascii es 10 el \n

        if (c == 10)
            count_cl++;
        if (count_cl > count_l)
            break;
    
        fseek(input_file,-1,SEEK_CUR);
    }
    char* line = malloc(100*sizeof(char));
    fgets(line,100,input_file);
    close(fileno(input_file));

    return line;
}

int ReadLines(int count_l)
{
    FILE* input_file = fopen(path,"r");
    // //int fd = open(path,O_RDONLY,0777);
    
    fseek(input_file,0,SEEK_END);
    
    int count_cl = 0;
    
    while (count_cl <= count_l)
    {
        fseek(input_file,-1,SEEK_CUR);
        
        long isbeg = ftell(input_file);
        if (!isbeg)
            break;

        int c = fgetc(input_file);  //leer el caracter //en ascii es 10 el \n

        if (c == 10)
            count_cl++;
        if (count_cl > count_l)
            break;
    
        fseek(input_file,-1,SEEK_CUR);
    }

    char* current_line = malloc(100*sizeof(char));
    // //char* current_line = calloc(100,sizeof(char));
    for (size_t i = 1; i <= min(count_cl,count_l); i++)
    {
        fgets(current_line,100,input_file);
        printf("%ld: ",i);
        printf("%s",current_line);
    }
    int fd = fileno(input_file);
    close(fd);
    free(current_line);
    //fclose(input_file);
    return 0;
}

// int main()
// {
//     SaveLine("abc");
//     ReadLines(10);
//     printf("***************\n");
//     ReadLines(4);
//     printf("***************\n");
//     printf("%s",GetHistoryLine(19));
//     SaveLine("fsdfsd");
//     return 0;
// }