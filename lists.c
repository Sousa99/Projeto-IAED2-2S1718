#include "lists.h"
#include "btree.h"

link createNode(task_link new_task) {
    struct node * new_node;

    new_node = malloc(sizeof(struct node));
    new_node->task = new_task;
    new_node->next = NULL;
    new_node->prev = NULL;

    return new_node;
}

void removeNode(list * tasksList, struct node * searched) {
    struct node * current = tasksList->first;

    if (tasksList->first == tasksList->last) {
        tasksList->first = NULL;
        tasksList->last = NULL;
    }
    else if (tasksList->first == searched) {
        tasksList->first = searched->next;
        searched->next->prev = NULL;
    }
    else if (tasksList->last == searched) {
        tasksList->last = searched->prev;
        searched->prev->next = NULL;
    }
    else {
        while (current != searched) current = current->next;
        current->prev->next = current->next;
        current->next->prev = current->prev;
    }

    free(searched);
}

list * createList() {
    list * tasksList;

    tasksList = malloc(sizeof(list));
    STinit(&tasksList->head);
    tasksList->first = NULL;
    tasksList->last = NULL;
    tasksList->path = 0;

    return tasksList;
}

void freeAll(list * tasks) {
    while (tasks->last != NULL) {
        STdelete(&tasks->head, tasks->last->task->id);
        removeTask(tasks->last->task);
        removeNode(tasks, tasks->last);
    }

    free(tasks);
}