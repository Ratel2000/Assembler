#include "defaults.h"
#include "firstPass.h"

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
    if(findNumberOfOperands(wordPtr->code.opcode,wordPtr->code.funct)==3)
        fillTreeOperands(lineCopy,wordPtr,variablesPtr);
    else if(findNumberOfOperands(wordPtr->code.opcode,wordPtr->code.funct)==2)
        fillTwoOperands(lineCopy,wordPtr,variablesPtr);
    else if(findNumberOfOperands(wordPtr->code.opcode,wordPtr->code.funct)==1)
        fillOneOperand(lineCopy,wordPtr,variablesPtr);
    else if(wordPtr->code.opcode != 63)
            /*add stop command work*/

    else { /* operation has no operands */
        if(strcmp(lineCopy,"")) /* if there is text left raise error */
            variablesPtr->status = TextAfterCommand;
        else {
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
