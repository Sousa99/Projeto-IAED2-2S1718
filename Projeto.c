#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Define Constants */
#define MAXINPUT 100000
#define MAXCOMMAND 9
#define MAXDESCRIPTION 8001
#define MAXDEPENDENCIES 10

/* Define Structs */
typedef char* String;
typedef struct node * link;

typedef struct task {
    unsigned long id, duration, ndependencies;
    struct task *dependencies[MAXDEPENDENCIES];
    String description;
} * task_link;

struct node {
    task_link task;
    link next;
    link prev;
};

typedef struct {  
     link first;  
     link last;  
} list;

/* Global Variables */


/* ---------- Complementary Functions ---------- */
String taskDescription(char * buffer) {
    String string, beg;

    beg = strstr(buffer, "\"");
    buffer = beg + 1;
    buffer = strstr(buffer, "\"");
    * buffer = '\0';
    
    string = malloc(sizeof(char) * (buffer-beg+1));
    strcpy(string, beg);
    
    return string;
}

task_link searchTask(list * tasks, long unsigned dependencie_number) {
    struct node * current = tasks->first;

    while (current != NULL) {
        if (current->task->id == dependencie_number) {
            printf("Encontrei!\n");
            return current->task;
        }
        current = current->next;
    }

    return NULL;
}

task_link createTask(list * tasks, char * buffer) {
    int offset = 0;
    int ndependencies = 0;
    long unsigned dependencie;
    struct task * new_task;

    new_task = malloc(sizeof(struct task));
    new_task->ndependencies = 0;

    sscanf(buffer, "%lu%n", &new_task->id, &offset);
    buffer = buffer + offset;

    new_task->description = taskDescription(buffer);

    sscanf(buffer, "%lu%n", &new_task->duration, &offset);
    buffer = buffer + offset;

    while (* buffer != '\n') {
        sscanf(buffer, "%lu%n", &dependencie, &offset);
        buffer = buffer + offset;
        printf("Dependencie: %lu\n", dependencie);
        new_task->dependencies[ndependencies] = searchTask(tasks, dependencie);
        printf("Dependencie Found: %lu\n", new_task->dependencies[ndependencies]->id);
        ndependencies ++;
    }
    new_task->ndependencies = ndependencies;

    return new_task;
}

link createNode(task_link new_task) {
    struct node * new_node;

    new_node = malloc(sizeof(struct node));
    new_node->task = new_task;
    new_node->next = NULL;
    new_node->prev = NULL;

    return new_node;
}

/* ---------- Asked Functions ---------- */
void add(list * tasks, char * buffer) {
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

void duration(list * tasks) {
    int i;
    struct node * current = tasks->first;

    while (current != NULL) {
        printf("%lu %s %lu", current->task->id, current->task->description, current->task->duration);
        for (i = 0; i < current->task->ndependencies; i++) {
            printf(" %lu", current->task->dependencies[i]->id);
        }
        printf("\n");

        current = current->next;
    }
}

/* ---------- Main ---------- */
int main(int argc, char **argv) {
	char input[MAXINPUT], command[MAXINPUT];
    char * buffer;
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
            duration(tasks);
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