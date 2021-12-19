#include "parser.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


char** split_line(char*line, int read)
{
    int count_w = 1;//cantidad de palabras
    char * temp_w;//palabra actual que estamos construyendo
    char** list_ws;//lista de palabras
    
    //////////Agregar aqui si la entrada no empieza con ' ' guardar la linea en history/////////

    if(line[0]=='#')
    {
        read=0;
        list_ws=malloc(sizeof(char*));
        return list_ws;
    }
    /*else
    {
        strtok(line,"#");                  //quitamos el comentario de la linea a analizar
    }*/

    for (size_t i = 0; i < read; i++)         //recorrido para contar la cantidad de palabras
    {
        if (line[i] == ' ')
        {
            count_w++;
            while (line[i+1]==' ')
            {
                i++;
            }
        }
        else if(line[i] == '#')
        {
            if(line[i-1]==' ')
            {
                count_w--;
            }
            break;
        }
    }   


    list_ws = malloc((count_w + 1)*sizeof(char*));

    char *ch;
    ch=strtok(line," \n\t");
    list_ws[0] = ch;
    for (size_t i = 1; i < count_w; i++)     //recorrido para separar las palabras
    {
        ch = strtok(NULL," \n\t");
        list_ws[i] = ch;
    }
    
    return list_ws;
}
command_t** command_list(char*line, int read)
{
    
    char** list_ws=split_line(line,read);//lista de palabras con split() hecho
    
    int command_count = 1;               //cantidad de comandos que tendra la lista de comandos
    int pos = 0;                         //posiscion actual en el recorrido por la lista de comandos
    while(list_ws[pos]!=NULL)            //recorrido para contar la cantidad de comandos
    {
        if(!(strcmp(list_ws[pos],"|")))
        {
            command_count++;
        }
        pos++;
    }

    command_t** command_list = malloc((command_count + 1)*sizeof(command_t*));//arreglo donde se almacenan todos los comandos
    if(list_ws[0]==NULL)
    {
        return command_list;
    }

    int command_arguments[command_count]; //arreglo donde se almacenara la cantidad de argumentos de cada comando
    command_arguments[0]=1;
    pos = 1;                              //se reinicia la variable pos para realizar otro recorrido a la lista de palabras
    int current_count = 0;                //numero del comando actual
    command_t* current = malloc(sizeof(command_t));      //comando que estamos construyendo
    command_list[0]=current;
    while (list_ws[pos]!=NULL)            //recorrido donde se almacenara la cantidad de argumentos y se actualizaran 
    {                                     //las propiedades ld,gd,qqd y pipe_coming que tiene cada comando
        if(!(strcmp(list_ws[pos],"<")))
        {
            current->ld=list_ws[pos+1];
            pos+=2;
        }
        else if(!(strcmp(list_ws[pos],">")))
        {
            current->gd=list_ws[pos+1];
            pos+=2;
        }
        else if(!(strcmp(list_ws[pos],">>")))
        {
            current->qqd=list_ws[pos+1];
            pos+=2;
        }
        else if((strcmp(list_ws[pos],"|") == 0))
        {
            current->pipe_coming = 1;
            current = malloc(sizeof(command_t));
            current_count++;
            command_list[current_count]=current;
            pos+=2;
            command_arguments[current_count]=1;         //se inicializan todos comandos con argumento 1 porque tienen al menos
        }                                               //a ellos mismos como argumento
        else
        {
            command_arguments[current_count]++;
            pos++;
        }
    }

    current=command_list[0];
    current_count=0;                             //reiniciamos la variable para almacenar en ella el numero del comando actual
    current->args=malloc((command_arguments[0] + 1)*sizeof(char *));
    current->args[0]=list_ws[0];
    pos = 1;
    int current_arg=1;                          //numero del argumento actual
    while(list_ws[pos]!=NULL)                   //recorrido donde cada comando adquire sus argumentos
    {
        if(!(strcmp(list_ws[pos],"|")))
        {
            
            current_count++;
            current=command_list[current_count];
            current->args = malloc((command_arguments[current_count] + 1)*sizeof(char *));
            current->args[0]=list_ws[pos+1];
            current_arg=1;
            pos+=2;
            
        }
        else if(!(strcmp(list_ws[pos],"<") && strcmp(list_ws[pos],">") && strcmp(list_ws[pos],">>")))
        {
            pos+=2;
        }
        else
        {
            current->args[current_arg]=list_ws[pos];
            current_arg++;
            pos++;
        }
    }

    if(command_list[0]==NULL){
        printf("????");
        exit(0);
    }
    
    return command_list;
}






