#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef _TASK_HEADER_

/* Define Constants */
#define _TASK_HEADER_
#define MAXDEPENDENCIES 10

/* Define Structs */
typedef char* string;
typedef struct node * link;

typedef struct task {
    unsigned long id, duration, ndependencies, ndependents;
    struct task **dependencies, **dependents;
    string description;
} * task_link;

struct node {
    task_link task;
    link next;
    link prev;
};

typedef struct {  
     link first;  
     link last;  
} list;

/* ---------- Headers of Functions ---------- */
task_link setupTask();
string taskDescription();
void taskDependencies(list * tasks, task_link new_task, string * buffer);
task_link searchTask(list * tasks, long unsigned dependencie_number);
task_link createTask(list * tasks, string buffer);

#endif