/* Decoder Assignment - Datastructuren
 *
 * Tijmen Zwaan
 * 10208917
 */
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "ll.h"

/* Intialize and return a list struct */
list *list_init() {
    list *list = malloc(sizeof(list));

    list->size = 0;
    list->head = NULL;

    return list;
}

/* Prepend a node to the list with given 'content'*/
void list_add(list *list, char *content) {
    if (!list) {
        printf("List is NULL!\n");
        return;
    }
    node *new_node = malloc(sizeof(node));

    new_node->content = content;
    new_node->next = list->head;

    list->head = new_node;
    list->size++;
}

/* Returns a pointer to the content of
 * the node in the list at 'index' */
char *list_at_index(list *list, int index) {
    if (!list) {
        printf("List is NULL!\n");
        return NULL;
    }
    if (index >= list->size) {
        printf("Index %d not in list\n", index);
        return NULL;
    }
    int i;
    node *current = list->head;
    for (i=0; i<index; i++) {
        current = current->next;
    }
    return current->content;
}

/* Prints the entire list to the screen
 * including list indexes */
void list_print(list *list) {
    if (!list) {
        printf("List is NULL!\n");
        return;
    }
    if (!list->size) {
        printf("List is empty\n");
        return;
    }
    int i;
    node *current = list->head;
    for (i=0; i<list->size; i++) {
        printf("%d: %s\n", i, current->content);
        current = current->next;
    }
}

/* Removes the node in the list at 'index' */
void list_remove_at_index(list* list, int index) {
    if (!list) {
        printf("\nList is NULL!\n");
        return;
    }
    if (index >= list->size) {
        printf("Index %d not in list\n", index);
        return;
    }
    int i;
    node *to_be_removed,
         *current = list->head;

    /* different behaviour if index is 0,
     * since the first element will be gone, and
     * the main list struct needs to be updated */
    if (!index) {
        list->head = current->next;
        list->size--;
        free(current->content);
        free(current);
        return;
    }

    /* go the the list entry before the one to be removed */
    for (i=0; i<index-1; i++) {
        current = current->next;
    }
    to_be_removed = current->next;
    current->next = to_be_removed->next;
    list->size--;

    free(to_be_removed->content);
    free(to_be_removed);
}

/* Find and remove the given word from the list */
void list_remove(list *list, char *word) {
    if (!list) {
        printf("\nList is NULL!\n");
        return;
    }
    if (!list->size) {
        printf("List is empty\n");
        return;
    }
    int i;
    node *current = list->head,
         *prev = NULL;

    for (i=0; i<list->size; i++) {
        if (!strcmp(word, current->content)) {
            /* If there is no previous node,
             * the main list struct must be updated */
            if (!prev) {
                list->head = current->next;
                list->size--;
                free(current->content);
                free(current);
                return;
            }
            prev->next = current->next;
            free(current->content);
            free(current);
            list->size--;
            return;
        }
        prev = current;
        current = current->next;
    }
}

/* Free the entire list and all its contents */
void destroy_list(list *list) {
    int i;
    node *current = list->head,
         *next;
    for (i=0; i<list->size; i++) {
        next = current->next;
        free(current->content);
        free(current);
        current = next;
    }

    free(list);
}
