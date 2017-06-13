#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "CommandParser.h"


// ##################################
// #   Internal Private Functions   #
// ##################################
int stringToInt(char* str) {
	int num = 0;
	int sign = 1;

	if (*str == '-') {
		sign = -1;
		str++;
	}
	else if (*str == '+')
		str++;

	while (isdigit(*str)) {
		num *= 10;
		num += *str - '0';
		str++;
	}
	return sign*num;
}

// #################################
// #   External Public Functions   #
// #################################
ECommand parseCommand(char* commandStr, int* out_arg1, int* out_arg2) {
	// all commands strings, important to keep same order as defined in ECommand
	const char* const allCommands[] = {
		"ADD_END", "ADD_START", "ADD_AFTER", "INDEX", "DEL", "PRINT", "EXIT"
	};
	char* delimiterPtr = commandStr;
	char* wordPtrs[MAX_WORDS] = { NULL };
	ECommand command = UNDEFINED;
	char* p;
	int i;
	
	// convert entire command to upper case
	for (p = commandStr; *p; p++)
		*p = toupper(*p);

	// get each word pointer individually
	for (i = 0; i < MAX_WORDS; i++) {
		wordPtrs[i] = delimiterPtr;
		delimiterPtr = strchr(delimiterPtr, DELIMITER);
		if (!delimiterPtr)
			break;
		delimiterPtr++; // skip delimiter and point to the next word
	}

	// match string to command
	for (i = 0; i < NUM_OF_COMMANDS; i++) {
		if (strstr(commandStr, allCommands[i])) {
			command = (ECommand)i;
			break;
		}
	}
	
	*out_arg1 = wordPtrs[1] ? stringToInt(wordPtrs[1]) : -1;
	*out_arg2 = wordPtrs[2] ? stringToInt(wordPtrs[2]) : -1;

	return command;
}

EResult executeCommand(LinkedList* list, ECommand command, int arg1, int arg2) {
	switch (command) {
	case ADD_END:
		add(list, arg1);
		return SUCCESS;

	case ADD_START:
		addToStart(list, arg1);
		return SUCCESS;

	case ADD_AFTER:
		if (addAfter(list, arg1, arg2) == FAIL) {
			printf("Error: Could not find %d.\n", arg2);
			return FAIL;
		}
		return SUCCESS;

	case INDEX:
		printf("%d\n", findIndex(list, arg1));
		return SUCCESS;

	case DEL:
		if (removeNode(list, arg1) == FAIL) {
			printf("Error: Index %d is too large.\n", arg1);
			return FAIL;
		}
		return SUCCESS;

	case PRINT:
		printList(list);
		return SUCCESS;

	case EXIT:
		// will cause abort and release of memory in caller function (main)
		return FAIL; 

	default: // handles UNDEFINED command as well
		printf("Error: This is awkward. Not a valid command.\n");
		return FAIL;
	}
}
