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
void add(list * tasks, string buffer) {
    struct task * new_task = createTask(tasks, buffer);
    struct node * new_node = createNode(new_task);

    if (tasks->first == NULL && tasks->last == NULL) {
        tasks->first = new_node;
        tasks->last = new_node;
    }
    else {
        new_node->prev = tasks->last;
        tasks->last->next = new_node;
        tasks->last = new_node;
    }
}

void duration(list * tasks, string buffer) {
    int i, offset = 0;
    long unsigned threshold = 0;
    struct node * current = tasks->first;

    sscanf(buffer, "%lu%n", &threshold, &offset);
    buffer = buffer + offset;

    while (current != NULL) {
        if (current->task->duration >= threshold) {
            printf("%lu %lu", current->task->id, current->task->duration);
            for (i = 0; i < current->task->ndependencies; i++) {
                printf(" %lu", current->task->dependencies[i]->id);
            }
            printf("\n");
        }
        current = current->next;
    }
}

/* ---------- Main ---------- */
int main(int argc, string*argv) {
	char input[MAXINPUT], command[MAXINPUT];
    string buffer;
    int offset;

    list * tasks;
    tasks = malloc(sizeof(struct node));
    tasks->first = NULL;
    tasks->last = NULL;

    buffer = input;
    offset = 0;

    *buffer = '\n';
	do {
        fgets(input, MAXINPUT, stdin);
        buffer = input;

        sscanf(buffer, "%s%n", command, &offset);
        buffer = buffer + offset;

        if (!strcmp(command, "add")) {
            add(tasks, buffer);
        }
        else if (!strcmp(command, "duration")) {
            duration(tasks, buffer);
        }
        else if (!strcmp(command, "depend")) {
            printf("3\n");
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