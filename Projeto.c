#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "task.h"

/* Define Constants */
#define MAXINPUT 100000
#define MAXCOMMAND 9
#define MAXDESCRIPTION 8001
#define MAXDEPENDENCIES 10

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

/* ---------- Asked Functions ---------- */
void add(list * tasksList, string buffer) {
    struct task * new_task = createTask(tasksList, buffer);
    struct node * new_node = createNode(new_task);

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

void duration(list * tasksList, string buffer) {
    int i, offset = 0;
    long unsigned threshold = 0;
    struct node * current = tasksList->first;

    sscanf(buffer, "%lu%n", &threshold, &offset);
    buffer = buffer + offset;

    while (current != NULL) {
        if (current->task->duration >= threshold) {
            printf("%lu \"%s\" %lu",
                current->task->id, current->task->description, current->task->duration);
            for (i = 0; i < current->task->ndependencies; i++) {
                printf(" %lu", current->task->dependencies[i]->id);
            }
            printf("\n");
        }
        current = current->next;
    }
}

void dependents_list(list * tasksList, string buffer) {
    int offset = 0;
    long unsigned i, task_id;
    struct task * searched;

    sscanf(buffer, "%lu%n", &task_id, &offset);
    buffer = buffer + offset;
    searched = searchTask(tasksList, task_id);

    printf("%lu:", searched->id);
    if (searched->ndependents == 0) printf(" no dependencies");
    else
        for (i = 0; i < searched->ndependents; i++)
            printf(" %lu", searched->dependents[i]->id);
    printf("\n");
}

/* ---------- Main ---------- */
int main(int argc, string*argv) {
	char input[MAXINPUT], command[MAXINPUT];
    string buffer;
    int offset;

    list * tasksList;
    tasksList = malloc(sizeof(struct node));
    tasksList->first = NULL;
    tasksList->last = NULL;

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
            printf("4\n");
        }
        else if (!strcmp(command, "path")) {
            printf("5\n");
        }
        else if (strcmp(command, "exit")) {
            printf("illegal arguments\n");
        }


	
	} while (strcmp(command, "exit"));
	return 0;
}