#include <string.h>
#include <stdlib.h>

#include "structs.h"

#ifndef _BTREE_HEADER_

/* Define Constants */
#define _BTREE_HEADER_

#define key(a) (a != NULL ? a->task->id : 0)
#define less(a,b) (a < b)
#define eq(a,b) (a == b)
typedef unsigned long Key;
typedef link Item;

/* ---------- Headers of Functions ---------- */
void STinit(link_b*);
Item STsearch(link_b, Key);
void STinsert(link_b*, Item);
void STdelete(link_b*, Key);
void STfree(link_b *);

#endif