struct tnode;
typedef struct tnode tnode;

/* Initialise a trie data structure.
 * Returns a pointer to the first node.
 */
tnode *trie_init();

/* Checks if a character edge c is in node n
 * Returns the index to edge c or -1 if not found.
 */
int in_node(tnode *n, char c);

/* Add word w to trie t.
 * Returns the number of times this word was added.
 */
int trie_add(tnode *t, char *w);

/* Adds node with edge c to node n.
 * Reallocs arraysize if array is full.
 */
void add_node(tnode *n, char c);

/* Delete from node n the child with edge c.
 */
void del_node(tnode *n, char c);

/* Lookup word w in trie t.
 * Returns a pointer to the endnode of w.
 */
tnode *trie_lookup(tnode *n, char *w);

/* Remove word w from trie t.
 * Returns true if node t had no more children.
 */
int trie_remove(tnode *t, char *w);

/* Print all the words in trie t starting with prefix.
 */
void trie_prefix(tnode *t, char *prefix);

/* Print all the words in trie t.
 */
void trie_print(tnode *t);

/* Recursive print function used by trie_print
 */
void trie_node_print(tnode *n, char *w, int l);

/* Count words in the trie.
 * Returns the amount of words in the trie.
 */
int trie_count(tnode *t);

/* Cleanup trie.
 * Returns the number of words free'd.
 */
int trie_cleanup(tnode *t);

/* Sorts the trie based on the most occuring letters
 * Returns the amount of times a word ends under the current node
 */
int trie_sort(tnode *n);

/* Sorts the trie alphabetically
 */
void trie_sort_alphabet(tnode *t);

/* Converts uppercase characters to lowercase.
 */
int ctoc(char c);
