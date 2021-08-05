
#include "firstPass.h"
#include "defaults.h"

void fillR(char *,Word *, variables *);
void fillI(char *,Word *, variables *);
void fillTreeOperandsR(char *,Word *, variables *);
void fillTwoOperandsR(char *,Word *, variables *);
void fillOneOperandJ(char *,Word *, variables *);

void handleString(char*,variables*);
void handleData(char*,variables*);

void handleInstruction(variables*,Word*);
void handleDirective(variables*,Word*);
void addLabel(variables*,Location);
void checkLabel(variables*,char*,Type);
void addNumberWord(variables*,char*);
void addStringWord(variables*,char);
void updateTables(variables*);
void addEmptyWord(variables*);

/* Handle the first pass of the assembler */
void firstPass(variables *variablesPtr) {
    Statement state;
    Word word;
    variablesPtr->lineCounter=0;

    while(!feof(variablesPtr->file)) {
        state = getLine(variablesPtr);
        variablesPtr->lineCounter++;
        strcpy(variablesPtr->line,strip(variablesPtr->line));
        defaultValues(variablesPtr);

        /* classify which statement is the line */

        if(state == Invalid)
            variablesPtr->status = LineTooLong;

        if(state == Empty || state == Comment)
            continue;

        if(state == Instruction)
            handleInstruction(variablesPtr,&word);

        if(state == Directive)
            handleDirective(variablesPtr,&word);

        if(variablesPtr->status != Valid)
            variablesPtr->foundError = True;

        printError(variablesPtr);

        if(feof(variablesPtr->file))
            break;
    }

    if(!variablesPtr->foundError)
        updateTables(variablesPtr);
}

/* Handle the instruction statement
 * Fill the word, update the linked list and do anything it needs
*/
void handleInstruction(variables *variablesPtr,Word *wordPtr) {
    char *lineCopy = (char*) malloc(LINE_LEN);
    char *temp = lineCopy;
    char *back;
    strcpy(lineCopy,variablesPtr->line);

    /* find the label */
    back = findLabel(lineCopy);
    strcpy(variablesPtr->label,back);
    free(back);
    if(strlen(variablesPtr->label) == strlen(variablesPtr->line)-1) {
        variablesPtr->status = MissingOperation;
        return;
    }
    if(strcmp(variablesPtr->label,"")) {
        checkSyntaxValidLabel(variablesPtr,variablesPtr->label,True);
        if(variablesPtr->status != Valid)
            return;

        checkLabel(variablesPtr,variablesPtr->label,NoneEntOrExt);
        if(variablesPtr->status != Valid)
            return;

        addLabel(variablesPtr, CodeImage);
        lineCopy+=strlen(variablesPtr->label)+1;
        strcpy(lineCopy,strip(lineCopy));
    }

    if(variablesPtr->status != Valid)
        return;

    /* find the opcode and funct */
    wordPtr->code.opcode = findOpcode(lineCopy);
    wordPtr->code.funct = findFunct(lineCopy);
    wordPtr->code.type = findTypeOfInstruction(lineCopy);

    if(wordPtr->code.opcode == MINUS1_6_BITS) {
        variablesPtr->status = UnknownOperation;
        return;
    }

    lineCopy += findNumberOfLeters(wordPtr->code.opcode,wordPtr->code.funct); /* go to the next char after the operation */
    strcpy(lineCopy,strip(lineCopy));
    if(wordPtr->code.type == R)
        fillR(lineCopy,wordPtr,variablesPtr);
    else if(wordPtr->code.type == I)
        fillI(lineCopy,wordPtr,variablesPtr);
    else if(wordPtr->code.type == J && wordPtr->code.opcode!=63)
        fillOneOperandJ(lineCopy,wordPtr,variablesPtr);


    else { /* operation has no operands (stop)*/
        if(strcmp(lineCopy,"")) /* if there is text left raise error */
            variablesPtr->status = TextAfterCommand;
        else {
            wordPtr->code.InstructionType = J;
            wordPtr->code.opcode = 63;
            wordPtr->code.address = 0;
            wordPtr->code.reg= 0;

            variablesPtr->IC=IC+4;
            variablesPtr->status = Valid;
        }
    }

    lineCopy = temp;
    free(lineCopy);
}



void fillR(char *str, Word *word, variables *variablesPtr){
    if(wordPtr->code.opcode==1 )
        fillTwoOperandsR(str,word,variablesPtr);
    if(wordPtr->code.opcode==0)
        fillTreeOperandsR(str,word,variablesPtr);
}

