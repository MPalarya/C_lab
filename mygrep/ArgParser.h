#ifndef ARGPARSER_H_
#define ARGPARSER_H_
#include <stdbool.h>

typedef struct arguments {

  struct arg_A_option {
    bool afterContextEnable;
    int numOfLinesToPrint;
  } arg_A_option;

  bool byteOffsetEnable;
  bool countEnable;
  bool extendedRegexpEnable;
  bool caseInsensitiveEnable;
  bool lineNumberEnable;
  bool invertMatchEnable;
  bool lineRegexpEnable;

  char *expression;
  char *match;

  struct file_argument {
    bool filePathEnable;
    char *filePath;
  } file_argument;

} Arguments;

int parseArgs(int argc, char **argv, Arguments *arguments);

#endif /* ARGPARSER_H_ */
