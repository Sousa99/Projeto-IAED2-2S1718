/**     ||  Project #2 - IAED - 2017/2018 - 2Sem    ||
 *      ||  Name: Rodrigo Borges Pessoa de Sousa    ||
 *      ||  Nº: 189535                  LEIC - A    || */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "structs.h"
#include "task.h"
#include "lists.h"
#include "btree.h"

/* Define Constants */
#define MAXINPUT 1000

/* Define Structs */

/* Global Variables */


/* ---------- Complementary Functions ---------- */
/**	Function: getinput
 *	@return buffer (string)
 *	Stores line wirtten by user in buffer witot any limitation in size
 */
string getinput() {
    size_t size = 0;
    size_t len = 0;
    string buffer = NULL;

    /* While line doesn't have char '\n' repeat, meaning fgets didn't get full line */
    do {
        size += MAXINPUT;
        buffer = realloc(buffer, size);
        fgets(buffer + len, size - len - 2, stdin);
        len = strlen(buffer);
    } while (!feof(stdin) && buffer[len - 1] != '\n');

    return buffer;
}

/**	Function: freeAll
 *	@param tasksList (link_list)
 *  Frees all tasks, nodes from linked list, nodes from binary tree and lists
 */
void freeAll(link_list tasks) {
    /* Remove node of binary tree */
    STfree(&tasks->head);
    /* Iterate over all nodes removing one by one */
    while (tasks->last != NULL) {
        /* Remove task */
        removeTask(tasks->last->task);
        /* Remove node of double linked list */
        removeNode(tasks, tasks->last);
    }

    /* Free the linked list itself */
    free(tasks);
}

/* ---------- Asked Functions ---------- */
/**	Function: add
 *	@param tasksList (link_list)
 *  @param buffer (string)
 *	Adds a new task to tasksList getting the paramethers from buffer and validating them
 */
void add(link_list tasksList, string buffer) {
    /* Effectively create the task according to all the paramethers given by user */
    task_link new_task = createTask(tasksList, buffer);
    link new_node;

    /* Add list to the linked list and binary tree */
    if (new_task != NULL) {
        /* Previously found path is no longer valid */
        tasksList->path = 0;
        tasksList->path_duration = 0;

        /* Create node that is going to be stored in linked list */
        new_node = createNode(new_task);
        /* Add list to the binary tree */
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

/**	Function: duration
 *	@param tasksList (link_list)
 *  @param buffer (string)
 *	Prints every task from tasksList with duration > to the
 * given by user and stored in buffer
 */
void duration(link_list tasksList, string buffer) {
    int offset = 0;
    long unsigned threshold = 0;
    link current = tasksList->first;

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

/**	Function: dependents_list
 *	@param tasksList (link_list)
 *  @param buffer (string)
 *	Lists all tasks from tasksList that depend on the
 * one given by user and stored in buffer
 */
void dependents_list(link_list tasksList, string buffer) {
    int offset = 0;
    long unsigned task_id;
    task_link searched;
    link tempNode;
    simpleList current;

    /* Get the id from buffer of the task */
    sscanf(buffer, "%lu%n", &task_id, &offset);
    buffer = buffer + offset;
    /* Search for the task in Binary Tree */
    tempNode = STsearch(tasksList->head, task_id);

    /* If task isn't stored */
    if (tempNode == NULL) printf("no such task\n");
    else {
        /* Effectively prints the id's of all tasks that depend on the one given */
        searched = tempNode->task;
        printf("%lu:", searched->id);
        if (searched->dependents == NULL) printf(" no dependencies");
        else {
            current = searched->dependents;
            while (current != NULL) {
                printf(" %lu", current->task->id);
                current = current->next;
            }
        }
        printf("\n");
    }
}

/**	Function: task_remover
 *	@param tasksList (link_list)
 *  @param buffer (string)
 *	Removes the task given by user and stored in buffer from tasksList
 */
void task_remover(link_list tasksList, string buffer) {
    int offset = 0;
    long unsigned task_id;
    link searched;

    /* Get the id from buffer of the task wished to be removed */
    sscanf(buffer, "%lu%n", &task_id, &offset);
    buffer = buffer + offset;
    /* Search for the task in Binary Tree */
    searched = STsearch(tasksList->head, task_id);

    /* If task isn't stored */
    if (searched == NULL) printf("no such task\n");
    /* In case task has dependencies (means it can't be removed) */
    else if (searched->task->dependents != NULL) printf("task with dependencies\n");
    else {
        /* Previously found path is no longer valid */
        tasksList->path = 0;
        tasksList->path_duration = 0;

        /* Effectively remove task, meaning components, nodes that store it and lists */
        STdelete(&tasksList->head, searched->task->id);
        removeTask(searched->task);
        removeNode(tasksList, searched);
    }
}

/**	Function: path
 *	@param tasksList (link_list)
 *  Lists all tasks, from tasksList, from Critical Path
 */
void path(link_list tasksList) {
    link current;

    /* If duration is called next early and late start will be represented */
    tasksList->path = 1;
    /* Get path duration and store it in tasksList->path_duration */
    setupPath_duration(tasksList);

    /* For each of the tasks stored in tasksList calculate and store late start */
    current = tasksList->last;
    while (current != NULL) {
        setupLate_Start(current->task, tasksList->path_duration);
        current = current->prev;
    }

    /* Follow critical path and print it out starting at the beggining of List*/
    current = tasksList->first;
    while (current != NULL) {
        if (current->task->early_start == current->task->late_start)
            showTask(tasksList, current->task);
        current = current->next;
    }

    /* Print project duration */
    printf("project duration = %lu\n", tasksList->path_duration);
}

/* ---------- Main ---------- */
int main() {
    int offset = 0;
	char command[MAXINPUT];
    string buffer;
    link_list tasksList = createList();

	do {
        /* Gets input from user and stores it in buffer */
        buffer = getinput();
        /* Stores the first word, (command word), in variable command */
        sscanf(buffer, "%s%n", command, &offset);
        /* Advances buffer pointer so that next sscanf will get next argument */
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
        /* Frees buffer in order to be allocated again completely clean */
        free(buffer);
	
	} while (strcmp(command, "exit"));

    /* In case user wants to exit program free everything */
    freeAll(tasksList);
	return 0;
}