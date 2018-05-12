#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "task.h"

/* Define Constants */
#define MAXINPUT 100000
#define MAXCOMMAND 9

/* Define Structs */

/* Global Variables */


/* ---------- Complementary Functions ---------- */
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

    if (tasksList->first == searched) {
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

    tasksList = malloc(sizeof(struct node));
    tasksList->first = NULL;
    tasksList->last = NULL;
    tasksList->path = 0;

    return tasksList;
}

/* ---------- Asked Functions ---------- */
void add(list * tasksList, string buffer) {
    struct task * new_task = createTask(tasksList, buffer);
    struct node * new_node;

    if (new_task != NULL) {
        new_node = createNode(new_task);
        if (tasksList->first == NULL && tasksList->last == NULL) {
            tasksList->first = new_node;
            tasksList->last = new_node;
        }
        else {
            new_node->prev = tasksList->last;
            tasksList->last->next = new_node;
            tasksList->last = new_node;
        }
    }
}

void duration(list * tasksList, string buffer) {
    int offset = 0;
    long unsigned threshold = 0;
    struct node * current = tasksList->first;

    sscanf(buffer, "%lu%n", &threshold, &offset);
    buffer = buffer + offset;

    while (current != NULL) {
        if (current->task->duration >= threshold)
            showTask(tasksList, current->task);
        
        current = current->next;
    }
}

void dependents_list(list * tasksList, string buffer) {
    int offset = 0;
    long unsigned i, task_id;
    struct task * searched;
    struct node * tempNode;

    sscanf(buffer, "%lu%n", &task_id, &offset);
    buffer = buffer + offset;

    tempNode = searchTask(tasksList, task_id);

    if (tempNode == NULL) printf("no such task\n");
    else {
        searched = tempNode->task;
        printf("%lu:", searched->id);
        if (searched->ndependents == 0) printf(" no dependencies");
        else
            for (i = 0; i < searched->ndependents; i++)
                printf(" %lu", searched->dependents[i]->id);
        printf("\n");
    }
}

void task_remover(list * tasksList, string buffer) {
    int offset = 0;
    long unsigned task_id;
    struct node * searched;

    sscanf(buffer, "%lu%n", &task_id, &offset);
    buffer = buffer + offset;
    searched = searchTask(tasksList, task_id);

    if (searched == NULL) printf("no such task\n");
    else if (searched->task->ndependents > 0) printf("task with dependencies\n");
    else {
        removeTask(searched->task);
        removeNode(tasksList, searched);
    }
}

void path(list * tasksList) {
    long unsigned i;
    struct node * current = tasksList->last;
    struct task * task;

    tasksList->path = 1;
    while (current != NULL) {
        setupLate_Start(current->task);
        current = current->prev;
    }

    task = tasksList->first->task;
    while (task != tasksList->last->task) {
        showTask(tasksList, task);
        for (i = 0; i < task->ndependents; i++)
            if (task->dependents[i]->early_start == task->dependents[i]->late_start)
                task = task->dependents[i];
    }

    showTask(tasksList, task);
    printf("project duration = %lu\n", task->duration + task->late_start);
}

/* ---------- Main ---------- */
int main(int argc, string*argv) {
    int offset;
    string buffer;
	char input[MAXINPUT], command[MAXINPUT];
    list * tasksList;

    tasksList = createList();

    buffer = input;
    offset = 0;

    *buffer = '\n';
	do {
        fgets(input, MAXINPUT, stdin);
        buffer = input;

        sscanf(buffer, "%s%n", command, &offset);
        buffer = buffer + offset;

        if (!strcmp(command, "add")) {
            add(tasksList, buffer);
        }
        else if (!strcmp(command, "duration")) {
            duration(tasksList, buffer);
        }
        else if (!strcmp(command, "depend")) {
            dependents_list(tasksList, buffer);
        }
        else if (!strcmp(command, "remove")) {
            task_remover(tasksList, buffer);
        }
        else if (!strcmp(command, "path")) {
            path(tasksList);
        }
        else if (strcmp(command, "exit")) {
            printf("illegal arguments\n");
        }
	
	} while (strcmp(command, "exit"));
	return 0;
}