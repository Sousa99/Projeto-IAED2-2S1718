#include "task.h"

task_link setupTask() {
    struct task * new_task;

    new_task = malloc(sizeof(struct task));
    new_task->id = NULL;
    new_task->description = NULL;
    new_task->duration = NULL;
    new_task->ndependencies = 0;
    return new_task;
}

string taskDescription(string buffer) {
    string string, beg;

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
        if (current->task->id == dependencie_number) return current->task;
        current = current->next;
    }

    return NULL;
}

task_link createTask(list * tasks, string buffer) {
    int offset = 0;
    int ndependencies = 0;
    long unsigned dependencie;
    struct task * new_task;

    new_task = setupTask();

    sscanf(buffer, "%lu%n", &new_task->id, &offset);
    buffer = buffer + offset;

    /*new_task->description = taskDescription(buffer);*/

    sscanf(buffer, "%lu%n", &new_task->duration, &offset);
    buffer = buffer + offset;

    while (* buffer != '\n') {
        sscanf(buffer, "%lu%n", &dependencie, &offset);
        buffer = buffer + offset;
        new_task->dependencies[ndependencies] = searchTask(tasks, dependencie);
        ndependencies ++;
    }
    new_task->ndependencies = ndependencies;

    return new_task;
}
