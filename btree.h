#include <string.h>
#include <stdlib.h>

#include "lists.h"

#ifndef _BTREE_HEADER_

/* Define Constants */
#define _BTREE_HEADER_

typedef long unsigned* Key;
typedef struct node* Item;

#define Key long unsigned
#define key(a) (a != NULL ? a->task->id: 0)
#define less(a,b) (a < b)
#define eq(a,b) (a == b)

/* ---------- Headers of Functions ---------- */


#endif