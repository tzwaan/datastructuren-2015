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

tnode *trie_init() {
    tnode *trie = calloc(1, sizeof(tnode));
    trie->edges = calloc(ARR_SIZE + 1, sizeof(char)); // +1 because of terminating char
    trie->nodes = calloc(ARR_SIZE, sizeof(tnode*));
    trie->arrlen = ARR_SIZE;
    return trie;
}

int in_node(tnode *node, char c) {
    c = ctoc(c);
    char *ptr = strchr(node->edges, c);
    if (ptr) {
        return ptr - node->edges;
    }
    return -1;
}
void add_node(tnode *node, char c) {
    c = ctoc(c);
    if (node->nrnodes == node->arrlen) {
        node->arrlen += ARR_SIZE;
        node->edges = (char*) realloc(node->edges, (node->arrlen) * sizeof(char));
        node->nodes = (tnode**) realloc(node->nodes, (node->arrlen) * sizeof(tnode*));
    }
    node->edges[node->nrnodes] = c;
    node->nodes[node->nrnodes] = calloc(1, sizeof(tnode));
    node->nodes[node->nrnodes]->edges = calloc(ARR_SIZE + 1, sizeof(char));
    node->nodes[node->nrnodes]->nodes = calloc(ARR_SIZE, sizeof(tnode*));
    node->nodes[node->nrnodes]->arrlen = ARR_SIZE;
    node->nrnodes++;
}
void del_node(tnode *node, char c) {
    c = ctoc(c);
    int i, j;
    if ((i = in_node(node, c)) != -1) {
        free(node->nodes[i]->edges);
        free(node->nodes[i]->nodes);
        free(node->nodes[i]);

        for (j=i; j<node->nrnodes; j++) {
            node->nodes[j] = node->nodes[j+1];
            node->edges[j] = node->edges[j+1];
        }
        node->edges[j+1] = '\0';
        node->nodes[j+1] = NULL;
        node->nrnodes--;
    }
}

int trie_add(tnode* node, char* w) {
    int i = 0;
    int wi;
    while (w[i] != '\0') {
        /* convert ascii value to array index */
        if ((wi = in_node(node, w[i])) == -1) {
            add_node(node, w[i]);
            wi = in_node(node, w[i]);
        }
        node = node->nodes[wi];

        i++;
    }
    /* if word does not yet exist, set word and return 1 */
    if (!node->ends) {
        node->ends = 1;
        return 1;
    }
    return 0;
}

/* testing made clear that this was slower than the sequential solution below
 */
int trie_add_recursive(tnode *node, char *w, int depth) {
    int i;
    if (w[depth] == '\0') {
        if (!node->ends) {
            node->ends = 1;
            return 1;
        }
        return 0;
    }
    i = ctoi(w[depth]);
    if (i == -1)
        return 0;

    if (node->nodes[i])
        return trie_add_recursive(node->nodes[i], w, depth+1);
    else {
        node_add(node, i);
        return trie_add_recursive(node->nodes[i], w, depth+1);
    }
}

void node_add(tnode *node, int i) {
    node->nodes[i] = calloc(1, sizeof(tnode));
    node->nodes[i]->nodes = calloc(26, sizeof(tnode*));
}

tnode *trie_lookup(tnode *node, char* w) {
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

int trie_remove(tnode *n, char *w) {
    int wi;
    if (*w == '\0') {
        n->ends = 0;
        if (n->nrnodes)
            return 0;
        return 1;
    }
    if ((wi = in_node(n, *w)) != -1) {
        if (trie_remove(n->nodes[wi], w+1)) {
            del_node(n, *w);
            if (n->nrnodes)
                return 0;
            return 1;
        }
    }
    return 0;
}

void trie_prefix(tnode* t, char* prefix) {
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

void trie_print(tnode *t) {
    char *w = malloc(sizeof(char));
    *w = '\0';
    trie_node_print(t, w, 0);
    printf("\n");
}
void trie_node_print(tnode* n, char *word, int length) {
    int i;
    char *w;
    if (n->ends) {
        printf("%s\n", word);
    }
    for (i=0; i<n->nrnodes; i++) {
        if (n->nodes[i]) {
            w = malloc(sizeof(char) + length + 1);
            strncpy(w, word, length);
            w[length] = n->edges[i];
            w[length+1] = '\0';
            trie_node_print(n->nodes[i], w, length+1);
        }
    }
    free(word);
}

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

int trie_cleanup(tnode *n) {
    int i,
        total = 0;
    for (i=0; i<n->nrnodes; i++) {
        if (n->nodes[i])
            total += trie_cleanup(n->nodes[i]);
    }
    if (n->ends)
        total++;
    free(n->nodes);
    free(n->edges);
    free(n);
    return total;
}

int ctoi(char c) {
    if (c>96 && c<123)
        return c-97;
    else if (c>64 && c<91)
        return c-65;
    return -1;
}
int ctoc(char c) {
    if (c>64 && c<91)
        return c+32;
    return c;
}
