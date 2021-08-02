#ifndef LABEL_TABLE_LINKED_LIST_H
#define LABEL_TABLE_LINKED_LIST_H

/* ~~~~~~~~~~~~~~~~~~~~~~ MSGS ~~~~~~~~~~~~~~~~~~~~~~ */

char* CANNOT_BUILD = "Cannot Build List!";


/* ~~~~~~~~~~~~~~~~~~~~ Includes ~~~~~~~~~~~~~~~~~~~~ */

#include "defaults.h"


/*  ~~~~~~~~~~~~~~ Methods Declaration ~~~~~~~~~~~~~~ */

void setType(labelTableNodePtr, char *, Type);

void addToList(labelTableNodePtr *, labelTableNode);

void freeLabelList(labelTableNodePtr *);    /* free the space */

Bool labelInList(labelTableNodePtr, char *);

Type getLabelType(labelTableNodePtr, char *);

int getLabelAddress(labelTableNodePtr, char *);



#endif
