#include <string.h>
#include <stdlib.h>

#include "structs.h"

#ifndef _LISTS_HEADER_

/* Define Constants */
#define _LISTS_HEADER_

/* ---------- Headers of Functions ---------- */
link createNode(task_link new_task);
void removeNode(link_list tasksList, link searched);
link_list createList();


#endif