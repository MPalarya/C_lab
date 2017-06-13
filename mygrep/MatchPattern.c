#include "MatchPattern.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define LEFT_ROUND '('
#define RIGHT_ROUND ')'
#define LEFT_SQUARE '['
#define BAR '|'
#define DOT '.'
#define BACKSLASH '\\'
#define MIN_CHAR 33
#define MAX_CHAR 126

bool isMatchRegularExpression(Arguments *arguments, char **line, int regexIndex, int matchIndex);

void toLowercase(char *str)
{
  for (; *str; str++) {
    *str = tolower(*str);
  }
}

bool isMatch(Arguments *arguments, char **line)
{
  bool res;

  char **newline = (char **)malloc(sizeof(char *));
  *newline = (char *)malloc(sizeof(char) * (strlen(*line) + 1));

  strncpy(*newline, *line, sizeof(char) * (strlen(*line) + 1));
  (*newline)[strlen(*line) - 1] = '\0';

  if (arguments->caseInsensitiveEnable) {
    toLowercase(arguments->expression);
    toLowercase(*newline);
  }

  if (arguments->extendedRegexpEnable) {
    res = isMatchRegularExpression(arguments, newline, 0, 0);
    free(*newline);
    free(newline);
    return res;
  }

  if (arguments->lineRegexpEnable) {
    res = (strcmp(*newline, arguments->expression) == 0);
  }

  else {
    res = (strstr(*newline, arguments->expression) != NULL);
  }

  free(*newline);
  free(newline);
  return arguments->invertMatchEnable ? !res : res;
}

bool checkDot(Arguments *arguments, char **line, int regexIndex, int matchIndex)
{
  char c;
  for (c = MIN_CHAR; c <= MAX_CHAR; c++) {
    arguments->match[matchIndex] = c;
    if (isMatchRegularExpression(arguments, line, regexIndex + 1, matchIndex + 1)) {
      return true;
    }
  }
  return false;
}

bool checkSquareBrackets(Arguments *arguments, char **line, int regexIndex, int matchIndex)
{
  char c;
  char from = arguments->expression[regexIndex + 1];  // [x <-
  char to = arguments->expression[regexIndex + 3];    // [x-y <-

  for (c = from; c <= to; c++) {
    arguments->match[matchIndex] = c;
    if (isMatchRegularExpression(arguments, line, regexIndex + 5, matchIndex + 1)) {
      return true;
    }
  }
  return false;
}

bool checkRoundBrackets(Arguments *arguments, char **line, int regexIndex, int matchIndex)
{
  int tmpMatchIndex, endBracketsIndex;
  char *xpression = arguments->expression;
  char *match = arguments->match;
  bool emptyStringChecked = false;

  // find index right after the right bracket (..)_<-
  for (endBracketsIndex = regexIndex; xpression[endBracketsIndex] != RIGHT_ROUND; endBracketsIndex++) {
  }
  endBracketsIndex++;
  regexIndex++;  // skip left bracket

  while (xpression[regexIndex] != RIGHT_ROUND) {
    tmpMatchIndex = matchIndex;  // to not modify original index

    // copy string from pointer to next '|' or ')'
    while (xpression[regexIndex] != BAR && xpression[regexIndex] != RIGHT_ROUND) {
      match[tmpMatchIndex] = xpression[regexIndex];
      tmpMatchIndex++;
      regexIndex++;
    }

    if (isMatchRegularExpression(arguments, line, endBracketsIndex, tmpMatchIndex)) {
      return true;
    }

    // '(|' case is handled by loop not occouring
    if (tmpMatchIndex == matchIndex) {
      emptyStringChecked = true;
    }

    // handle the case of '|)':
    if (!emptyStringChecked && xpression[regexIndex] == BAR && xpression[regexIndex + 1] == RIGHT_ROUND) {
      emptyStringChecked = true;
      if (isMatchRegularExpression(arguments, line, endBracketsIndex, matchIndex)) {
        return true;
      }
    }

    // '||' case is handled by advancing counter and not entering loop
    if (xpression[regexIndex] != RIGHT_ROUND) {
      regexIndex++;
    }
  }
  return false;
}

bool isMatchRegularExpression(Arguments *arguments, char **line, int regexIndex, int matchIndex)
{
  bool res;

  for (; (unsigned)regexIndex < strlen(arguments->expression); regexIndex++) {
    switch (arguments->expression[regexIndex]) {
      case LEFT_SQUARE:
        return checkSquareBrackets(arguments, line, regexIndex, matchIndex);

      case LEFT_ROUND:
        return checkRoundBrackets(arguments, line, regexIndex, matchIndex);

      case DOT:
        return checkDot(arguments, line, regexIndex, matchIndex);

      case BACKSLASH:
        regexIndex++;  // skip the backslash
        arguments->match[matchIndex] = arguments->expression[regexIndex];
        matchIndex++;
        break;

      default:  // copy character as-is
        arguments->match[matchIndex] = arguments->expression[regexIndex];
        matchIndex++;
        break;
    }
  }
  arguments->match[matchIndex] = '\0';

  if (arguments->lineRegexpEnable) {
    res = (strcmp(*line, arguments->match) == 0);
  } else {
    res = (strstr(*line, arguments->match) != NULL);
  }

  return arguments->invertMatchEnable ? !res : res;
}
