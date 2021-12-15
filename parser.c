#include "parser.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


char** split_line(char*line, int read)
{
    int count_w = 1;//cantidad de palabras
    char * temp_w;//palabra actual que estamos construyendo
    char** list_ws;//lista de palabras
    

    for (size_t i = 0; i < read; i++)
    {
        if (line[i] == ' ')
        {
            count_w++;
            while (line[i+1]==' ')
            {
                i++;
            }
        }
    }   

    list_ws = malloc((count_w + 1)*sizeof(char*));

    char *ch;
    ch=strtok(line," ");
    list_ws[0]=ch;
    for (size_t i = 1; i < count_w; i++)
    {
        ch=strtok(NULL," ,");
        list_ws[i]=ch;
    }
    
    return list_ws;
}
command_t** command_list(char*line, int read)
{
    char** list_ws=split_line(line,read);//lista de palabras con split() hecho

   
    int command_count = 1;               //cantidad de comandos que tendra la lista de comandos
    int pos = 0;                         //posiscion actual en el recorrido por la lista de comandos
    while(list_ws[pos]!=NULL)
    {
        if(!(strcmp(list_ws[pos],"<") && strcmp(list_ws[pos],">") && strcmp(list_ws[pos],"|") && strcmp(list_ws[pos],">>")))
        {
            command_count++;
        }
        pos++;
    }

    command_t** command_list = malloc((command_count + 1)*sizeof(command_t*));

    pos=1;
    int last_command_pos=0;
    int current_count = 0;
    char** args;
    command_t* current = malloc(sizeof(command_t));      //comando que estamos construyendo
    while(list_ws[pos]!=NULL)
    {
        if(!(strcmp(list_ws[pos],"<") && strcmp(list_ws[pos],">") && strcmp(list_ws[pos],"|") && strcmp(list_ws[pos],">>")))
        {
            args = malloc((pos-last_command_pos + 1)*sizeof(char*));
            for (size_t i = last_command_pos; i < pos; i++)
            {
                args[i-last_command_pos]=list_ws[i];
            }
            current->args = args;

            if(!(strcmp(list_ws[pos],"<")))
            {
                current->ld=list_ws[pos];
                pos+=2;
            }
            else if(!(strcmp(list_ws[pos],">")))
            {
                current->gd=list_ws[pos];
                pos+=2;
            }
            else if(!(strcmp(list_ws[pos],">>")))
            {
                current->qqd=list_ws[pos];
                pos+=2;
            }
            if((strcmp(list_ws[pos],"|")))
            {
                current->pipe_coming = 1;
            }

            command_list[current_count]=current;
            current_count++;
            current = malloc(sizeof(command_t));
            last_command_pos = pos+1;   
        }
        pos++;        
    }
    args = malloc((pos-last_command_pos + 1)*sizeof(char*));
    for (size_t i = last_command_pos; i < pos; i++)
    {
        args[i-last_command_pos]=list_ws[i];
    }
    current->args = args;
    command_list[current_count]=current;
    current_count++;
        
    return command_list;

}






