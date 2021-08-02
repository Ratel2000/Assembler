/* ~~~~~~~~~~~~~~~~~~~ Includes ~~~~~~~~~~~~~~~~~~~ */

#include "firstPass.h"
#include "defaults.h"


/* ~~~~~~~~~~~~~~ Methods Declaration ~~~~~~~~~~~~~~ */

void handleString(char *, variables *);

void handleData(char *, variables *);

void handleInstruction(variables *, Word *);

void fillTwoOperands(char *, Word *, variables *);

void fillOneOperand(char *, Word *, variables *);

void handleDirective(variables *, Word *);

void addLabel(variables *, Location);

void checkLabel(variables *, char *, Type);

void addNumberWord(variables *, char *);

void addStringWord(variables *, char);

void updateTables(variables *);

void addEmptyWord(variables *);


/* ~~~~~~~~~~~~~~~~~~~ Methods ~~~~~~~~~~~~~~~~~~~~~ */

/* Handle the first pass of the assembler */
void firstPass(variables *variablesPtr) {
    Statement state;
    Word word;
    variablesPtr->lineCounter = 0;

    /* while its not the end-of-file indicator for variablesPtr->file. */
    while (!feof(variablesPtr->file)) {
        state = getLine(variablesPtr); /* initiate state with the statement of the line */
        variablesPtr->lineCounter++;
        /* the function copies the string pointed by strip(variablesPtr->line) to the variablesPtr->line. 
           And then it returns the copied string. */
        strcpy(variablesPtr->line, strip(variablesPtr->line));
        defaultValues(variablesPtr);

        /* classify which statement is the line */

        if (state == Invalid) {
            variablesPtr->status = LineTooLong;
        }
        if (state == Empty || state == Comment) {
            continue;
        }
        if (state == Instruction) {
            handleInstruction(variablesPtr, &word);
        }
        if (state == Directive) {
            handleDirective(variablesPtr, &word);
        }
        if (variablesPtr->status != Valid) {
            variablesPtr->foundError = True;
        }

        if (feof(variablesPtr->file)) {
            printError(variablesPtr);
            break;
        }
    }

    if (!variablesPtr->foundError) {
        updateTables(variablesPtr);
    }
}


/*
 * Handle the instruction statement
 * Fill the word, update the linked list and do anything it needs
 */
