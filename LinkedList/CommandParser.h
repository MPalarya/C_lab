#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include "LinkedList.h"

#define DELIMITER ' '

#define NUM_OF_COMMANDS 7 // do not count UNDEFINED as a command
#define MAX_COMMAND_LENGTH 100
#define MAX_WORDS 3

typedef enum { ADD_END, ADD_START, ADD_AFTER, INDEX, DEL, PRINT, EXIT, UNDEFINED } ECommand;

ECommand parseCommand(char* commandStr, int* out_arg1, int* out_arg2);

EResult executeCommand(LinkedList* list, ECommand command, int arg1, int arg2);

#endif // !COMMAND_PARSER_H