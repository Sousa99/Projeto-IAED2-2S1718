#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "structs.h"

#ifndef _TASK_HEADER_

/* Define Constants */
#define _TASK_HEADER_

/* ---------- Headers of Functions ---------- */
void removeTask(task_link task);
void setupLate_Start(task_link task, long unsigned path_duration);
void setupPath_duration(link_list tasks);
task_link createTask(link_list tasks, string buffer);
void showTask(link_list tasks, task_link current);

#endif