void handleInstruction(variables *variablesPtr, Word *wordPtr) {
    /* Allocation of memory using malloc */
    char *lineCopy = (char *) malloc(LINE_LEN);
    /* Variable initialization */
    char *temp = lineCopy;
    char *back;
    /* the function copies the string pointed by variablesPtr->line to the lineCopy.
       and also returns the copied string. */
    strcpy(lineCopy, variablesPtr->line);

    back = findLabel(lineCopy);     /* find the label */
    /* the function copies the string pointed by 'back' to the variablesPtr.
       and also returns the copied string. */
    strcpy(variablesPtr->label, back);
    free(back);      /* releases the memory from 'back' */

    /* checks if both equal by calculating the length of a given string. */
    if (strlen(variablesPtr->label) == strlen(variablesPtr->line) - 1) {
        variablesPtr->status = MissingOperation;
        return;
    }

    /* this function compare variablesPtr->label to " ", If the strings are equal, the function returns 0. */
    if (strcmp(variablesPtr->label, "")) {
        checkSyntaxValidLabel(variablesPtr, variablesPtr->label, True);

        /* Check integrity */
        if (variablesPtr->status != Valid) {
            return;
        }

        checkLabel(variablesPtr, variablesPtr->label, NoneEntOrExt);

        /* Check integrity */
        if (variablesPtr->status != Valid) {
            return;
        }

        addLabel(variablesPtr, CodeImage);
        /* store and add the length of (variablesPtr->label) in lineCopy, and also add yourself */
        lineCopy += strlen(variablesPtr->label) + 1;
        /* the function copies the string pointed by  strip(lineCopy) to the lineCopy. 
           And then it returns the copied string. */
        strcpy(lineCopy, strip(lineCopy));
    }

    /* Check integrity */
    if (variablesPtr->status != Valid)
    {
        return;
    }

    /* find the opcode and funct */
    wordPtr->code.opcode = findOpcode(lineCopy);
    wordPtr->code.funct = findFunct(lineCopy);
    wordPtr->code.type = findTypeOfInstruction(lineCopy);

    /* if its the 'stop' gerund */
    if (wordPtr->code.opcode == MINUS1_6_BITS)
    {
        variablesPtr->status = UnknownOperation;
        return;
    }

    /* go to the next char after the operation */
    lineCopy += findNumberOfLeters(wordPtr->code.opcode, wordPtr->code.funct); 
    /* the function copies the string pointed by strip(lineCopy) to the lineCopy. 
       And then it returns the copied string. */
    strcpy(lineCopy, strip(lineCopy));

    /* Check the opcode */
    
    if (findNumberOfOperands(wordPtr->code.opcode, wordPtr->code.funct) == 3) 
    {
        fillTreeOperands(lineCopy, wordPtr, variablesPtr);
    }
    else if (findNumberOfOperands(wordPtr->code.opcode, wordPtr->code.funct) == 2) 
    {
        fillTwoOperands(lineCopy, wordPtr, variablesPtr);
    }
    else if (findNumberOfOperands(wordPtr->code.opcode, wordPtr->code.funct) == 1) 
    {
        fillOneOperand(lineCopy, wordPtr, variablesPtr);
    }
    else if (wordPtr->code.opcode != 63)
        /*add stop command work*/
            else
    if (wordPtr->code.opcode < 1 && wordPtr->code.opcode > 63) { /* operation has no operands */
        if (strcmp(lineCopy, "")) /* if there is text left raise error */
            variablesPtr->status = TextAfterCommand;
        else {
            wordPtr->code.opcode = 63;
            wordPtr->code.address = 0;
            wordPtr->code.reg = 0;

            variablesPtr->IC = IC + 4;
            variablesPtr->status = Valid;
        }
    }


    lineCopy = temp;
    free(lineCopy);
}

