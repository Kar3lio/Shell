#include <stdio.h>
#include <stdlib.h>
#include "execute.h"
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

//comprueba si un comando es builtin
int is_builtin(command_t* command)
{
    return !(strcmp(command->args[0], "cd")&&strcmp(command->args[0],"help")
            &&strcmp(command->args[0],"exit"));
}

int exec_cd(char* new_path)
{
    int a = chdir(new_path);
    printf("%d\n",a);
    if (a!=0)
    {
        printf("cd: ");
        printf("%s",new_path);
        printf(": Not such file or directory\n");
    }
    return a;
}
int exec_help(char* command)
{
    printf("Se ejecutó help\n");
}
int exec_exit()//cuando se hace exit hay que guardar el history del shell
{
    exit(-1);
    return 1;
}

int exec_builtin(command_t* command)
{
    char* name = command->args[0];

    int exc = 0;
    if (strcmp(name,"cd"))
    {
        printf("se va a ejecutar cd\n");
        exc = exec_cd(command->args[1]);
    }
    if (strcmp(name,"help"))
    {
        exc = exec_help(command->args[1]);
        //printf("es help\n");

    }
    if (strcmp(name,"exit"))
    {
        //printf("es exit\n");
        exc = exec_exit();
    }
}

int execute(command_t** commands)
{
    int i = 0;
    while (commands[i] != NULL)
    {        
        if (is_builtin(commands[i]))
        {
            exec_builtin(commands[i]);
        }
        else
        {
            command_t* current = malloc(sizeof(command_t));
            current = commands[i];
            int pid = fork();
            if(pid == -1)
            {
                printf("Process Fork error");
                return 1;
            }
            if(pid==0)   //Proceso hijo
            {
                int error = execvp(commands[i]->args[0],current->args);
                if(error == -1)
                {
                    printf("Could not find program to execute\n");
                    return -1;
                }
            }
            else
            {
                int wstatus;
                wait(&wstatus);
                if (WIFEXITED(wstatus))
                {
                    int status_code = WIFEXITED(wstatus);
                    if(status_code == 0)
                    {
                        return 0;
                    }
                    else
                    {
                        return status_code;
                    }
                }
                
                return 0;
            }
        }
        
        i++;
    }
}
