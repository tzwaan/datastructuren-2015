/* Decoder Assignment - Datastructuren
 *
 * Tijmen Zwaan
 * 10208917
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <errno.h>

#include "ll.h"
#include "debug.h"

#define BUF_SIZE 512

#define DELIM "!?\",. \n"

int main(int argc, char *argv[]) {
    FILE *infile;
    char *linebuf, *s, *e, *word;
    char sep;
    int index;
    list *list = list_init();

    if (argc < 2) {
        // No filename, read from stdin.
        infile = stdin;
    } else {
        infile = fopen(argv[1], "r");
        if (!infile) {
            perror(argv[1]);
            exit(EXIT_FAILURE);
        }
    }

    linebuf = malloc(BUF_SIZE);
    assert(linebuf);


    while (fgets(linebuf, BUF_SIZE, infile) != NULL) {
        s = linebuf; // First word starts at line beginning.
        while ((e = strpbrk(s, DELIM)) != NULL) {
            sep = *e; // Store separator
            *e = '\0'; // End word.

            /* char* s now points the current word, and sep to the
             * current separator. */

            /* if the last character of the word is '*',
             * add it to the list */
            if (*(e-1) == '*') {
                /* check if this is not a stray '*' */
                if (!strchr(DELIM, *(e-2))) {
                    word = malloc(sizeof(char) * (e-s));
                    *(e-1) = '\0';
                    strcpy(word, s);

                    list_add(list, word);
                }
                printf("%s%c", s, sep);
            }
            /* if the last character of the word is '~',
             * remove it from the list */
            else if (*(e-1) == '~') {
                /* check if this is not a stray '~' */
                if (!strchr(DELIM, *(e-2))) {
                    *(e-1) = '\0';
                    list_remove(list, s);
                }
                printf("%s%c", s, sep);
            }
            /* if the word is a number, print the corresponding
             * word from the list */
            else if ((index = atoi(s)) || *s == '0') {
                printf("%s%c", list_at_index(list, index), sep);
            }
            else {
                printf("%s%c", s, sep);
            }

            /* next word */
            s = e + 1;
        }
    }

    /* Cleanup. */
    free(linebuf);
    fclose(infile);

    destroy_list(list);

    return EXIT_SUCCESS;
}
