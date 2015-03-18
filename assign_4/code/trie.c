#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "trie.h"

struct tnode {
    int ends;
    tnode **nodes;
};

tnode *trie_init() {
    tnode *trie = calloc(1, sizeof(trie));
    trie->nodes = calloc(26, sizeof(tnode*));
    return trie;
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

int trie_add(tnode* node, char* w) {
    int i = 0;
    int wi;
    while (w[i] != '\0') {
        /* convert ascii value to array index */
        wi = ctoi(w[i]);
        if (wi == -1) {
            return 0;
        }

        /* if node for index exists, go to the node */
        if (node->nodes[wi]) {
            node = node->nodes[wi];
        }
        /* else add the node */
        else {
            node_add(node, wi);
            node = node->nodes[wi];
        }

        i++;
    }
    /* if word does not yet exist, set word and return 1 */
    if (!node->ends) {
        node->ends = 1;
        return 1;
    }
    return 0;
}

void node_add(tnode *node, int i) {
    node->nodes[i] = calloc(1, sizeof(tnode));
    node->nodes[i]->nodes = calloc(26, sizeof(tnode*));
}

tnode *trie_lookup(tnode *node, char* w) {
    int i = 0,
        wi;
    while (w[i] != '\0') {
        wi = ctoi(w[i]);
        if (wi == -1) {
            return NULL;
        }

        /* if node for index exists, go to the node */
        if (node->nodes[wi]) {
            node = node->nodes[wi];
        }
        else {
            return NULL;
        }

        i++;
    }
    return node;
}

int trie_remove(tnode *n, char *w) {
    int wi, i;
    if (*w == '\0') {
        n->ends = 0;
        for (i=0; i<26; i++) {
            if (n->nodes[i]) {
                return 0;
            }
        }
        free(n->nodes);
        free(n);
        return 1;
    }
    else {
        wi = ctoi(*w);
        if (n->nodes[wi]) {
            if (trie_remove(n->nodes[wi], w+1)) {
                n->nodes[wi] = NULL;
                for (i=0; i<26; i++) {
                    if (n->nodes[i]) {
                        return 0;
                    }
                }
                free(n->nodes);
                free(n);
                return 1;
            }
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
    for (i=0; i<26; i++) {
        if (n->nodes[i]) {
            w = malloc(sizeof(char) + length + 1);
            strncpy(w, word, length);
            w[length] = i+97;
            w[length+1] = '\0';
            trie_node_print(n->nodes[i], w, length+1);
        }
    }
    free(word);
}

int trie_count(tnode *n) {
    int i,
        total = 0;
    for (i=0; i<26; i++) {
        if (n->nodes[i])
            total += trie_count(n->nodes[i]);
    }
    if (n->ends)
        total++;
    return total;
}

int trie_cleanup(tnode *n) {
    int i,
        total = 0;
    for (i=0; i<26; i++) {
        if (n->nodes[i])
            total += trie_cleanup(n->nodes[i]);
    }
    if (n->ends)
        total++;
    free(n->nodes);
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
