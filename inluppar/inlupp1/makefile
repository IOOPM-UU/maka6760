CC      = gcc
CFLAGS  = -g -Wall -Wextra
CUNIT   = -lcunit

.PHONY: all test memtest clean

all: freq_count hash_table_tests linked_list_test

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

hash_table.o: hash_table.c hash_table.h hash_table_iterator.h
freq_count.o: freq_count.c hash_table.h hash_table_iterator.h
hash_table_tests.o: hash_table_tests.c hash_table.h hash_table_iterator.h
linked_list.o: linked_list.c linked_list.h
linked_list_test.o: linked_list_test.c linked_list.h

freq_count: freq_count.o hash_table.o
	$(CC) $(CFLAGS) $^ -o $@

hash_table_tests: hash_table_tests.o hash_table.o
	$(CC) $(CFLAGS) $^ -o $@ $(CUNIT)

linked_list_test: linked_list_test.o linked_list.o
	$(CC) $(CFLAGS) $^ -o $@ $(CUNIT)

hash_test: hash_table_tests
	./hash_table_tests

list_test: linked_list_test
	./linked_list_test

hash_memtest: hash_table_tests 
	valgrind --leak-check=full ./hash_table_tests

list_memtest: linked_list_test
	valgrind --leak-check=full ./linked_list_test

clean:
	rm -f *.o freq_count hash_table_tests linked_list_test
