#ifndef parser_h
#define parser_h
#include "command.h"

char** split_line(char*line, int read);
command_t** command_list(char*line, int read);
#endif 
