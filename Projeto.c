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
    tasksList->first = NULL;
    tasksList->last = NULL;
    tasksList->path = 0;

    return tasksList;
}

void freeAll(list * tasks) {
    while (tasks->last != NULL) {
        removeTask(tasks->last->task);
        removeNode(tasks, tasks->last);
    }

    free(tasks);
}

/* ---------- Asked Functions ---------- */
void add(list * tasksList, string buffer) {
    struct task * new_task = createTask(tasksList, buffer);
    struct node * new_node;

    tasksList->path = 0;
    tasksList->path_duration = 0;

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

    tasksList->path = 0;
    tasksList->path_duration = 0;

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
    struct node * current = tasksList->last;

    tasksList->path = 1;

    setupPath_duration(tasksList);
    while (current != NULL) {
        setupLate_Start(current->task, tasksList->path_duration);
        current = current->prev;
    }

    current = tasksList->first;
    while (current != NULL) {
        if (current->task->early_start == current->task->late_start)
            showTask(tasksList, current->task);
        current = current->next;
    }

    printf("project duration = %lu\n", tasksList->path_duration);
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

    freeAll(tasksList);
	return 0;
}