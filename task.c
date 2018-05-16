#include "task.h"
#include "btree.h"

task_link setupTask() {
    struct task * new_task;

    new_task = malloc(sizeof(struct task));
    new_task->id = 0;
    new_task->description = NULL;
    new_task->duration = 0;
    new_task->dependencies = NULL;
    new_task->dependents = NULL;
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
    if (STsearch(tasks->head, new_task->id) != NULL) {
        printf("id already exists\n");
        freeTask(new_task);
        return NULL;
    }

    new_task->description = taskDescription(&buffer);
    if (new_task->description == NULL) {
        printf("illegal arguments\n");
        freeTask(new_task);
        return NULL;
    }

    sscanf(buffer, "%lu%n", &new_task->duration, &offset);
    buffer = buffer + offset;

    if (new_task->id == 0 || new_task->duration == 0) {
        printf("illegal arguments\n");
        freeTask(new_task);
        return NULL;
    }

    if (!taskDependencies(tasks, new_task, &buffer)) {
        freeTask(new_task);
        return NULL;
    }

    setupEarly_Start(new_task);

    return new_task;
}

void freeTask(task_link task) {
    free(task->description);
    free(task);
}

void removeTask(task_link task) {
    simpleList current_main, current_sub, temp;

    current_main = task->dependencies;
    while (current_main != NULL) {
        current_sub = current_main->task->dependents;
        if (current_sub->next == NULL) {
            free(current_sub);
            current_main->task->dependents = NULL;
        }
        else if (current_sub->task == task) {
            temp = current_sub;
            current_main->task->dependents = current_sub->next;
            free(temp);
        }
        else {
            while (current_sub->next->task != task)
                current_sub = current_sub->next;
            temp = current_sub->next;
            current_sub->next = current_sub->next->next;
            free(temp);
        }
        
        temp = current_main;
        current_main = current_main->next;
        free(temp);
    }

    freeTask(task);
}

void showTask(list * tasks, struct task * current) {
    simpleList current_dependencies;

    printf("%lu \"%s\" %lu",
        current->id, current->description, current->duration);

    if (tasks->path) {
        printf(" [%lu ", current->early_start);
        if (current->early_start == current->late_start)
            printf("CRITICAL]");
        else printf("%lu]", current->late_start);
    }
    
    current_dependencies = current->dependencies;
    while (current_dependencies != NULL) {
        printf(" %lu", current_dependencies->task->id);
        current_dependencies = current_dependencies->next;
    }

    printf("\n");
}

string taskDescription(string * buffer) {
    string string, beg, end;

    beg = strstr(*buffer, "\"");
    if (beg == NULL) return NULL;
    beg ++;

    end = strstr(beg, "\"");
    if (end == NULL) return NULL;
    *end = '\0';

    *buffer = end + 1;
    
    string = malloc(sizeof(char) * (end - beg + 1));
    strcpy(string, beg);
    
    return string;
}

int taskDependencies(list * tasks, task_link new_task, string * buffer) {
    int offset, already_exists;
    long unsigned dependencie;
    struct task * searched;
    struct node * tempNode;
    simpleList current, new_dependencie, new_dependent;

    while (** buffer != '\n') {
        already_exists = 0;
        dependencie = 0;
        sscanf(*buffer, "%lu%n", &dependencie, &offset);
        if (dependencie == 0) {
            printf("illegal arguments\n");
            return 0;
        }

        *buffer = *buffer + offset;
        tempNode = STsearch(tasks->head, dependencie);
        
        if (tempNode == NULL) {
            printf("no such task\n");
            return 0;
        }

        current = new_task->dependencies;
        while (current != NULL) {
            if (current->task->id == dependencie) already_exists = 1;
            current = current->next;
        }

        if (!already_exists) {
            searched = tempNode->task;

            new_dependencie = malloc(sizeof(struct simpleList));
            new_dependencie->task = searched;
            new_dependencie->next = NULL;

            if (new_task->dependencies == NULL) new_task->dependencies = new_dependencie;
            else {
                current = new_task->dependencies;
                while (current->next != NULL) current = current->next;
                current->next = new_dependencie;
            }
            
            new_dependent = malloc(sizeof(struct simpleList));
            new_dependent->task = new_task;
            new_dependent->next = NULL;

            if (searched->dependents == NULL) searched->dependents = new_dependent;
            else {
                current = searched->dependents;
                while (current->next != NULL) current = current->next;
                current->next = new_dependent;
            }
        }
    }

    return 1;
}

void setupEarly_Start(task_link task) {
    task_link currentMax;
    simpleList current;
    
    if (task->dependencies != NULL) {
        currentMax = task->dependencies->task;
        current = task->dependencies;
        while (current != NULL) {
            if (currentMax->duration + currentMax->early_start < 
            current->task->duration + current->task->early_start) 
                currentMax = current->task;
            current = current->next;
        }
        task->early_start = currentMax->duration + currentMax->early_start;
    }
}

void setupLate_Start(task_link task, long unsigned path_duration) {
    task_link currentMin;
    simpleList current;

    if (task->dependents == NULL) task->late_start = path_duration - task->duration;
    else {
        currentMin = task->dependents->task;
        current = task->dependents;
        while (current != NULL) {
            if (currentMin->late_start > current->task->late_start)
                currentMin = current->task;
            current = current->next;
        }
        
        task->late_start = currentMin->late_start - task->duration;
    }
}

void setupPath_duration(list * tasks) {
    struct node * current = tasks->first;
    
    while (current != NULL) {
        if (current->task->dependents == NULL &&
            current->task->duration + current->task->early_start > tasks->path_duration)
                tasks->path_duration = current->task->duration + current->task->early_start;
        current = current->next;
    }
}