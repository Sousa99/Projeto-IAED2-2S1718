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
    new_task->early_start = 0;
    new_task->late_start = 0;
    return new_task;
}

task_link createTask(list * tasks, string buffer) {
    int offset = 0;
    struct task * new_task;

    new_task = setupTask();

    sscanf(buffer, "%lu%n", &new_task->id, &offset);
    buffer = buffer + offset;
    if (searchTask(tasks, new_task->id) != NULL) {
        printf("id already exists\n");
        freeTask(new_task);
        return NULL;
    }

    new_task->description = taskDescription(&buffer);

    sscanf(buffer, "%lu%n", &new_task->duration, &offset);
    buffer = buffer + offset;

    if (new_task->id == 0 || new_task->duration == 0) {
        printf("illegal arguments\n");
        freeTask(new_task);
        return NULL;
    }

    if (!taskDependencies(tasks, new_task, &buffer)) {
        printf("no such task\n");
        freeTask(new_task);
        return NULL;
    }

    setupEarly_Start(new_task);

    return new_task;
}

void freeTask(task_link task) {
    free(task->description);
    free(task->dependencies);
    free(task->dependents);
    free(task);
}

void removeTask(task_link task) {
    int done;
    long unsigned i, j;
    struct task * current;

    for (i = 0; i < task->ndependencies; i++) {
        done = 0;
        current = task->dependencies[i];
        for (j = 0; j < current->ndependents; j++) {
            if (current->dependents[j] == task) done = 1;
            else current->dependents[j-done] = current->dependents[j];
        }
        current->ndependents --;
        current->dependents = realloc(current->dependents, sizeof(struct task)*(current->ndependents));
    }

    freeTask(task);
}

void showTask(list * tasks, struct task * current) {
    long unsigned i;

    printf("%lu \"%s\" %lu",
        current->id, current->description, current->duration);

    if (tasks->path) {
        printf(" [%lu ", current->early_start);
        if (current->early_start == current->late_start)
            printf("CRITICAL]");
        else printf("%lu]", current->late_start);
    }
    
    for (i = 0; i < current->ndependencies; i++) {
        printf(" %lu", current->dependencies[i]->id);
    }

    printf("\n");
}

string taskDescription(string * buffer) {
    string string, beg, end;

    beg = strstr(*buffer, "\"") + 1;
    end = strstr(beg, "\"");
    *end = '\0';

    *buffer = end + 1;
    
    string = malloc(sizeof(char) * (end - beg + 1));
    strcpy(string, beg);
    
    return string;
}

int taskDependencies(list * tasks, task_link new_task, string * buffer) {
    int offset;
    long unsigned ndependencies = 0, dependencie;
    struct task * searched;
    struct node * tempNode;

    while (** buffer != '\n') {
        sscanf(*buffer, "%lu%n", &dependencie, &offset);
        *buffer = *buffer + offset;
        tempNode = searchTask(tasks, dependencie);
        
        if (tempNode == NULL) return 0;

        searched = tempNode->task;
        ndependencies ++;
        new_task->dependencies = realloc(new_task->dependencies, sizeof(struct task)*(ndependencies));
        new_task->dependencies[ndependencies-1] = searched;
        
        searched->ndependents ++;
        searched->dependents = realloc(searched->dependents, sizeof(struct task)*(searched->ndependents));
        searched->dependents[searched->ndependents - 1] = new_task;

    }
    new_task->ndependencies = ndependencies;
    return 1;
}

void setupEarly_Start(task_link task) {
    long unsigned i;
    struct task * currentMax;
    
    if (task->ndependencies > 0) {
        currentMax = task->dependencies[0];
        for (i = 1; i < task->ndependencies; i++)
            if (currentMax->duration + currentMax->early_start < 
            task->dependencies[i]->duration + task->dependencies[i]->early_start) 
                currentMax = task->dependencies[i];
        
        task->early_start = currentMax->duration + currentMax->early_start;
    }
}

void setupLate_Start(task_link task, long unsigned path_duration) {
    long unsigned i;
    struct task * currentMax;

    if (task->ndependents == 0) task->late_start = path_duration - task->duration;
    else {
        currentMax = task->dependents[0];
        for (i = 1; i < task->ndependents; i++)
            if (currentMax->late_start > task->dependents[i]->late_start)
                currentMax = task->dependents[i];
        
        task->late_start = currentMax->late_start - task->duration;
    }
}

void setupPath_duration(list * tasks) {
    struct node * current = tasks->first;
    
    while (current != NULL) {
        if (current->task->ndependents == 0 &&
            current->task->duration + current->task->early_start > tasks->path_duration)
                tasks->path_duration = current->task->duration + current->task->early_start;
        current = current->next;
    }
}

struct node * searchTask(list * tasks, long unsigned id) {
    struct node * current = tasks->first;

    while (current != NULL) {
        if (current->task->id == id) return current;
        current = current->next;
    }

    return NULL;
}