/* this function handles the instruction I commands with 3 operands and fills the word if there are no errors*/
void fillI(char *str, Word *word, variables *variablesPtr)
{
    char arr[STRING_PARTS_3][LINE_LEN];
    char checkExtraOperand[STRING_PARTS_3][LINE_LEN];
    int op1,op2,op3;
    int tokExist;
    memset(arr,'\0',sizeof(arr[0][0])*STRING_PARTS_3*LINE_LEN);
    memset(checkExtraOperand,'\0',sizeof(checkExtraOperand[0][0])*STRING_PARTS*LINE_LEN);

    tokExist = split3(str,",",arr); /* split the 3 operands*/
    strcpy(arr[FIRST],strip(arr[FIRST]));/*clear from spaces*/
    strcpy(arr[SECEND],strip(arr[SECEND]));
    strcpy(arr[THERED],strip(arr[THERED]));

    if(tokExist == DELIM_NOT_EXIST) { /* if there is no comma */
        if(!strcmp(arr[FIRST],"")) /* if the part before the comma empty - no operands */
            variablesPtr->status = NoOperands;
        else /* there is only one operand */
            variablesPtr->status = MissingOperand;
    }
    else { /* if there is a comma */
        if(!strcmp(arr[FIRST],"") && !strcmp(arr[SECEND],"") && !strcmp(arr[THERED],"") /* before and after the comma empty - no operands*/
            variablesPtr->status = NoOperands;
                else if(!strcmp(arr[FIRST],"") || !strcmp(arr[SECEND],"")||!strcmp(arr[THERED],"") /* before or after the comma empty - no operands */
            variablesPtr->status = MissingOperand;
    }

    if(split(arr[SECEND],",",checkExtraOperand) == DELIM_EXIST) { /* split with the next comma, if there are more */
        if(!strcmp(checkExtraOperand[THERED],"")) /* if the part after the comma is empty we have extra comma*/
            variablesPtr->status = ExtraComma;
        else if(!strcmp(checkExtraOperand[FIRST],"")) /* if the part before the comma is empty we have extra comma */
            variablesPtr->status = ExtraComma;
        else if(!strcmp(checkExtraOperand[SECEND],"")) /* if the part before the comma is empty we have extra comma */
            variablesPtr->status = ExtraComma;
        else
            variablesPtr->status = ExtraOperand; /* we have an extra operand */
        return;
    }

    if(variablesPtr->status != Valid)
        return;

    if(word->code.opcode>=10&&word->code.opcode<=14||word->code.opcode>=19&&word->code.opcode<=24) {

        if (checkNum(arr[SECEND]) != Valid) {
            variablesPtr->status=InvalidNumber;
            return;
        }

        op1 = findReg(arr[FIRST]);
        op2 = atoi(arr[SECEND]);
        op3 = findReg(arr[THERED]);


        if (op1 == -1 || op3 == -1) {
            variablesPtr->status = InvalidOperand;
            return;
        }

        word->code.InstructionType = I;
        word->code.rs = op1;
        word->code.immed = op2;
        word->code.rt = op3;
        word->code.unused = 0;
    }
    else if(word->code.opcode>=15&&word->code.opcode<=18) {

        if (checkNum(arr[SECOND]) != Valid) {
            variablesPtr->status=InvalidNumber;
            return;
        }

        op1 = findReg(arr[FIRST]);
        op2 = findReg(arr[SECEND]);


        if (op1 == -1 || op2 == -1) {
            variablesPtr->status = InvalidOperand;
            return;
        }
        word->code.InstructionType = I;
        word->code.rs = op1;
        word->code.rt = op2;
        word->code.unused = 0;
        word->code.checkOnSecondPass = True;

    }


    addWordToImage(&variablesPtr->codeHptr,*word,variablesPtr->IC);
    variablesPtr->IC=IC+4;

    variablesPtr->status = Valid;
}


/* this function handles the instruction R commands with 3 operands and fills the word if there are no errors*/
void fillTreeOperandsR(char *str, Word *word, variables *variablesPtr)
{
    char arr[REST][LINE_LEN];
    char checkExtraOperand[STRING_PARTS_3][LINE_LEN];
    int op1,op2,op3;
    int tokExist;
    memset(arr,'\0',sizeof(arr[0][0])*STRING_PARTS*LINE_LEN);
    memset(checkExtraOperand,'\0',sizeof(checkExtraOperand[0][0])*STRING_PARTS*LINE_LEN);

    tokExist = split(str,",",arr); /* split the two operands */
    strcpy(arr[FIRST],strip(arr[FIRST]));/*clear from spaces*/
    strcpy(arr[SECEND],strip(arr[SECEND]));
    strcpy(arr[THERED],strip(arr[THERED]));

    if(tokExist == DELIM_NOT_EXIST) { /* if there is no comma */
        if(!strcmp(arr[FIRST],"")) /* if the part before the comma empty - no operands */
            variablesPtr->status = NoOperands;
        else /* there is only one operand */
            variablesPtr->status = MissingOperand;
    }
    else { /* if there is a comma */
        if(!strcmp(arr[FIRST],"") && !strcmp(arr[SECEND],"") && !strcmp(arr[THERED],"") /* before and after the comma empty - no operands*/
            variablesPtr->status = NoOperands;
        else if(!strcmp(arr[FIRST],"") || !strcmp(arr[SECEND],"")||!strcmp(arr[THERED],"") /* before or after the comma empty - no operands */
            variablesPtr->status = MissingOperand;
    }

    if(split(arr[THERED],",",checkExtraOperand) == DELIM_EXIST) { /* split with the next comma, if there are more */
        if(!strcmp(checkExtraOperand[THERED],"")) /* if the part after the comma is empty we have extra comma */
            variablesPtr->status = ExtraComma;
        else if(!strcmp(checkExtraOperand[FIRST],"")) /* if the part before the comma is empty we have extra comma */
            variablesPtr->status = ExtraComma;
        else if(!strcmp(checkExtraOperand[SECEND],"")) /* if the part before the comma is empty we have extra comma */
            variablesPtr->status = ExtraComma;
        else
            variablesPtr->status = ExtraOperand; /* we have an extra operand */
        return;
    }

    if(variablesPtr->status != Valid)
        return;

    op1 = findReg(arr[FIRST]);
    op2 = findReg(arr[SECEND]);
    op3 = findReg(arr[THERED]);

    if(variablesPtr->status != Valid)
        return;

    if(op1 == -1 || op2 == -1|| op3 == -1) {
        variablesPtr->status = InvalidOperand;
        return;
    }

    word->code.InstructionType=R;
    word->code.rd=op1;
    word->code.rs = op2;
    word->code.rt = op3;
    word->code.unused=0;

    /*addWordToImage(&variablesPtr->codeHptr,*word,variablesPtr->IC);*/
    variablesPtr->IC=IC+4;

    variablesPtr->status = Valid;
}
void fillTwoOperandsR(char *str, Word *word, variables *variablesPtr)
{
    char arr[STRING_PARTS][LINE_LEN];
    char checkExtraOperand[STRING_PARTS][LINE_LEN];
    int op1,op2;
    int tokExist;
    memset(arr,'\0',sizeof(arr[0][0])*STRING_PARTS*LINE_LEN);
    memset(checkExtraOperand,'\0',sizeof(checkExtraOperand[0][0])*STRING_PARTS*LINE_LEN);

    tokExist = split(str,",",arr); /* split the two operands */
    strcpy(arr[FIRST],strip(arr[FIRST]));
    strcpy(arr[SECEND],strip(arr[SECEND]));

    if(tokExist == DELIM_NOT_EXIST) { /* if there is no comma */
        if(!strcmp(arr[FIRST],"")) /* if the part before the comma empty - no operands */
            variablesPtr->status = NoOperands;
        else /* there is only one operand */
            variablesPtr->status = MissingOperand;
    }
    else { /* if there is a comma */
        if(!strcmp(arr[FIRST],"") && !strcmp(arr[SECEND],"")) /* before and after the comma empty - no operands*/
            variablesPtr->status = NoOperands;
        else if(!strcmp(arr[FIRST],"") || !strcmp(arr[SECEND],"")) /* before or after the comma empty - no operands */
            variablesPtr->status = MissingOperand;
    }

    if(split(arr[SECEND],",",checkExtraOperand) == DELIM_EXIST) { /* split with the next comma, if there are more */
        if(!strcmp(checkExtraOperand[SECEND],"")) /* if the part after the comma is empty we have extra comma */
            variablesPtr->status = ExtraComma;
        else if(!strcmp(checkExtraOperand[IMPORTANT],"")) /* if the part before the comma is empty we have extra comma */
            variablesPtr->status = ExtraComma;
        else
            variablesPtr->status = ExtraOperand; /* we have an extra operand */
        return;
    }

    if(variablesPtr->status != Valid)
        return;

    op1 = findReg(arr[FIRST]);
    op2 = findReg(arr[SECEND]);

    if(variablesPtr->status != Valid)
        return;

    if(op1 == -1 || op2 == -1) {
        variablesPtr->status = InvalidOperand;
        return;
    }

    word->code.InstructionType=R;
    word->code.rd=op1;
    word->code.rs = op2;
    word->code.rt = 0;
    word->code.unused=0;

    /*addWordToImage(&variablesPtr->codeHptr,*word,variablesPtr->IC);*/
    variablesPtr->IC=IC+4;

    variablesPtr->status = Valid;
}

/* this function handles the instruction commands with 1 operand*/
void fillOneOperandJ(char *str,Word *word, variables *variablesPtr)
{
    int op;
    char checkExtraOperand[STRING_PARTS][LINE_LEN];

    strcpy(str, strip(str));

    if(!strcmp(str,"")) {
        variablesPtr->status = NoOperands;
        return;
    }

    if(split(str,",",checkExtraOperand) == DELIM_EXIST) { /* if we find a comma */
        if(!strcmp(checkExtraOperand[REST],"")) /* nothing after the comma - an extra comma */
            variablesPtr->status = ExtraComma;
        else /* we have an extra operand */
            variablesPtr->status = ExtraOperand;
        return;
    }


    op = findReg(str);

    if(op == -1) {
        word->code.type=J;
        word->code.reg = 0;
        word->code.checkOnSecondPass = True;
    }

    if(variablesPtr->status != Valid)
        return;

    addWordToImage(&variablesPtr->codeHptr,*word,variablesPtr->IC);
    variablesPtr->IC=IC+4;

    variablesPtr->status = Valid;
}
