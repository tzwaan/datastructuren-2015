/* Decoder Assignment - Datastructuren
 *
 * Tijmen Zwaan
 * 10208917
 */
struct list {
    int size;
    struct node *head;
};

struct node {
    struct node *next;
    char *content;
};

typedef struct list list;
typedef struct node node;

/* Intialize and return a list struct */
list *list_init();

/* Prepend a node to the list with given 'content'*/
void list_add(list*, char*);

/* Returns a pointer to the content of
 * the node in the list at 'index' */
char *list_at_index(list*, int);

/* Prints the entire list to the screen
 * including list indexes */
void list_print(list *);

/* Removes the node in the list at 'index' */
void list_remove_at_index(list*, int);

/* Find and remove the given word from the list */
void list_remove(list*, char*);

/* Free the entire list and all its contents */
void destroy_list(list*);