/* this function handles the instruction commands with 3 operands and fills the word if there are no errors STRING_PARTS=3,REST=3,*/
void fillTreeOperands(char *str, Word *word, variables *variablesPtr) {
    char arr[REST][LINE_LEN];
    char checkExtraOperand[STRING_PARTS][LINE_LEN];
    int op1, op2, op3;
    int tokExist;
    memset(arr, '\0', sizeof(arr[0][0]) * STRING_PARTS * LINE_LEN);
    memset(checkExtraOperand, '\0', sizeof(checkExtraOperand[0][0]) * STRING_PARTS * LINE_LEN);

    tokExist = split(str, ",", arr); /* split the two operands */
    strcpy(arr[FIRST], strip(arr[FIRST]));/*clear from spaces*/
    strcpy(arr[SECOND], strip(arr[SECOND]));
    strcpy(arr[REST], strip(arr[REST]));

    if (tokExist == DELIM_NOT_EXIST) { /* if there is no comma */
        if (!strcmp(arr[FIRST], "")) { /* if the part before the comma empty - no operands */
            variablesPtr->status = NoOperands;
        } else /* there is only one operand */
        {
            variablesPtr->status = MissingOperand;
        }
    } else { /* if there is a comma */
        if (!strcmp(arr[FIRST], "") && !strcmp(arr[SECOND], "") &&
            !strcmp(arr[REST], "") /* before and after the comma empty - no operands*/
            variablesPtr->status = NoOperands;
                else if (!strcmp(arr[FIRST], "") || !strcmp(arr[SECOND], "") ||
                         !strcmp(arr[REST], "") /* before or after the comma empty - no operands */
            variablesPtr->status = MissingOperand;
    }

    if (split(arr[REST], ",", checkExtraOperand) == DELIM_EXIST) { /* split with the next comma, if there are more */
        if (!strcmp(checkExtraOperand[REST], "")) /* if the part after the comma is empty we have extra comma */
            variablesPtr->status = ExtraComma;
        else if (!strcmp(checkExtraOperand[FIRST], "")) /* if the part before the comma is empty we have extra comma */
            variablesPtr->status = ExtraComma;
        else if (!strcmp(checkExtraOperand[SECOND], "")) /* if the part before the comma is empty we have extra comma */
            variablesPtr->status = ExtraComma;
        else
            variablesPtr->status = ExtraOperand; /* we have an extra operand */
        return;
    }

    if (variablesPtr->status != Valid)
        return;

    op1 = findReg(arr[FIRST]);
    op2 = findReg(arr[SECOND]);
    op3 = findReg(arr[REST]);

    if (variablesPtr->status != Valid) {
        return;
    }
    if (op1 == -1 || op2 == -1 || op3 == -1) {
        variablesPtr->status = InvalidOperand;
        return;
    }

    word->code.InstructionType = R;
    word->code.rd = op1;
    word->code.rs = op2;
    word->code.rt = op3;
    word->code.unused = 0;

    addWordToImage(&variablesPtr->codeHptr, *word, variablesPtr->IC);
    variablesPtr->IC = IC + 4;

    variablesPtr->status = Valid;
}

/* this function handles the instruction commands with 2 operands and fills the word if there are no errors */
void fillTwoOperands(char *str, Word *word, variables *variablesPtr) {
    char arr[STRING_PARTS][LINE_LEN];
    char checkExtraOperand[STRING_PARTS][LINE_LEN];
    int op1, op2;
    int tokExist;
    memset(arr, '\0', sizeof(arr[0][0]) * STRING_PARTS * LINE_LEN);
    memset(checkExtraOperand, '\0', sizeof(checkExtraOperand[0][0]) * STRING_PARTS * LINE_LEN);

    tokExist = split(str, ",", arr); /* split the two operands */
    strcpy(arr[IMPORTANT], strip(arr[IMPORTANT]));
    strcpy(arr[REST], strip(arr[REST]));

    if (tokExist == DELIM_NOT_EXIST) { /* if there is no comma */
        if (!strcmp(arr[IMPORTANT], "")) /* if the part before the comma empty - no operands */
            variablesPtr->status = NoOperands;
        else /* there is only one operand */
            variablesPtr->status = MissingOperand;
    } else { /* if there is a comma */
        if (!strcmp(arr[IMPORTANT], "") && !strcmp(arr[REST], "")) /* before and after the comma empty - no operands*/
            variablesPtr->status = NoOperands;
        else if (!strcmp(arr[IMPORTANT], "") ||
                 !strcmp(arr[REST], "")) /* before or after the comma empty - no operands */
            variablesPtr->status = MissingOperand;
    }

    if (split(arr[REST], ",", checkExtraOperand) == DELIM_EXIST) { /* split with the next comma, if there are more */
        if (!strcmp(checkExtraOperand[REST], "")) /* if the part after the comma is empty we have extra comma */
            variablesPtr->status = ExtraComma;
        else if (!strcmp(checkExtraOperand[IMPORTANT],
                         "")) /* if the part before the comma is empty we have extra comma */
            variablesPtr->status = ExtraComma;
        else
            variablesPtr->status = ExtraOperand; /* we have an extra operand */
        return;
    }

    if (variablesPtr->status != Valid)
        return;

    op1 = findReg(arr[FIRST]);
    op2 = findReg(arr[SECOND]);

    if (variablesPtr->status != Valid)
        return;

    if (op1 == -1 || op2 == -1) {
        variablesPtr->status = InvalidOperand;
        return;
    }

    word->code.type = R;
    word->code.rd = op1;
    word->code.rs = op2;
    word->code.rt = 0;
    word->code.unused = 0;

    addWordToImage(&variablesPtr->codeHptr, *word, variablesPtr->IC);
    variablesPtr->IC = +4;

    variablesPtr->status = Valid;
}
