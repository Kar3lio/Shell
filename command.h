#ifndef command_h
#define command_h

typedef struct command
{
    char** args;
    char** flags;

    int pipe_coming;
    char* gd;
    char* ld;
    char * qqd;
    
}command_t;

typedef command_t * commandptr;

#endif 

