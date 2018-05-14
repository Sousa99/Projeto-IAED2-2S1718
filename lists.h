#include <string.h>
#include <stdlib.h>

#include "task.h"

#ifndef _LISTS_HEADER_

/* Define Constants */
#define _LISTS_HEADER_

/* ---------- Headers of Functions ---------- */
link createNode(task_link new_task);
void removeNode(list * tasksList, struct node * searched);
list * createList();
void freeAll(list * tasks);


#endif