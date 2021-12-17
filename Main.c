#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "execute.h"

int main(int argc, char const *argv[])
{
    int status = 1;

    while (status)
    {
        char prompt[]="shell$";
        printf("\e[1m\x1b[32m%s ",prompt);
        printf("\x1b[0m");


        char * line;
        size_t line_size = 0;
        size_t read = 0;

        read = getline(&line, &line_size, stdin); //lee la entrada y crea una lista de todas las palabras
        command_t** list_c = command_list(line, read);
        

        int pos=0;
        while(list_c[pos]!=NULL)
        {
            command_t* current=list_c[pos];
            int arg_pos=0;
            while (current->args[arg_pos]!=NULL)
            {
                printf("%s ",current->args[arg_pos]);
                arg_pos++;
            }
            pos++;
            printf("\n");
        }
        
        printf("End");

        int command_count = 0;
        
    }
    




    return 0;
}
