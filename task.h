#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "structs.h"

#ifndef _TASK_HEADER_

/* Define Constants */
#define _TASK_HEADER_

/* ---------- Headers of Functions ---------- */
task_link setupTask();
task_link createTask(link_list tasks, string buffer);
void removeTask(task_link task);
void freeTask(task_link task);
void showTask(link_list tasks, task_link current);
string taskDescription();
int taskDependencies(link_list tasks, task_link new_task, string * buffer);
void setupEarly_Start(task_link task);
void setupLate_Start(task_link task, long unsigned path_duration);
void setupPath_duration(link_list tasks);
struct node * searchTask(link_list tasks, long unsigned dependencie_number);

#endif