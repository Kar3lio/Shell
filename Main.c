#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include "parser.h"
#include "execute.h"



int And_Or_Split(char* ch)
{
    int last2 = 0;
    for (size_t k = 0; k <= strlen(ch); k++)
    {
        if(k==strlen(ch))
        {
            char * temp = calloc(k-last2,sizeof(char));
            for (size_t l = last2; l < k; l++)
            {
                temp[l-last2]=ch[l];
            }
            command_t **list_c = command_list(temp, strlen(temp));
            execute(list_c);
            free(temp);
        }
        else if((ch[k]=='&' && ch[k+1]=='&' && ch[k-1]==' ' && ch[k+2]==' '))
        {
            char * temp = calloc(k+1-last2,sizeof(char));
            for (size_t l = last2; l < k; l++)
            {
                temp[l-last2]=ch[l];
            }
            command_t **list_c = command_list(temp, strlen(temp));
            int exit_status = execute(list_c);
            if(exit_status != 0)
            {
                return exit_status;
            }
            free(temp);
            last2 = k + 2;
        }
        else if ((ch[k]=='|' && ch[k+1]=='|' && ch[k-1]==' ' && ch[k+2]==' '))
        {
            char * temp = calloc(k+1-last2,sizeof(char));
            for (size_t l = last2; l < k; l++)
            {
                temp[l-last2]=ch[l];
            }
            command_t **list_c = command_list(temp, strlen(temp));
            int exit_status = execute(list_c);
            if(exit_status == 0)
            {
                return exit_status;
            }
            free(temp);
            last2 = k + 2;
        }
        
        
    }
    return 0;
    
}
int Semicolon_Split(char* line, int read)
{
    int last = 0;
    for (size_t i = 0; i < read; i++)
    {
        if(line[i]==';'|| line[i]=='\n')
        {
            char * ch=calloc((i+1-last),sizeof(char));
            for (size_t j = last; j < i; j++)
            {
                ch[j-last]=line[j];
            }
            And_Or_Split(ch);
            free(ch);
            last = i+1;          
        }
    }
}

int main(int argc, char const *argv[])
{
    int status = 1;

    history = malloc(10 * sizeof(char *));
    count_hist = 0;

    while (status)
    {
        char prompt[] = "shell$";
        printf("\e[1m\x1b[32m%s ", prompt);
        printf("\x1b[0m");


        char *line;
        size_t line_size = 0;
        size_t read = 0;

        read = getline(&line, &line_size, stdin); //lee la entrada y crea una lista de todas las palabras

        if (line[0] != ' ')
        {
            count_hist = count_hist % 10;
            history[count_hist] = line; //guardo la linea de command en history
            count_hist++;
        }
        
        Semicolon_Split(line,read);

    }

    return 0;
}

