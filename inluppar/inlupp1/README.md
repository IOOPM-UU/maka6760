Inlupp 1, Vilgot Lenninger and Markus Karlsson.

This project implements linked structures as a hash table and a list that can store elements as follows:
- int
- unsigned int
- bool
- float
- void *
- char *

Inlupp 1 contains tests for the hash table and the linked list as well as freq_count which counts words in a textfile by using a hash table.

## Preconditions
- gcc
- valgrind
- CUnit
- make

## Compile and run the tests
Open terminal

Go to folder "inluppar/inlupp1" 

Write 'make' in your terminal to create and compile. 

You can now run one of the following *tests*;

- make hash_test

- make list_test

- make hash_memtest

- make list_memtest.

*Note that the memtests are ran with Valgrind to detect memory leaks* 

Use make clean to remove .o-files.

## Ownership
Neither the hash table nor the list takes ownership of keys or values. They are never copied or freed by the data structures; the caller is responsible for that.

## Errors
The user can input negative indices for the list, but because the index is typed 
as size_t, a negative value will turn into a VERY large positive value which will be discarded.
In linked_list:
- get, remove, insert and lookup return false on an invalid index, or an invalid key.
- head and last on and empty list is undefined behaviour. This is a pre-condition
in the header file.
- If malloc or calloc fails, there is no control for this.
- freq_count prints an error message and skips the file if it can not be opened. 

## Code Coverage

Run:
    make coverage

This compiles each test program with "--coverage", runs it, and runs "gcov -b"
on each implementation file. Binaries end up in "binaries/" and all coverage files
in "analysis-files/". Both folders are emptied at the start of every run, and by "make clean."

Tool: gcov, with "-b" for branch coverage

| File            | Line coverage      | Branch coverage  |
|-----------------|--------------------|------------------|
| `hash_table.c`  | 99.26 %            | 94.44 %          |
| `linked_list.c` | 100.00 %           | 100.00 %         |

Branch coverage is gcov's "Taken at least once".

### Changes made to improve coverage

The first coverage run showed a few uncovered lines and branches. Some were missing tests,
other were conditions that could never be false:

**Added tests**
- "ioopm_list_iterator_insert" was never called with the iterator at the first
element or at the end of the list, so the code updating "first" and "last" never ran.
Added "test_iterator_insert_at_front" and "test_iterator_insert_at_end".
- "ioopm_hash_table_is_empty" was only called on empty tables. Added
"test_is_empty_on_non_empty_table".

**Removed unreachable conditions**
- "ioopm_list_remove": the "else if" for removing the only element checked
"index == 0 && size == 1". Any list with "index == 0" and size > 1 is already handled
by the "if" above, so "size == 1" was always true there. The check was removed;
the branch is now "else if (index == 0)".
- "ioopm_hash_table_iterator_at_end": the condition checked "current_entry == && current_bucket == No_Buckets". The iterator only reaches "current_buckets == No_Buckets" after stepping off the last bucket, and the "current_entry" is always "NULL". The "current_entry" check was removed.

The removed conditions are kept as comments in the code to show exactly what was changed.

# Initial Profiling Results
We made our profiling on freq_count using gprof. We also ran freq_count with the
*time* command. We ran it five times for every text file and saved the median.

These are the results:

1k-long-words:
1. string_eq: 3564
2. find_previous_entry: 2000
3. string_hash: 2000

Runtime (median): 
- real: 0.0028s

10k-words
1. string_eq: 115860 calls
2. find_previous_entry: 20000
3. string_hash: 20000

Runtime (median):
- real: 0.0056s

16k-words
1. string_eq: 2494074
2. cmp_freq_word: 35943
3. find_previous_entry & string_hash: 33984

Runtime (median): 
- real: 0.0434s

We can see that for 1k and 10k words, find_previous_entry & string_hash har called exactly 100% more. 
of words. When counting 16k, we see that the functions are called 106.2% times more, and we don't know why.
For some reason when count 16k words, we see a tremendous rise in the use of cmp_freq_word. It goes to 
being called just a few dozen times to almost 36.000. We suspect there is some exponential growth when reading >10k words,
thats why we see cmp_freq_word at the top and find_previous_entry/string_hash being used more than 100% of the time.
This is probably due to our limited amount of buckets, which are causing problems when dealing with larger and larger 
datasets.

