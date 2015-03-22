#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "trie.h"

#define ARR_SIZE 2

struct tnode {
    int ends,
        arrlen,
        nrnodes;
    char *edges;
    tnode **nodes;
};

/* Initialise a trie data structure.
 * Returns a pointer to the first node.
 */
tnode *trie_init() {
    tnode *trie = calloc(1, sizeof(tnode));
    trie->edges = calloc(ARR_SIZE + 1, sizeof(char)); // +1 because of terminating char
    trie->nodes = calloc(ARR_SIZE, sizeof(tnode*));
    trie->arrlen = ARR_SIZE;
    return trie;
}

/* Checks if a character edge c is in node n
 * Returns the index to edge c or -1 if not found.
 */
int in_node(tnode *node, char c) {
    c = ctoc(c);
    char *ptr = strchr(node->edges, c);
    if (ptr) {
        return ptr - node->edges;
    }
    return -1;
}

/* Adds node with edge c to node node.
 * Reallocs arraysize if array is full.
 */
void add_node(tnode *node, char c) {
    c = ctoc(c);
    /* if the node array is full. Resize both arrays by ARR_SIZE */
    if (node->nrnodes == node->arrlen) {
        node->arrlen += ARR_SIZE;
        node->edges = (char*) realloc(node->edges, (node->arrlen + 1) * sizeof(char));
        node->nodes = (tnode**) realloc(node->nodes, (node->arrlen) * sizeof(tnode*));
    }
    node->edges[node->nrnodes] = c;
    node->nodes[node->nrnodes] = calloc(1, sizeof(tnode));
    node->nodes[node->nrnodes]->edges = calloc(ARR_SIZE + 1, sizeof(char));
    node->nodes[node->nrnodes]->nodes = calloc(ARR_SIZE, sizeof(tnode*));
    node->nodes[node->nrnodes]->arrlen = ARR_SIZE;
    node->nrnodes++;
    /* set next value to terminating char because realloc doesn't set
     * the memory to zero
     */
    node->edges[node->nrnodes] = '\0';
}

/* Delete from node n the child with edge c.
 */
void del_node(tnode *node, char c) {
    c = ctoc(c);
    int i, j;
    if ((i = in_node(node, c)) != -1) {
        free(node->nodes[i]->edges);
        free(node->nodes[i]->nodes);
        free(node->nodes[i]);

        /* Shift entries in both arrays to fill the gap. */
        for (j=i; j<node->nrnodes-1; j++) {
            node->nodes[j] = node->nodes[j+1];
            node->edges[j] = node->edges[j+1];
        }
        node->edges[j+1] = '\0';
        node->nodes[j+1] = NULL;
        node->nrnodes--;
    }
}

/* Add word w to trie t.
 * Returns the number of times this word was added.
 */
int trie_add(tnode *node, char *w) {
    int i = 0;
    int wi;
    while (w[i] != '\0') {
        /* checks if char is in current node and gets the index */
        if ((wi = in_node(node, w[i])) == -1) {
            /* adds new node if char is not in the current node */
            add_node(node, w[i]);
            wi = in_node(node, w[i]);
        }
        node = node->nodes[wi];

        i++;
    }
    node->ends++;
    return node->ends;
}

/* Lookup word w in trie node.
 * Returns a pointer to the endnode of w.
 */
tnode *trie_lookup(tnode *node, char *w) {
    int i = 0,
        wi;
    while (w[i] != '\0') {
        if ((wi = in_node(node, w[i])) == -1)
            return NULL;

        node = node->nodes[wi];

        i++;
    }
    return node;
}

/* Remove word w from trie t.
 * Returns true if node t had no more children.
 */
int trie_remove(tnode *n, char *w) {
    int wi;
    if (*w == '\0') {
        n->ends = 0;
        return !n->nrnodes;
    }
    if ((wi = in_node(n, *w)) != -1) {
        /* If child node contains no more word endings, remove the node */
        if (trie_remove(n->nodes[wi], w+1)) {
            del_node(n, *w);
            return !n->nrnodes;
        }
    }
    return 0;
}

