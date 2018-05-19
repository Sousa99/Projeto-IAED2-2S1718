#include "task.h"
#include "btree.h"

#define MAXDESCRIPTION 8000

/**	Function: setupTask
 *	@return new_task (task_link)
 *  Inicializes a Task and all its charactheristics
 */
task_link setupTask() {
    task_link new_task;

    /* Allocates memory for a task */
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

/**	Function: createTask
 *	@param tasks (link_list)
 *  @param buffer (string)
 *  @return new_task
 *  Gets all information needed and given by user from buffer
 */
task_link createTask(link_list tasks, string buffer) {
    int offset = 0;
    task_link new_task;

    /* Setup / initialize new_task in order to store necessary information */
    new_task = setupTask();

    /* Get from buffer the id given by user to the new task */
    sscanf(buffer, "%lu%n", &new_task->id, &offset);
    buffer = buffer + offset;
    /* Verify if the id given is already stored in tasks and therefore not available */
    if (STsearch(tasks->head, new_task->id) != NULL) {
        printf("id already exists\n");
        freeTask(new_task);
        return NULL;
    }

    /* Store description from buffer into new_task */
    new_task->description = taskDescription(&buffer);
    /* Verify if the argument given for description is invalid */
    if (new_task->description == NULL) {
        printf("illegal arguments\n");
        freeTask(new_task);
        return NULL;
    }

    /* Get from buffer the duration given by user to the new task */
    sscanf(buffer, "%lu%n", &new_task->duration, &offset);
    buffer = buffer + offset;
    /* Verify if the argument given for duration is valid */
    if (new_task->id == 0 || new_task->duration == 0) {
        printf("illegal arguments\n");
        freeTask(new_task);
        return NULL;
    }

    /* Store dependencies given by user in new_task and verify if argument is valid */
    if (!taskDependencies(tasks, new_task, &buffer)) {
        freeTask(new_task);
        return NULL;
    }

    /* Setup the early start of new_task */
    setupEarly_Start(new_task);

    return new_task;
}

/**	Function: removeDependent
 *	@param task (task_link)
 *  Removes dependent created by task in another through dependencie
 */
void removeDependent(task_link task) {
    simpleList current_dependencie, current_dependent, temp;

    current_dependencie = task->dependencies;
    current_dependent = current_dependencie->task->dependents;

    /* If dependencie only has one dependent therefore is the task to remove */
    if (current_dependent->next == NULL) {
        /* Free the allocated memory for task's dependencie's dependent */
        free(current_dependent);
        current_dependencie->task->dependents = NULL;
    }

    /* Else if first depencie's dependent is the task to remove */
    else if (current_dependent->task == task) {
        temp = current_dependent;
        current_dependencie->task->dependents = current_dependent->next;
        /* Free the allocated memory for task's dependencie's dependent */
        free(temp);
    }

    else {
        while (current_dependent->next->task != task)
            current_dependent = current_dependent->next;
        temp = current_dependent->next;
        current_dependent->next = current_dependent->next->next;
        /* Free the allocated memory for task's dependencie's dependent */
        free(temp);
    }
}

/**	Function: removeTask
 *	@param task (task_link)
 *  Removes a task, freeing all memory previously allocated to this
 * task and to store it in others as dependencies
 */
void removeTask(task_link task) {
    simpleList temp;

    while (task->dependencies != NULL) {
        /* Remove dependent from task->dependencie */
        removeDependent(task);
        
        temp = task->dependencies;
        task->dependencies = task->dependencies->next;
        /* Free the allocated memory for task's dependencie */
        free(temp);
    }

    freeTask(task);
}


/**	Function: removeTask
 *	@param task (task_link)
 *  Frees the allocated memory for the task itself and the description
 */
void freeTask(task_link task) {
    /* Free the allocated memory for task's description */
    free(task->description);
    /* Free the allocated memory for task itself */
    free(task);
}

/**	Function: showTask
 *	@param tasks (link_list)
 *  @param current (task_link)
 *  Prints current task accordingly
 */
void showTask(link_list tasks, task_link current) {
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
    /* Print all dependecies of current */
    while (current_dependencies != NULL) {
        printf(" %lu", current_dependencies->task->id);
        current_dependencies = current_dependencies->next;
    }

    printf("\n");
}

/**	Function: taskDescription
 *	@param buffer (string *)
 *  @return string (string)
 *  Gets description for task from buffer accordingly
 */
string taskDescription(string * buffer) {
    string string, beg, end;

    /* Get pointer to position of first " in buffer */
    beg = strstr(*buffer, "\"");
    if (beg == NULL) return NULL;
    /* Advance pointer one position in order to not save the " characther */
    beg ++;

    /* Get pointer to position of second " in buffer */
    end = strstr(beg, "\"");
    if (end == NULL) return NULL;
    /* Change second " for end of string characther */
    *end = '\0';

    /* Verify if string is superior than 8000 characthers */
    if (end - beg > MAXDESCRIPTION - 2) return NULL;

    /* Advance buffer to after description in order to get rest of arguments after */
    *buffer = end + 1;
    
    /* Allocate exactly the needed space to the description of the task */
    string = malloc(sizeof(char) * (end - beg + 1));
    /* Copy description os task to string variable */
    strcpy(string, beg);
    
    return string;
}

/**	Function: taskDependencies
 *	@param tasks (link_list)
 *  @param new_task (task_link)
 *  @param buffer (string *)
 *  @return (int)
 *  Stores all dependencies, given by user and stored in buffer, in new_task
 * accordingly taking attention to add to each dependecy new_task as dependent
 */
int taskDependencies(link_list tasks, task_link new_task, string * buffer) {
    int offset, already_exists;
    long unsigned dependencie;
    task_link searched;
    link tempNode;
    simpleList current, new_dependencie, new_dependent;

    while (** buffer != '\n') {
        already_exists = 0;
        dependencie = 0;

        /* Get dependencie id from buffer */
        sscanf(*buffer, "%lu%n", &dependencie, &offset);
        if (dependencie == 0) {
            printf("illegal arguments\n");
            return 0;
        }

        *buffer = *buffer + offset;
        tempNode = STsearch(tasks->head, dependencie);
        
        /* In case id given for dependencie doesn't exist in tasks */
        if (tempNode == NULL) {
            printf("no such task\n");

            while (new_task->dependencies != NULL) {
                /* Remove dependent from task->dependencie */
                removeDependent(new_task);
                
                current = new_task->dependencies;
                new_task->dependencies = new_task->dependencies->next;
                /* Free the allocated memory for task's dependencie */
                free(current);
            }

            return 0;
        }

        /* Verify if dependencie was already added to the dependencies of task */
        current = new_task->dependencies;
        while (current != NULL) {
            if (current->task->id == dependencie) already_exists = 1;
            current = current->next;
        }

        /* In case it wasn't previously added to dependencies already */
        if (!already_exists) {
            searched = tempNode->task;

            /* Allocate memory for the new dependencie */
            new_dependencie = malloc(sizeof(struct sList));
            new_dependencie->task = searched;
            new_dependencie->next = NULL;

            /* If list of dependencies is empty therefore this will be the first */
            if (new_task->dependencies == NULL) new_task->dependencies = new_dependencie;
            else {
                current = new_task->dependencies;
                /* Cycle through all dependents untill it's reached the end of list */
                while (current->next != NULL) current = current->next;
                /* The new dependent is added to the of the list */
                current->next = new_dependencie;
            }
            
            /* Allocate memory for the new dependencie */
            new_dependent = malloc(sizeof(struct sList));
            new_dependent->task = new_task;
            new_dependent->next = NULL;

            /* If list of dependents is empty therefore this will be the first */
            if (searched->dependents == NULL) searched->dependents = new_dependent;
            else {
                new_dependent->next = searched->dependents;
                searched->dependents = new_dependent;
            }
        }
    }

    return 1;
}

/**	Function: setupEarly_Start
 *	@param task (task_link)
 *  Calculates and stores early start of task in task
 */
void setupEarly_Start(task_link task) {
    task_link currentMax;
    simpleList current;
    
    /* If task has dependencies */
    if (task->dependencies != NULL) {
        currentMax = task->dependencies->task;
        current = task->dependencies;
        /* Find depedencie wich duration plus early start is maximum */
        while (current != NULL) {
            if (currentMax->duration + currentMax->early_start < 
            current->task->duration + current->task->early_start) 
                currentMax = current->task;
            current = current->next;
        }
        task->early_start = currentMax->duration + currentMax->early_start;
    }
    /* If task has no dependencies early start remains 0 */
    else task->early_start = 0;
}

/**	Function: setupLate_Start
 *	@param task (task_link)
 *  @param path_duration (long unsigned)
 *  Calculates and stores late start of task in task
 */
void setupLate_Start(task_link task, long unsigned path_duration) {
    simpleList current;

    /* If task doesn't have dependents late start is path duration less its duration */
    if (task->dependents == NULL) task->late_start = path_duration - task->duration;
    else {
        task->late_start = task->dependents->task->late_start - task->duration;
        current = task->dependents;
        /* Find dependent with minimum late start */
        while (current != NULL) {
            if (task->late_start > current->task->late_start - task->duration)
                task->late_start = current->task->late_start - task->duration;
            current = current->next;
        }
    }
}

/**	Function: setupPath_duration
 *	@param tasks (link_list)
 *  Calculates critical path duration accordingly and stores it in tasks->path_duration
 */
void setupPath_duration(link_list tasks) {
    link current = tasks->first;
    
    while (current != NULL) {
        if (current->task->dependents == NULL &&
            current->task->duration + current->task->early_start > tasks->path_duration)
                tasks->path_duration = current->task->duration + current->task->early_start;
        current = current->next;
    }
}