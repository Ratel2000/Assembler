#ifndef INPUT_ANALYZE_H
#define INPUT_ANALYZE_H

#include "defaults.h"

#define IMPORTANT 0
#define REST 1
#define STRING_PARTS 2
#define DELIM_EXIST 0
#define DELIM_NOT_EXIST 1

Statement getLine(variables*);
char *strip(char*);
int split(char*,char*,char[STRING_PARTS][LINE_LEN]);
Status checkNum(char*);
DirectiveType checkLableIsDataWord(char *);
void checkSyntaxValidLabel(variables*,char*,Bool);
int findOpcode(char*);
int findReg(char*);
int findFunct(char*);
int findNumberOfLeters(int,int);
Type findEntryOrExternal(char*);
DirectiveType findDataType(char*);
int findFromEnd(char*,char);
char *findLabel(char*);

#endif
