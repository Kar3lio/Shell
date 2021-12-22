#include <stdio.h>
#include <stdlib.h>
#include "execute.h"
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

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
int exec_history()
{
    count_hist = count_hist%10;//encapsular esto en un metodo(se escribe 3 veces el mismo código)
    history[count_hist] = "history";
    count_hist++;//hasta aqui

    for (size_t i = count_hist; i < 10; i++)
    {
        printf("%s",history[i]);
    }
    if (count_hist!=0)
    {
        for (size_t i = 0; i < count_hist; i++)
        {
            printf("%s",history[i]);
        }
    }
}
int exec_again(int hist_num)
{
    hist_num = hist_num%10;
    char*line = history[hist_num];

    count_hist = count_hist%10;
    history[count_hist] = line;
    count_hist++;

    int count_line = 0;
    while (line[count_line] != 10)
        count_line++;
    
    command_t** comm_list = command_list(line,count_line);
    execute(comm_list);
    
}

int exec_builtin(command_t* command)
{
    char* name = command->args[0];

    int exc = 0;
    if (strcmp(name,"cd")==0)
    {
        printf("se va a ejecutar cd\n");
        exc = exec_cd(command->args[1]);
    }
    if (strcmp(name,"help")==0)
    {
        exc = exec_help(command->args[1]);
        //printf("es help\n");

    }
    if (strcmp(name,"exit")==0)
    {
        //printf("es exit\n");
        exc = exec_exit();
    }
}

int execute(command_t** commands)
{
    int fd[2];
    /*if(pipe(fd) == -1)
    {
        printf("Pipeling Error");
        return 1;
    }*/
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
                return 2;
            }
            if(pid==0)   //Proceso hijo
            {
                if(current->gd != NULL)
                {
                    int file = open("tester.txt", O_WRONLY | O_CREAT , 0777);
                    if(file == -1)  
                    {
                        printf("Writing File Error");
                        return 3;
                    }
                    dup2(file, STDOUT_FILENO);
                    close(file);
                }
                if (current->ld != NULL)
                {
                    int file = open("tester.txt", O_RDONLY, 0777);
                    if(file == -1)
                    {
                        printf("Reading File Error");
                        return 4;
                    }
                    dup2(file, STDIN_FILENO);
                    close(file);
                }
                if (current->qqd != NULL)
                {
                    int file = open("tester.txt", O_APPEND | O_WRONLY | O_CREAT , 0777);
                    if(file == -1)  
                    {
                        printf("Writing File Error");
                        return 5;
                    }
                    dup2(file, STDOUT_FILENO);
                    close(file);
                }
                if(fd[0] != -1)
                {
                    dup2(fd[0], STDIN_FILENO);
                    close(fd[0]);
                }
                if(current->pipe_coming != 0)
                {
                    dup2(fd[1], STDOUT_FILENO);
                    close(fd[1]);
                }
                else
                {
                    fd[0]=-1;
                }

                int error = execvp(current->args[0],current->args);
                if(error == -1)
                {
                    printf("Could not find program to execute\n");
                    return 6;
                }
            }
            else        //Proceso padre
            {
                int wstatus;
                wait(&wstatus);
                /*if (WIFEXITED(wstatus))
                {
                    int status_code = WIFEXITED(wstatus);
                    if(status_code == 0)
                    {
                        return status_code;
                    }
                }*/
            }
        }
        i++;
    }
    return 0;
}
