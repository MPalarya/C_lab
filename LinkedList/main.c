#include <stdio.h>
#include <stdlib.h>
#include "CommandParser.h"
#include "LinkedList.h"

int main() {
	char commandStr[MAX_COMMAND_LENGTH];
	int* arg1 = (int*)malloc(sizeof(int));
	int* arg2 = (int*)malloc(sizeof(int));
	ECommand command;
	
	LinkedList* list = newList();
	EResult result = SUCCESS;
	
	while (result != FAIL)
	{
		fgets(commandStr, MAX_COMMAND_LENGTH, stdin);
		command = parseCommand(commandStr, arg1, arg2);
		result = executeCommand(list, command, *arg1, *arg2);
	}

	freeList(list);
	free(arg1);
	free(arg2);

	return 0;
}
