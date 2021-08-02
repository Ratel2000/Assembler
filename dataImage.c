/* ~~~~~~~~~~~~~~~~~~~~ Includes ~~~~~~~~~~~~~~~~~~~~ */

#include "dataImage.h"


/* ~~~~~~~~~~~~~~~~~~~~ Methods ~~~~~~~~~~~~~~~~~~~~~ */

void addWordToImage(wordNodePtr *hptr, Word word, unsigned long address) {
    wordNodePtr p1 = NULL,p2 = NULL, t;

    /* Allocation of memory */
    t = (wordNodePtr) malloc(sizeof(wordNode));

    if(!t)
    {
        printf(MALLOC_FAILD);
        exit(0);
    }

    /* copy fields from node to t */
    t->address = address;
    t->word = word;
    /*  function copies the string pointed by "" to t->externLabel. also returns the copied string. */
    strcpy(t->externLabel,"");
    t->next = NULL;

    p1 = *hptr;

    /* get to the last node */
    while(p1)
    {
        p2 = p1;
        p1 = p1->next;
    }

    /* if the list is empty */
    if(p1 == *hptr)
    {
        *hptr = t;
    }

        /* add the new node to the end of the list */
    else
    {
        p2->next = t;
        t->next = p1;
    }
}

/* function that helps free the space */
void freeList(wordNodePtr *hptr)
{
    wordNodePtr p;

    while(*hptr)
    {
        p = (*hptr)->next;;
        free(*hptr);
        *hptr = p;
    }
}
