/* ~~~~~~~~~~~~~~~~~~~~ Includes ~~~~~~~~~~~~~~~~~~~~ */

#include "inputAnalyze.h"


/*  ~~~~~~~~~~~~~~~~~~~~ Methods ~~~~~~~~~~~~~~~~~~~~ */

Statement firstCheck(char*);

/* get the line and return the statement of the line */
Statement getLine(variables *variablesPtr)
{
    Statement state;
    size_t len;

    /* copies '\0' to the first LINE_LEN of the string pointed to, by the argument variablesPtr->line */
    memset(variablesPtr->line,'\0',LINE_LEN);
    /* reads a line from the specified stream and stores it into the string pointed to by variablesPtr->line */
    fgets(variablesPtr->line,LINE_LEN,variablesPtr->file);
    /* stores the length of variablesPtr->line in len */
    len = strlen(variablesPtr->line);

    /* check if the length of the line length is valid (80 chars exclude \n\0), if invalid go to the next line */
    if(len == LINE_LEN - 1 && variablesPtr->line[len-1] != '\0' && variablesPtr->line[len-1] != '\n')
    {
        advanceToNextLine(variablesPtr->file);
        return Invalid;
    }

    /* test the end of file indicator for the given stream. */
    if(feof(variablesPtr->file))
    { /* make sure if we have eof and the length is valid, to place \n at the end of the line */
        if(variablesPtr->line[80] == '\0')
        {
            variablesPtr->line[strlen(variablesPtr->line)] = '\n';
        }
    }

    state = firstCheck(variablesPtr->line);
    return state;
}

/* get to the next line of the file */
void advanceToNextLine(FILE *f)
{
    while(fgetc(f) != '\n');
}

/* return the statement of the sentence or invalid */
Statement firstCheck(char *str)
{
    char arr[STRING_PARTS][LINE_LEN];
    memset(arr, 0, sizeof(arr[0][0]) * STRING_PARTS * LINE_LEN);
    /* line too long */
    if (str[strlen(str) - 1] != '\n')
    {
        return Invalid;
    }
    /* the function copies the string pointed by strip(str) to the str. And then it returns the copied string. */
    strcpy(str,strip(str));
    /* check if the first char is \0 */
    if(str[0] == '\n' || str[0] == '\0')
    {
        return Empty;
    }
    /* check if the first char is ; */
    if(str[0] == ';')
    {
        return Comment;
    }

    if(split(str,".",arr) == DELIM_EXIST)
    {
        /* check if '.' is the first char, or it comes after whitespace or : 
           (after label and not inside string or a number) */
        if(!strcmp(arr[IMPORTANT],""))
            return Directive;
        if(isspace(arr[IMPORTANT][strlen(arr[IMPORTANT])-1]) || arr[IMPORTANT][strlen(arr[IMPORTANT]-1)] == ':')
            return Directive;
        /*return Directive;*/
    }
    return Instruction;
}

/* strip the string str from whitespaces and return the stripped string */
char *strip(char *str) {
    size_t size;
    char *copy = (char*) malloc(LINE_LEN);
    char *temp;
    char *end;
    temp = copy;
    memset(copy,'\0',LINE_LEN);
    strcpy(copy,str);
    /* if string empty, return it */
    size = strlen(copy);
    if (!size) {
        strcpy(str,copy);
        free(copy);
        return str;
    }

    /* remove all whitespaces from the end of the string */
    end = copy + size - 1;
    while(end >= copy && isspace(*end))
        --end;
    *(end + 1) = '\0';

    /* remove all whitespaces from the start of the string */
    while(copy && isspace(*copy))
        copy++;

    strcpy(str,copy);
    copy = temp;
    free(copy);
    return str;
}

/* split the string str by the delimeters delim
 * arr[0] will contain the string before the delim, and arr[1] will contain the string after
 * return if the delimeters exists, same as arr[1] = "" */
int split(char *str, char *delim, char arr[STRING_PARTS][LINE_LEN]) {
    char *tok;
    char strCopy[LINE_LEN];
    char temp[LINE_LEN];
    int i;

    strcpy(strCopy, str);

    if(!strcmp(strCopy,"")) { /* if the string is empty */
        strcpy(arr[IMPORTANT],"");
        strcpy(arr[REST],"");
        return DELIM_NOT_EXIST;
    }

    /* check if the delimeter on the first char */
    for(i=0;i<strlen(delim);i++) {
        if(str[0] == delim[i]) {
            strcpy(arr[IMPORTANT],"");
            strcpy(arr[REST],strCopy+1);
            return DELIM_EXIST;
        }
    }

    tok = strtok(strCopy, delim); /* look for the first token */
    if (tok != NULL && strlen(tok) == strlen(str)) { /* if there is no delim */
        strcpy(arr[IMPORTANT], str);
        strcpy(arr[REST], "");
        return DELIM_NOT_EXIST;
    }

    tok = strtok(NULL, delim); /* look for the next token */
    strcpy(arr[IMPORTANT], strCopy);
    strcpy(temp, (str + strlen(strCopy) + 1)); /* we want the rest of the string, and not until the next token */
    strcpy(arr[REST],temp);
    return DELIM_EXIST;
}

/* returns the opcode of the str, -1 if not an opcode */
int findOpcode(char *str) {
    char arr[STRING_PARTS][LINE_LEN];
    char oper[LINE_LEN];
    split(str," \t",arr);
    strcpy(oper,arr[IMPORTANT]);
    if(!strcmp(oper,"add") || !strcmp(oper,"sub")||!strcmp(oper,"and") || !strcmp(oper,"or")||!strcmp(oper,"nor"))
        return 0;
    if(!strcmp(oper,"move")||!strcmp(oper,"mvhi")||!strcmp(oper,"mvlo"))
        return 1;
    if(!strcmp(oper,"addi"))
        return 10;
    if(!strcmp(oper,"subi"))
        return 11;
    if(!strcmp(oper,"andi"))
        return 12;
    if(!strcmp(oper,"ori"))
        return 13;
    if(!strcmp(oper,"nori"))
        return 14;
    if(!strcmp(oper,"bne"))
        return 15;
    if(!strcmp(oper,"beq"))
        return 16;
    if(!strcmp(oper,"blt"))
        return 17;
    if(!strcmp(oper,"bgt"))
        return 18;
    if(!strcmp(oper,"lb"))
        return 19;
    if(!strcmp(oper,"sb"))
        return 20;
    if(!strcmp(oper,"lw"))
        return 21;
    if(!strcmp(oper,"sw"))
        return 22;
    if(!strcmp(oper,"lh"))
        return 23;
    if(!strcmp(oper,"sh"))
        return 24;
    if(!strcmp(oper,"jmp"))
        return 30;
    if(!strcmp(oper,"la"))
        return 31;
    if(!strcmp(oper,"call"))
        return 32;
    if(!strcmp(oper,"stop"))
        return 63;
    return -1;
}

int findNumberOfOperands(int opcode,int funct)
{
    if((opcode >= 0 && opcode <= 24) && opcode != 1)
        return 3;
    else if (opcode == 1)
        return 2;
    else if(opcode >= 30 && opcode <= 32)
        return 1;
    else
        return 0;
}


