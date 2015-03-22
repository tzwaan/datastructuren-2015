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

sort: all war-and-peace-ascii.txt
	./autocomplete -P -S war-and-peace-ascii.txt
alpha: all war-and-peace-ascii.txt
	./autocomplete -P -s war-and-peace-ascii.txt

time: all war-and-peace-ascii.txt
	./autocomplete -L 100 war-and-peace-ascii.txt
	@echo ----------
	./autocomplete -L 200 war-and-peace-ascii.txt
	@echo ----------
	@echo
	./autocomplete -L 400 war-and-peace-ascii.txt
	@echo ----------
	@echo
	./autocomplete -L 800 war-and-peace-ascii.txt
	@echo ----------
	@echo
	./autocomplete -L 1600 war-and-peace-ascii.txt
	@echo ----------
	@echo
	./autocomplete -L 3200 war-and-peace-ascii.txt
	@echo ----------
	@echo
	./autocomplete -L 6400 war-and-peace-ascii.txt
	@echo ----------
	@echo
	./autocomplete -L 12800 war-and-peace-ascii.txt
	@echo ----------
	@echo
	./autocomplete -L 25600 war-and-peace-ascii.txt
	@echo ----------
	@echo
	./autocomplete -L 51200 war-and-peace-ascii.txt
	@echo ----------
	@echo
	./autocomplete -L 102400 war-and-peace-ascii.txt
	@echo ----------
	@echo
	./autocomplete -L 204800 war-and-peace-ascii.txt
	@echo ----------
	@echo
	./autocomplete -L 409600 war-and-peace-ascii.txt
	@echo ----------
	@echo

check: all war-and-peace-ascii.txt
	# short test
	./autocomplete -P test1.txt
	@echo
	./autocomplete -p h -P -r remove_list.txt test1.txt
	@echo
	# longer test
	./autocomplete war-and-peace-ascii.txt
	@echo
	./autocomplete -p thea -P war-and-peace-ascii.txt

clean:
	rm -f *.o autocomplete war-and-peace-ascii.txt war-and-peace-mod-the.txt
