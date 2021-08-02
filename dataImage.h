#ifndef DATA_IMAGE_H
#define DATA_IMAGE_H

/* ~~~~~~~~~~~~~~~~~~~~~~ MSGS ~~~~~~~~~~~~~~~~~~~~~~ */

char* MALLOC_FAILD = "Malloc Failed - Cannot Build List!";


/* ~~~~~~~~~~~~~~~~~~~~ Includes ~~~~~~~~~~~~~~~~~~~~ */

#include "defaults.h"


/*  ~~~~~~~~~~~~~~ Methods Declaration ~~~~~~~~~~~~~~ */

void addWordToImage(wordNodePtr *, Word, unsigned long);

void freeList(wordNodePtr *);    /* free the space */

int getDestAdd(wordNodePtr, int);

int getOpcode(wordNodePtr, int);

int getSrcAdd(wordNodePtr, int);



#endif
