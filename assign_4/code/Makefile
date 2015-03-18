CC=gcc
#CC=clang

# no-unused-parameter used here to supress warnings in the incomplete
# template code. Remove this flag when you have added your own code.
CFLAGS= -std=c99 -Wall -Wextra -pedantic -Wno-unused-parameter

# debug flags.
#CFLAGS= -g3 -DDEBUG -std=c99 -Wall -Wextra -pedantic

all: autocomplete

autocomplete: autocomplete.o trie.o

autocomplete.o: trie.h

# Rule to unzip input when needed for running the checks
%.txt: %.txt.gz
	gzip -dk $<

test: all war-and-peace-ascii.txt
	./autocomplete -P test1.txt
test2: all war-and-peace-ascii.txt
	./autocomplete -P test2.txt
test3: all war-and-peace-ascii.txt
	./autocomplete -P war-and-peace-ascii.txt

check: all war-and-peace-ascii.txt
	# short test
	./autocomplete -P test1.txt
	@echo
	./autocomplete -p h -P -r remove_list.txt test1.txt
	@echo
	# longer test
	./autocomplete war-and-peace-ascii.txt
	@echo
	./autocomplete -p thea war-and-peace-ascii.txt

clean:
	rm -f *.o autocomplete war-and-peace-ascii.txt war-and-peace-mod-the.txt
