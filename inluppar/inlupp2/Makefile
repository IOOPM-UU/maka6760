CC = gcc
CFLAGS = -Wall -Wextra -pedantic -g
IFLAGS = -Igeneric_data_structures -Ilogic -Itests -Igeneric_utils -Iinterface
CUNIT = -lcunit
VALGRIND = valgrind --leak-check=full --error-exitcode=1

COVERAGE_FLAGS = --coverage 
COVERAGE_SOURCES = hash_table.c linked_list.c cart.c merch.c webstore.c

PROFILE_FLAGS = -pg
REPORT ?= gprof.txt


C_FILES = $(wildcard */*.c)
O_FILES = $(C_FILES:.c=.o)


TESTS = cart_tests \
		merch_tests \
		webstore_tests \

.PHONY: all objects test memtest coverage clean

all: $(TESTS)

objects: $(O_FILES)

# General rule for compiling a .c file into a .o file
%.o: %.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@


#Header dependencies
generic_data_structures/hash_table.o: generic_data_structures/hash_table.h generic_data_structures/hash_table_iterator.h generic_data_structures/common.h
generic_data_structures/linked_list.o: generic_data_structures/linked_list.h generic_data_structures/list_iterator.h generic_data_structures/common.h

logic/merch.o tests/merch_tests.o: logic/merch.h generic_data_structures/linked_list.h generic_data_structures/hash_table.h generic_data_structures/common.h generic_utils/hash_funcs.h generic_utils/utils.h
logic/cart.o tests/cart_tests.o: logic/cart.h logic/merch.h generic_data_structures/linked_list.h generic_data_structures/hash_table.h generic_data_structures/common.h generic_utils/hash_funcs.h generic_utils/utils.h

generic_utils/utils.o: generic_utils/utils.h
logic/webstore.o: logic/webstore.h generic_data_structures/hash_table.h
tests/webstore_tests.o: logic/webstore.h logic/merch.h logic/cart.h generic_data_structures/hash_table.h generic_data_structures/common.h generic_utils/hash_funcs.h

generic_utils/hash_funcs.o: generic_utils/hash_funcs.h generic_data_structures/common.h
interface/merch_frontend.o: generic_data_structures/hash_table.h generic_data_structures/hash_table_iterator.h generic_data_structures/common.h \
	generic_data_structures/linked_list.h generic_data_structures/list_iterator.h logic/merch.h logic/cart.h logic/webstore.h interface/merch_frontend.h interface/cart_frontend.h \
	generic_utils/utils.h generic_utils/hash_funcs.h
interface/cart_frontend.o: generic_data_structures/hash_table.h generic_data_structures/hash_table_iterator.h generic_data_structures/common.h \
	generic_data_structures/linked_list.h generic_data_structures/list_iterator.h logic/merch.h logic/cart.h logic/webstore.h interface/merch_frontend.h interface/cart_frontend.h \
	generic_utils/utils.h generic_utils/hash_funcs.h

interface/ui.o: generic_data_structures/hash_table.h generic_data_structures/hash_table_iterator.h generic_data_structures/common.h \
	generic_data_structures/linked_list.h generic_data_structures/list_iterator.h logic/merch.h logic/cart.h logic/webstore.h interface/merch_frontend.h interface/cart_frontend.h \
	generic_utils/utils.h generic_utils/hash_funcs.h
 

#Program

# Test executables
cart_tests: tests/cart_tests.o logic/cart.o logic/merch.o generic_data_structures/hash_table.o generic_data_structures/linked_list.o logic/webstore.o generic_utils/hash_funcs.o generic_utils/utils.o
	$(CC) $(CFLAGS) $^ -o $@ $(CUNIT)

merch_tests: tests/merch_tests.o logic/merch.o generic_data_structures/hash_table.o generic_data_structures/linked_list.o logic/webstore.o generic_utils/hash_funcs.o generic_utils/utils.o
	$(CC) $(CFLAGS) $^ -o $@ $(CUNIT)


webstore_tests: tests/webstore_tests.o logic/webstore.o logic/cart.o logic/merch.o generic_data_structures/hash_table.o generic_data_structures/linked_list.o generic_utils/hash_funcs.o generic_utils/utils.o
	$(CC) $(CFLAGS) $^ -o $@ $(CUNIT)

# UI executables
ui_c:  interface/ui.o generic_utils/utils.o logic/webstore.o logic/merch.o logic/cart.o generic_data_structures/hash_table.o generic_data_structures/linked_list.o interface/cart_frontend.o interface/merch_frontend.o generic_utils/hash_funcs.o
	$(CC) $(CFLAGS) $^ -o $@

# Run all tests
test: $(TESTS) 
	./cart_tests 
	./merch_tests
	./webstore_tests

merch_test: merch_tests
	./merch_tests

cart_test: cart_tests
	./cart_tests

webstore_test: webstore_tests
	./webstore_tests


# Run all tests through Valgrind
memtest: $(TESTS)
	$(VALGRIND) ./cart_tests
	$(VALGRIND) ./merch_tests
	$(VALGRIND) ./webstore_tests

merch_memtest: merch_tests
	$(VALGRIND) ./merch_tests

cart_memtest: cart_tests
	$(VALGRIND) ./cart_tests

webstore_memtest: webstore_tests
	$(VALGRIND) ./webstore_tests

ui: ui_c
	./ui_c

# Remove generated files
clean:
	rm -f $(O_FILES) $(TESTS) ui_c
	rm -f *.gcno *gcda *.gcov gmon.out $(REPORT)
