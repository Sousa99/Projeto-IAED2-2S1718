#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef _TASK_HEADER_

/* Define Constants */
#define _TASK_HEADER_

/* Define Structs */
typedef char* string;
typedef struct task * task_link;
typedef struct node * link;
typedef struct node_b * link_b;
typedef struct simpleList * simpleList;

struct simpleList{
    task_link task;
    simpleList next;
};

struct task {
    unsigned long id, duration, early_start, late_start;
    simpleList dependencies, dependents;
    string description;
};

struct node {
    task_link task;
    link next, prev;
};
struct node_b {
    link item;
    link_b r, l;
    int height;
};

typedef struct {
    int path;
    long unsigned path_duration;
    link first, last;
    link_b head;
} list;

/* ---------- Headers of Functions ---------- */
task_link setupTask();
task_link createTask(list * tasks, string buffer);
void removeTask(task_link task);
void freeTask(task_link task);
void showTask(list * tasks, struct task * current);
string taskDescription();
int taskDependencies(list * tasks, task_link new_task, string * buffer);
void setupEarly_Start(task_link task);
void setupLate_Start(task_link task, long unsigned path_duration);
void setupPath_duration(list * tasks);
struct node * searchTask(list * tasks, long unsigned dependencie_number);

#endif