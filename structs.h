#ifndef _STRUCTS_HEADER_

/* Define Constants */
#define _STRUCTS_HEADER_

/* Redefine char* to string */
typedef char* string;
/* Define pointers to the various structs */
typedef struct sList * simpleList;
typedef struct task * task_link;
typedef struct node * link;
typedef struct node_b * link_b;
typedef struct list * link_list;

/* Define struct used in task dependencies and dependents (linked list (simple)) */
struct sList{
    task_link task;
    simpleList next;
};

/* Define struct used to store the various informations of the task created by the user */
struct task {
    unsigned long id, duration, early_start, late_start;
    simpleList dependencies, dependents;
    string description;
};

/* Node used in double linked list where all tasks are stored in the order given by user */
struct node {
    task_link task;
    link next, prev;
};
/* Node used in binary tree where tasks are stored ordered by their ID number (search faster) */
struct node_b {
    link item;
    link_b r, l;
    int height;
};

/* Define struct used to store all tasks in the order given by user (double linked list) */
struct list {
    int path;
    long unsigned path_duration;
    link first, last;
    link_b head;
};

#endif