/* Print all the words in trie t starting with prefix.
 */
void trie_prefix(tnode *t, char *prefix) {
    /* first lookup the end node of prefix */
    tnode *node = trie_lookup(t, prefix);
    char *w;
    int length;
    if (!node) {
        printf("Word not found in Trie\n");
        return;
    }
    length = (int)strlen(prefix);
    w = malloc(sizeof(char) * (length + 1));
    strcpy(w, prefix);
    trie_node_print(node, w, length);
    printf("\n");
}

/* Print all the words in trie t.
 */
void trie_print(tnode *t) {
    char *w = malloc(sizeof(char));
    *w = '\0';
    trie_node_print(t, w, 0);
    printf("\n");
}

/* Recursive print function used by trie_print
 */
void trie_node_print(tnode* n, char *word, int length) {
    int i;
    char *w;
    if (n->ends) {
        printf("%s\n", word);
    }
    /* copy word for each of its children
     * then call trie_node_print for the next depth. */
    for (i=0; i<n->nrnodes; i++) {
        w = malloc(sizeof(char) + length + 1);
        strncpy(w, word, length);
        w[length] = n->edges[i];
        w[length+1] = '\0';
        trie_node_print(n->nodes[i], w, length+1);
    }
    /* each child frees the word copied by its parent */
    free(word);
}

/* Count words in the trie.
 * Returns the amount of words in the trie.
 */
int trie_count(tnode *n) {
    int i,
        total = 0;
    for (i=0; i<n->nrnodes; i++) {
        total += trie_count(n->nodes[i]);
    }
    if (n->ends)
        total++;
    return total;
}

/* Cleanup trie.
 * Returns the number of words free'd.
 */
int trie_cleanup(tnode *n) {
    int i,
        total = 0;
    for (i=0; i<n->nrnodes; i++) {
        total += trie_cleanup(n->nodes[i]);
    }
    if (n->ends)
        total++;
    free(n->nodes);
    free(n->edges);
    free(n);
    return total;
}

/* Sorts the trie alphabetically
 */
void trie_sort_alphabet(tnode *n) {
    int i, j;
    char tempc;
    tnode *tempt;

    /* bubblesort the edges */
    for (i=0; i<n->nrnodes; i++) {
        /* call sort for each of children */
        trie_sort_alphabet(n->nodes[i]);
        for (j=i; j>0; j--) {
            if (n->edges[j] < n->edges[j-1]) {
                tempc = n->edges[j];
                tempt = n->nodes[j];

                n->edges[j] = n->edges[j-1];
                n->nodes[j] = n->nodes[j-1];

                n->edges[j-1] = tempc;
                n->nodes[j-1] = tempt;
            }
            else
                break;
        }
    }
}

/* Sorts the trie based on the most occuring letters
 * Returns the amount of times a word ends under the current node
 */
int trie_sort(tnode *n) {
    int *priority,
        i, j, temp,
        total = 0;
    char tempc;
    tnode *tempt;
    if (!n->nrnodes)
        return n->ends;
    total += n->ends;

    priority = malloc(n->nrnodes * sizeof(int));
    /* bubblesort based on priority of children */
    for (i=0; i<n->nrnodes; i++) {
        /* priority is the amount of words ending under a node.
         * One word can end multiple times if it was added multiple times */
        priority[i] = trie_sort(n->nodes[i]);
        total += priority[i];
        for (j=i; j>0; j--) {
            if (priority[j] > priority[j-1]) {
                temp = priority[j];
                tempc = n->edges[j];
                tempt = n->nodes[j];

                priority[j] = priority[j-1];
                n->edges[j] = n->edges[j-1];
                n->nodes[j] = n->nodes[j-1];

                priority[j-1] = temp;
                n->edges[j-1] = tempc;
                n->nodes[j-1] = tempt;
            }
            else
                break;
        }
    }
    free(priority);

    return total;
}

/* Converts uppercase characters to lowercase.
 */
int ctoc(char c) {
    if (c>64 && c<91)
        return c+32;
    return c;
}
