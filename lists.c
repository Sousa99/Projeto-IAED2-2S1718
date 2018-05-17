#include "lists.h"
#include "btree.h"

/**	Function: createNode
 *	@param new_task (task_link)
 *  Creates and returns a new node in order to add task to tasksList after
 */
link createNode(task_link new_task) {
    link new_node;

    /* Allocates memory for a node storing in it new_task */
    new_node = malloc(sizeof(struct node));
    new_node->task = new_task;
    new_node->next = NULL;
    new_node->prev = NULL;

    return new_node;
}

/**	Function: removeNode
 *	@param tasksList (link_list)
 *  @param searched (link)
 *  Removes a node (searched) from tasksList
 */
void removeNode(link_list tasksList, link searched) {
    link current = tasksList->first;

    /* If tasksList has only one node and therefore list will become empty */
    if (tasksList->first == tasksList->last) {
        tasksList->first = NULL;
        tasksList->last = NULL;
    }
    /* Else if element wished to be removed is in the first position of tasksList */
    else if (tasksList->first == searched) {
        tasksList->first = searched->next;
        searched->next->prev = NULL;
    }
    /* Else if element wished to be removed is in the last position of tasksList */
    else if (tasksList->last == searched) {
        tasksList->last = searched->prev;
        searched->prev->next = NULL;
    }
    else {
        while (current != searched) current = current->next;
        current->prev->next = current->next;
        current->next->prev = current->prev;
    }

    /* Free the alocated memory to the node searched */
    free(searched);
}

/**	Function: createList
 *	@return tasksList (link_list)
 *  Creates and returns a double linked list where tasks will be stored
 */
link_list createList() {
    link_list tasksList;

    /* Allocate memory needed to the double linked list */
    tasksList = malloc(sizeof(struct list));
    tasksList->first = NULL;
    tasksList->last = NULL;
    tasksList->path = 0;
    /* Initialize binary tree where tasks will also be stored */
    STinit(&tasksList->head);

    return tasksList;
}