#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "execute.h"

int main(int argc, char const *argv[])
{
    int status = 1;

    history = malloc(10*sizeof(char*));
    count_hist = 0;

    while (status)
    {
        char prompt[]="shell$";
        printf("\e[1m\x1b[32m%s ",prompt);
        printf("\x1b[0m");


        char * line;
        size_t line_size = 0;
        size_t read = 0;

        read = getline(&line, &line_size, stdin); //lee la entrada y crea una lista de todas las palabras
        
        if (line[0]!=' ')
        {
            count_hist = count_hist%10;
            history[count_hist] = line;//guardo la linea de command en history
            count_hist++;
        }
        
        command_t** list_c = command_list(line, read);
                
        execute(list_c);

        int command_count = 0;
        count_hist++;
        
    }
    
    return 0;
}
