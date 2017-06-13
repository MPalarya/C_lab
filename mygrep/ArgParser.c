#include "ArgParser.h"
#include <argp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool isValidANumber(char *arg)
{
  if (strcmp(arg, "0") == 0) {
    return true;
  }
  int num = atoi(arg);
  if (num <= 0) {
    return false;
  }
  return true;
}

int handleCaseA(char *arg, Arguments *arguments)
{
  arguments->arg_A_option.afterContextEnable = true;
  if (isValidANumber(arg)) {
    arguments->arg_A_option.numOfLinesToPrint = atoi(arg);
  } else {
    printf("mygrep: %s: invalid context length argument\n", arg);
    return 1;
  }
  return 0;
}

void handleCaseE(char *arg, Arguments *arguments)
{
  arguments->extendedRegexpEnable = true;
  arguments->expression = arg;
  arguments->match = (char *)malloc(sizeof(char) * (strlen(arg) + 1));
  memset(arguments->match, '\0', sizeof(char) * (strlen(arg) + 1));
}

void handleTooManyParameters(char *arg, Arguments *arguments, struct argp_state *state)
{
  if (state->arg_num >= 2 || (arguments->extendedRegexpEnable && state->arg_num >= 1)) {
    argp_usage(state);
  } else if (arguments->extendedRegexpEnable == false) {
    if (state->arg_num == 0) {
      arguments->expression = arg;
      arguments->match = (char *)malloc(sizeof(char) * (strlen(arg) + 1));
      memset(arguments->match, '\0', sizeof(char) * (strlen(arg) + 1));
    }

    if (state->arg_num == 1) {
      arguments->file_argument.filePathEnable = true;
      arguments->file_argument.filePath = arg;
    }
  } else {
    arguments->file_argument.filePathEnable = true;
    arguments->file_argument.filePath = arg;
  }
}

void handleTooFewParameters(Arguments *arguments, struct argp_state *state)
{
  if (state->arg_num < 1 && arguments->extendedRegexpEnable == false) {
    argp_usage(state);
  }
}

error_t parse_opt(int key, char *arg, struct argp_state *state)
{
  Arguments *arguments = state->input;
  bool result = 0;

  switch (key) {
    case 'A':
      result = handleCaseA(arg, arguments);
      break;
    case 'b':
      arguments->byteOffsetEnable = true;
      break;
    case 'E':
      handleCaseE(arg, arguments);
      break;
    case 'c':
      arguments->countEnable = true;
      break;
    case 'i':
      arguments->caseInsensitiveEnable = true;
      break;
    case 'n':
      arguments->lineNumberEnable = true;
      break;
    case 'v':
      arguments->invertMatchEnable = true;
      break;
    case 'x':
      arguments->lineRegexpEnable = true;
      break;
    case ARGP_KEY_ARG:
      handleTooManyParameters(arg, arguments, state);
      break;
    case ARGP_KEY_END:
      handleTooFewParameters(arguments, state);
      break;
    default:
      return ARGP_ERR_UNKNOWN;
  }
  return result;
}

void init_argp(Arguments *arguments)
{
  arguments->arg_A_option.afterContextEnable = false;
  arguments->arg_A_option.numOfLinesToPrint = 0;
  arguments->byteOffsetEnable = false;
  arguments->countEnable = false;
  arguments->caseInsensitiveEnable = false;
  arguments->lineNumberEnable = false;
  arguments->invertMatchEnable = false;
  arguments->lineRegexpEnable = false;
  arguments->extendedRegexpEnable = false;
  arguments->file_argument.filePathEnable = false;
  arguments->file_argument.filePath = NULL;
  arguments->expression = NULL;
  arguments->match = NULL;
}

int parseArgs(int argc, char **argv, Arguments *arguments)
{
  char args_doc[] = "[FILENAME]...";
  struct argp_option options[] = {
      {"after-context", 'A', "NUM", 0, "print NUM lines of trailing context", 0},
      {"byte-offset", 'b', 0, 0, "print the byte offset with output lines", 0},
      {"count", 'c', 0, 0, "print only a count of matching lines per FILE", 0},
      {"extended-regexp", 'E', "REGEXP", 0, "PATTERN is an extended regular expression (ERE)", 0},
      {"ignore-case", 'i', 0, 0, "ignore case distinctions", 0},
      {"line-number", 'n', 0, 0, "print line number with output lines", 0},
      {"invert-match", 'v', 0, 0, "select non-matching lines", 0},
      {"line-regexp", 'x', 0, 0, "force PATTERN to match only whole lines", 0},
      {0}};

  struct argp argp = {options, parse_opt, args_doc, 0, 0, 0, 0};

  init_argp(arguments);

  if (argp_parse(&argp, argc, argv, 0, 0, arguments) == 1) {
    return 1;
  }
  return 0;
}
