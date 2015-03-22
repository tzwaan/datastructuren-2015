#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <assert.h>
#include <string.h>
#include <time.h>

#include "trie.h"

#define BUF_SIZE 10000
#define DELIM "-!?\",. \n"

void usage(char* prog) {
    fprintf(stderr, "Usage %s\n"
                    "         [-p] prefix   : Print words with prefix\n"
                    "         [-r] rmfile   : Remove words in rmfile\n"
                    "         [-P]          : Print trie\n"
                    "         [-s]          : Sort the trie alphabetically\n"
                    "         [-S]          : Sort the trie by frequency\n"
                    "         [-L] limit    : Limit the amount of words added from input by limit\n"
                    "         filename\n", prog);
    exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[]) {
    FILE *infile, *rmfile;
    char *w = NULL, *prefix = NULL, *rmfile_name = NULL;
    char *linebuf;
    int opt, added = 0, print = 0, sort = 0, sort_alphabet = 0, limit = 0,
        nr_words = 0;
    clock_t start, end;
    double adding, removing, finding, cleaning;
    struct tnode *t;

    /* Handle command line arguments. */
    while ((opt = getopt(argc, argv, "Pp:r:SsL:")) != -1) {
        switch(opt) {
            case 'S':
                sort = 1;
                break;
            case 's':
                sort_alphabet = 1;
                break;
            case 'p':
                prefix = optarg;
                break;
            case 'P':
                print = 1;
                break;
            case 'r':
                rmfile_name = optarg;
                break;
            case 'L':
                limit = atoi(optarg);
                break;
            default: /* '?' */
                usage(argv[0]);
        }
    }
    if (optind >= argc) {
        printf("ik kom hier \n");
        usage(argv[0]); // expect at least input filename.
    }

    /* Open input file. */
    infile = fopen(argv[optind], "r");
    if (!infile) {
        perror(argv[optind]);
        exit(EXIT_FAILURE);
    }

    /* Initialize the trie. */
    t = trie_init();

    /* Read lines from infile, tokenize every line and add
     * each word to the trie. */
    linebuf = malloc(BUF_SIZE);
    assert(linebuf);
    start = clock();
    while (fgets(linebuf, BUF_SIZE, infile) != NULL) {
        w = strtok(linebuf, DELIM); // strtok receives the new line of text.
        while (w && (nr_words < limit || !limit)) {
            if (trie_add(t, w) == 1)
                added += 1;
            w = strtok(NULL, DELIM); // get next token from current line.
            nr_words++;
        }
    }
    end = clock();

    adding = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Words added to trie: %d:\n", added);
    printf("Number of words in trie: %d\n", trie_count(t));

    if (sort) {
        printf("Sorting the trie\n");
        trie_sort(t);
    }

    if (sort_alphabet) {
        printf("Sorting alphabetically\n");
        trie_sort_alphabet(t);
    }


    if (prefix) {
        if (print) {
            printf("All words stored:\n");
            trie_print(t);
        }
        start = clock();
        printf("Prefixes of: %s\n", prefix);
        trie_prefix(t, prefix);
        end = clock();
        finding = (double)(end - start) / CLOCKS_PER_SEC;
    }

    if (rmfile_name) {
        rmfile = fopen(rmfile_name, "r");
        if (!rmfile) {
            perror(argv[optind]);
            exit(EXIT_FAILURE);
        }

        start = clock();
        while (fgets(linebuf, BUF_SIZE, rmfile) != NULL) {
            w = strtok(linebuf, DELIM); // strtok receives the new line of text.
            while (w) {
                trie_remove(t, w);
                w = strtok(NULL, DELIM); // get next token from current line.
            }
        }
        end = clock();
        removing = (double)(end - start) / CLOCKS_PER_SEC;
    }

    if (print) {
        printf("All words stored:\n");
        trie_print(t);
    }

    /* Cleanup */
    start = clock();
    free(linebuf);
    fclose(infile);
    printf("Number of words freed: %d\n", trie_cleanup(t));
    end = clock();
    cleaning = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\n\nAdding words took %f seconds\n", adding);
    printf("Printing suggestions took %f seconds\n", finding);
    printf("Removing words took %f seconds\n", removing);
    printf("Cleaning everything took %f seconds\n", cleaning);
    return EXIT_SUCCESS;
}
