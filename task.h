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
    unsigned long id, duration, ndependencies, ndependents, early_start, late_start;
    struct task **dependencies, **dependents;
    string description;
} * task_link;

struct node {
    task_link task;
    link next;
    link prev;
};

typedef struct {
    int path;
    link first;  
    link last;  
} list;

/* ---------- Headers of Functions ---------- */
task_link setupTask();
void showTask(list * tasks, struct task * current);
void freeTask(task_link task);
void removeTask(task_link task);
string taskDescription();
void taskDependencies(list * tasks, task_link new_task, string * buffer);
struct node * searchTask(list * tasks, long unsigned dependencie_number);
void setupEarly_Start(task_link task);
void setupLate_Start(task_link task);
task_link createTask(list * tasks, string buffer);

#endif