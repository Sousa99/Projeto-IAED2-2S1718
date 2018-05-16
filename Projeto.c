#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "task.h"
#include "lists.h"
#include "btree.h"

/* Define Constants */
#define MAXINPUT 1000

/* Define Structs */

/* Global Variables */


/* ---------- Complementary Functions ---------- */

string getinput() {
    size_t size = 0;
    size_t len = 0;
    string buffer = NULL;

    do {
        size += MAXINPUT;
        buffer = realloc(buffer, size);
        fgets(buffer + len, size - len - 2, stdin);
        len = strlen(buffer);
    } while (!feof(stdin) && buffer[len - 1] != '\n');

    return buffer;
}

/* ---------- Asked Functions ---------- */
void add(list * tasksList, string buffer) {
    struct task * new_task = createTask(tasksList, buffer);
    struct node * new_node;

    if (new_task != NULL) {
        tasksList->path = 0;
        tasksList->path_duration = 0;

        new_node = createNode(new_task);
        STinsert(&tasksList->head, new_node);
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

    if (sscanf(buffer, "%lu%n", &threshold, &offset) == EOF || threshold != 0) {
        buffer = buffer + offset;
        while (current != NULL) {
            if (current->task->duration >= threshold)
                showTask(tasksList, current->task);
            
            current = current->next;
        }
    }
    else printf("illegal arguments\n");
}

void dependents_list(list * tasksList, string buffer) {
    int offset = 0;
    long unsigned i, task_id;
    struct task * searched;
    struct node * tempNode;

    sscanf(buffer, "%lu%n", &task_id, &offset);
    buffer = buffer + offset;

    tempNode = STsearch(tasksList->head, task_id);

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
    searched = STsearch(tasksList->head, task_id);

    if (searched == NULL) printf("no such task\n");
    else if (searched->task->ndependents > 0) printf("task with dependencies\n");
    else {
        tasksList->path = 0;
        tasksList->path_duration = 0;

        STdelete(&tasksList->head, searched->task->id);
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
    int offset = 0;
    string buffer;
	char command[MAXINPUT];
    list * tasksList = createList();

	do {
        buffer = getinput();

        sscanf(buffer, "%s%n", command, &offset);
        buffer += offset;

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

        buffer -= offset;
        free(buffer);
	
	} while (strcmp(command, "exit"));

    freeAll(tasksList);
	return 0;
}