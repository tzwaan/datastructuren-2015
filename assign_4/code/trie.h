struct tnode;
typedef struct tnode tnode;

/* Initialise a trie data structure.
 * Return: ...
 */
tnode *trie_init();
int in_node(tnode *n, char c);

/* Add word w to trie t.
 * Return: ...
 */
int trie_add_recursive(tnode *n, char *w, int i);
int trie_add(tnode *n, char *w);
void node_add(tnode *n, int i);

/* Lookup word w in trie t.
 * Return: ...
 */
tnode *trie_lookup(tnode *n, char *w);

/* Remove word w from trie t.
 * Return: ...
 */
int trie_remove(tnode *t, char *w);

/* Print all the words in trie t starting with prefix.
 */
void trie_prefix(tnode *t, char *prefix);

/* Print all the words in trie t.
 */
void trie_print(tnode *t);
void trie_node_print(tnode *n, char *w, int l);

/* Count words in the trie.
 * Return: ...
 */
int trie_count(tnode *t);

/* Cleanup trie.
 * Return: number of words free'd.
 */
int trie_cleanup(tnode *t);

int ctoi(char c);
