#ifndef INPUT_ANALYZE_H
#define INPUT_ANALYZE_H

/* ~~~~~~~~~~~~~~~~~ Includes ~~~~~~~~~~~~~~~~~ */

#include "defaults.h"


/* ~~~~~~~~~~~~~~~~~~ Macros ~~~~~~~~~~~~~~~~~~ */

#define FIRST 0
#define SECEND 1
#define THERED 2
#define STRING_PARTS_3 3
#define STRING_PARTS_2 2
#define DELIM_EXIST 0
#define DELIM_NOT_EXIST 1


/* ~~~~~~~~~~~~~~ Directives List ~~~~~~~~~~~~~~ */

Statement getLine(variables *);

Status checkNum(char *);

Type findEntryOrExternal(char *);

DirectiveType checkLableIsDataWord(char *);

DirectiveType findDataType(char *);

void advanceToNextLine(FILE*);

void checkSyntaxValidLabel(variables *, char *, Bool);

int split(char *, char *, char[STRING_PARTS_2][LINE_LEN]);

int split3(char *, char *, char[STRING_PARTS_3][LINE_LEN]);

int findOpcode(char *);

int findReg(char *);

int findFunct(char *);

int findFromEnd(char *, char);

int findNumberOfLeters(int, int);

char *findLabel(char *);

char *strip(char *);



#endif
