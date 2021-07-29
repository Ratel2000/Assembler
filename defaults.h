#ifndef DEFAULTS_H
#define DEFAULTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define FILE_NAME_LEN 100
#define AS_OB_EXTENSION_LEN 3
#define EXT_ENT_EXTENSION_LEN 4
#define LINE_LEN 82
#define LABEL_LEN 31
#define WORD_LEN 32
#define MAX_32_SIGNED 2147483647
#define MIN_32_SIGNED -2147483648
#define MAX_32_UNSIGNED 4294967295
#define MINUS1_6_BITS 63

#define ADDRESS_LEN 25
#define OPCODE_LEN 6
#define FUNCT_LEN 5
#define RS_LEN 5
#define RT_LEN 5
#define RD_LEN 5
#define IMMED_LEN 16
#define REG_LEN 1
#define UNUSED_LEN 6

#define STARTING_IC 100

typedef enum {False, True} Bool;
typedef enum {CodeImage, DataImage} Location;
typedef enum {Entry, External, NoneEntOrExt} Type;
typedef enum {R,I,J} InstructionType;
typedef enum {Asciz,Dw,Dh,Db, NoneDirectiveType} DirectiveType;
typedef enum {Invalid, Empty, Comment, Directive, Instruction} Statement;
typedef enum {LineTooLong=0, UnknownOperation=1, LabelAlreadyExist=2,
              TextAfterCommand=3, MissingOperand=4, InvalidOperand=5,
              NoOperands=6, InvalidNumber=7, MultipleDirectives=8,
              ReservedLabelName=9, LabelTooLong=10, LabelInvalidStart=11,
              LabelInvalidCharacters=12, MissingWhitespace=13,
              LabelEntryAndExtern=14, LabelDefinedAndExtern=15,
              MissingLabel=16, InvalidDirectiveCommand=17,
              NoClosingQuotes=18, ExtraneousText=19, ExternalBranching=20,
              NoOpeningQuotes=21, ExtraComma=22, NumOutOfMemory=23,
              Invalid2AdressMethod=24, Invalid0AdressMethod=25,
              InvalidLeaOperands=26, InvalidOperand5_12=27,
              InvalidOperand9=28, ExtraOperand=29, MissingOperation=30,
              Valid=100} Status;

typedef struct node *labelTableNodePtr;
typedef struct node {
    char label[LABEL_LEN];
    unsigned long address;
    Location location;
    Type type;
    labelTableNodePtr next;
} labelTableNode;

typedef union {
    unsigned long index;

    struct {
    	InstructionType type;/*R,I,J*/
    	unsigned int address:ADDRESS_LEN;
        unsigned int rs:RS_LEN;
        unsigned int unused:UNUSED_LEN;
        unsigned int funct:FUNCT_LEN;
        unsigned int rt:RT_LEN;
        unsigned int rd:RD_LEN;
        unsigned int reg:REG_LEN;
        unsigned int immed:IMMED_LEN;
        unsigned int opcode:OPCODE_LEN;
    } code;
} Word;

typedef struct wordnode *wordNodePtr;
typedef struct wordnode {
    char externLabel[LABEL_LEN];
    Word word;
    unsigned long address;
    wordNodePtr next;
} wordNode;

typedef struct {
        int IC;
        int DC;
        int lineCounter;
        char    filename[FILE_NAME_LEN];
        FILE *file;
        char line[LINE_LEN];
        char label[LABEL_LEN];
        Bool foundError;
        Status status;
        labelTableNodePtr labelHptr;
        wordNodePtr dataHptr;
        wordNodePtr codeHptr;
} variables;

void defaultValues(variables*);
#endif
