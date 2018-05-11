#include "task.h"

task_link setupTask() {
    struct task * new_task;

    new_task = malloc(sizeof(struct task));
    new_task->id = 0;
    new_task->description = NULL;
    new_task->duration = 0;
    new_task->dependencies = NULL;
    new_task->ndependencies = 0;
    return new_task;
}

string taskDescription(string * buffer) {
    string string, beg, end;

    beg = strstr(*buffer, "\"") + 1;
    end = strstr(beg, "\"");
    *buffer = end + 1;
    
    string = malloc(sizeof(char) * ((* buffer)-beg));
    strcpy(string, beg);
    *(string + (end - beg)) = '\0';
    
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

    new_task->description = taskDescription(&buffer);

    sscanf(buffer, "%lu%n", &new_task->duration, &offset);
    buffer = buffer + offset;

    while (* buffer != '\n') {
        new_task->dependencies = realloc(new_task->dependencies, sizeof(task_link)*(ndependencies + 1));
        sscanf(buffer, "%lu%n", &dependencie, &offset);
        buffer = buffer + offset;
        new_task->dependencies[ndependencies] = searchTask(tasks, dependencie);
        ndependencies ++;
    }
    new_task->ndependencies = ndependencies;

    return new_task;
}
