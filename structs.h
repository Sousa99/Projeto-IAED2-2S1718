#ifndef _STRUCTS_HEADER_

/* Define Constants */
#define _STRUCTS_HEADER_

typedef char* string;
typedef struct sList * simpleList;
typedef struct task * task_link;
typedef struct node * link;
typedef struct node_b * link_b;
typedef struct list * link_list;

struct sList{
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

struct list {
    int path;
    long unsigned path_duration;
    link first, last;
    link_b head;
};

#endif