We made a small test where we increased our No_Buckets to 1700. Running freq_count with *time*
gave no significant improvement for 1k words and 10k words. However, running freq_count with 16k words
and 1700 buckets gave us the following times:

- real: 0.0016s 

This is about 27 times faster. So our bottleneck is clearly the number of buckets in our table. 

After implementing a dynamic number of buckets we got the following data, once 
again using gprof and *time*. 

1k-long-words:
1. string_eq: 2647
2. find_previous_entry: 2017
3. string_hash: 2017

Runtime (median): 
- real: 0.0046s

10k-words:
1. string_eq: 26793
2. find_previous_entry: 20242
3. string_hash: 20242

Runtime (median):
- real: 0.008s

16k-words:
1. find_previous_entry: 42165
2. string_hash: 42165
3. string_eq: 41664

Runtime (median): 
- real: 0.034s

We can see a large decrease in function calls for all text files, due to the fact that
we now have a dynamic table size so we don't need to make as many searches through long linked lists.
Time-wise we can only see an improvement on the 16k words file, the others are actually slower.
We think this is due to the fact that we are making a copy of the hash table when increasing the size, 
instead of increasing the size of the existing table. 

## Memory usage 
Running freq_count with valgrinds memory profiling tool massif and cachegrind 
for simple modelling of cache behaviour gives us the following data (we look at peak values):



1k-long-words (no double pointer):
| total(B)        | useful-heap(B)     | extra-heap(B)    | I refs    | D refs    | D1 misses/D1 miss rate | LLd misses |
|-----------------|--------------------|------------------|-----------|-----------|------------------------|------------|
| 23,976          | 23,390             | 686              | 3,871,767 | 1,865,242 | 2,078/0.1%             | 1,856      |

1k-long-words (double pointer):
| total(B)        | useful-heap(B)     | extra-heap(B)    | I refs    | D refs    | D1 misses/D1 miss rate | LLd misses |
|-----------------|--------------------|------------------|-----------|-----------|------------------------|------------|
| 23,728          | 22,899             | 829              | 3,919,491 | 1,888,744 | 2,073                  | 1,850      |

Runtime (double pointer):
- real: 0.003s (vs 0.0046s)


10k-words (no double pointer);
| total(B)        | useful-heap(B)     | extra-heap(B)    | I refs    | D refs    | D1 misses/D1 miss rate | LLd misses |
|-----------------|--------------------|------------------|-----------|-----------|------------------------|------------|
| 40,976          | 43,229             | 6,747            | 15,927,526| 8,050,859 | 2,479/0,0%             | 2,137      |

10k-words (double pointer):
| total(B)        | useful-heap(B)     | extra-heap(B)    | I refs    | D refs    | D1 misses/D1 miss rate | LLd misses |
|-----------------|--------------------|------------------|-----------|-----------|------------------------|------------|
| 38,576          | 31,965             | 6,611            | 16,122,541| 8,214,292 | 2,433/0,0%             | 2,107      |

Runtime (double pointer):
- real: 0.004s (0.008s)

16k-words (no double pointer):
| total(B)        | useful-heap(B)     | extra-heap(B)    | I refs    | D refs    | D1 misses/D1 miss rate | LLd misses |
|-----------------|--------------------|------------------|-----------|-----------|------------------------|------------|
| 581,368         | 452,024            | 129,344          | 52,160,350| 27,354,305| 120,698/0,4%           | 11,575     |

16k-words (double pointer):
| total(B)        | useful-heap(B)     | extra-heap(B)    | I refs    | D refs    | D1 misses/D1 miss rate | LLd misses |
|-----------------|--------------------|------------------|-----------|-----------|------------------------|------------|
| 434,032         | 296,195            | 137,837          | 45,541,760| 23,348,940| 88,605/0,4%            | 9,011      |

Runtime (double pointer):
- real: 0.016s (vs 0.034s)

## AI 
We have used Claude Pro to improve readability in our already written text, like fixing grammar mistakes and using Markdown syntax to improve the layout of the document. 