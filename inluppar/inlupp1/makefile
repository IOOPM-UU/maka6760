CC      = gcc
CFLAGS  = -g -Wall -Wextra -pg
GCOV = --coverage
CUNIT   = -lcunit

BIN = binaries
ANA = analysis_files
TESTS = hash_table_tests linked_list_tests

.PHONY: all test memtest clean coverage

all: freq_count $(TESTS)

WORD = ord

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

hash_table.o: hash_table.c hash_table.h hash_table_iterator.h common.h
freq_count.o: freq_count.c hash_table.h hash_table_iterator.h common.h
hash_table_tests.o: hash_table_tests.c hash_table.h hash_table_iterator.h common.h
linked_list.o: linked_list.c linked_list.h common.h
linked_list_tests.o: linked_list_tests.c linked_list.h common.h

freq_count: freq_count.o hash_table.o
	$(CC) $(CFLAGS) $^ -o $@

hash_table_tests: hash_table_tests.o hash_table.o
	$(CC) $(CFLAGS) $^ -o $@ $(CUNIT)

linked_list_tests: linked_list_tests.o linked_list.o
	$(CC) $(CFLAGS) $^ -o $@ $(CUNIT)

# --- TESTS ---
hash_test: hash_table_tests
	./hash_table_tests

list_test: linked_list_tests
	./linked_list_tests

# --- MEMTESTS ---
hash_memtest: hash_table_tests 
	valgrind --leak-check=full ./hash_table_tests

list_memtest: linked_list_tests
	valgrind --leak-check=full ./linked_list_tests

# --- COVERAGE ---
coverage:
	mkdir -p $(BIN) $(ANA)
	rm -rf $(BIN)/* $(ANA)/*
	for t in $(TESTS); do \
		src=$${t%_test*}; \
		$(CC) $(CFLAGS) $(GCOV) -dumpdir $(ANA)/ $$t.c $$src.c -o $(BIN)/$$t $(CUNIT) || exit 1; \
		./$(BIN)/$$t > /dev/null || exit 1; \
		gcov -b -o $(ANA) $$src.c; \
	done
	mv *.gcov $(ANA)

# --- PROFILING ---
show_result: freq_count
	gprof ./freq_count gmon.out

# --- CLEAN ---
clean:
	rm -f *.o freq_count $(TESTS)
	rm -rf $(BIN)/* $(ANA)/*
