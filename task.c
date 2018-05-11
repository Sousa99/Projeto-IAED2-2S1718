#include "task.h"

task_link setupTask() {
    struct task * new_task;

    new_task = malloc(sizeof(struct task));
    new_task->id = 0;
    new_task->description = NULL;
    new_task->duration = 0;
    new_task->dependencies = NULL;
    new_task->dependents = NULL;
    new_task->ndependencies = 0;
    new_task->ndependents = 0;
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

void taskDependencies(list * tasks, task_link new_task, string * buffer) {
    int offset;
    long unsigned ndependencies = 0, dependencie;
    struct task * searched;

    while (** buffer != '\n') {
        sscanf(*buffer, "%lu%n", &dependencie, &offset);
        *buffer = *buffer + offset;

        ndependencies ++;

        searched = searchTask(tasks, dependencie);
        new_task->dependencies = realloc(new_task->dependencies, sizeof(struct task)*(ndependencies));
        new_task->dependencies[ndependencies-1] = searched;
        
        searched->ndependents ++;
        searched->dependents = realloc(searched->dependents, sizeof(struct task)*(searched->ndependents));
        searched->dependents[searched->ndependents - 1] = new_task;

    }
    new_task->ndependencies = ndependencies;
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
    struct task * new_task;

    new_task = setupTask();

    sscanf(buffer, "%lu%n", &new_task->id, &offset);
    buffer = buffer + offset;

    new_task->description = taskDescription(&buffer);

    sscanf(buffer, "%lu%n", &new_task->duration, &offset);
    buffer = buffer + offset;

    taskDependencies(tasks, new_task, &buffer);

    return new_task;
}
