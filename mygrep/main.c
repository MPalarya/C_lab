#define _GNU_SOURCE

#include "MatchPattern.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void getFileDescriptor(FILE **fp, Arguments *arguments)
{
  *fp = stdin;

  if (arguments->file_argument.filePathEnable) {
    *fp = fopen(arguments->file_argument.filePath, "r");

    if (*fp == NULL) {
      printf("opening file failed: %s\n", strerror(errno));
      exit(EXIT_FAILURE);
    }
  }
}

bool isMatchInLine(char **line, Arguments *arguments)
{
  char *matchInLine = NULL;

  if (arguments->lineRegexpEnable || arguments->extendedRegexpEnable) {
    return isMatch(arguments, line);
  }

  if (arguments->caseInsensitiveEnable) {
    matchInLine = strcasestr(*line, arguments->expression);
  }

  else {
    matchInLine = strstr(*line, arguments->expression);
  }

  if (arguments->invertMatchEnable) {
    return matchInLine == NULL;
  }

  return matchInLine != NULL;
}

void printPrefix(char **line, Arguments *arguments, int lineNumber, int byteOffset, char separator)
{
  if (arguments->lineNumberEnable) {
    printf("%d%c", lineNumber, separator);
  }

  if (arguments->byteOffsetEnable) {
    printf("%d%c", byteOffset, separator);
  }

  printf("%s", *line);
}

void matchPrinter(bool toPrint, char **line, Arguments *arguments, int lineNumber, int byteOffset)
{
  static int printLineAfter = 0;
  static bool printAfterEnable = false;

  if (!arguments->countEnable) {
    if (toPrint) {
      printLineAfter = 0;
      printPrefix(line, arguments, lineNumber, byteOffset, ':');
      printAfterEnable = true;
    } else if (printAfterEnable && arguments->arg_A_option.afterContextEnable &&
               printLineAfter <= arguments->arg_A_option.numOfLinesToPrint) {
      printPrefix(line, arguments, lineNumber, byteOffset, '-');
    } else {
      printAfterEnable = false;
      return;
    }
    printLineAfter++;
  }
}

void freeAllocatedMemory(FILE **fp, char **line, Arguments *arguments)
{
  if (*fp != NULL) {
    fclose(*fp);
    *fp = NULL;
  }
  if (*line != NULL) {
    free(*line);
    *line = NULL;
  }
  if (arguments != NULL && arguments->match != NULL) {
    free(arguments->match);
    arguments->match = NULL;
  }
}

int main(int argc, char **argv)
{
  Arguments arguments;
  FILE *fp = NULL;
  char *line = NULL;
  size_t len = 0;
  ssize_t read = 0;
  bool matchFound = false;
  int matchCounter = 0, lineNumber = 1, byteOffset = 0;

  if (parseArgs(argc, argv, &arguments) != 0) {
    printf("Error Parsing");
    freeAllocatedMemory(&fp, &line, &arguments);
    exit(EXIT_FAILURE);
  }

  getFileDescriptor(&fp, &arguments);

  while ((read = getline(&line, &len, fp)) != -1) {
    matchFound = isMatchInLine(&line, &arguments);
    if (matchFound) {
      matchCounter++;
    }
    matchPrinter(matchFound, &line, &arguments, lineNumber, byteOffset);
    lineNumber++;
    byteOffset += strlen(line);
  }

  if (arguments.countEnable) {
    printf("%d\n", matchCounter);
  }

  freeAllocatedMemory(&fp, &line, &arguments);
  exit(EXIT_SUCCESS);
}
