# Webstore
Authors: Alva Fällman Nilsson & Markus Karlsson

A terminal webstore backend and menu-driven frontend, written in C.
It manages merchandise, the shelves it is stored on, and shopping carts.

## Build and test

Requirements: `gcc`, `make`, the [CUnit](http://cunit.sourceforge.net/) library
(`libcunit1-dev` on Debian/Ubuntu), and optionally `valgrind` and `gcov`.

```
make test       # build and run all unit tests (cart_tests, merch_tests and webstore_tests)
make memtest    # the same tests under valgrind
```

### Code coverage

Measured with `gcov` 15.2.0 (GCC 15.2.0). The tests were built with
`--coverage` and run with `make test`, then `gcov -b` was run on each `.c`
file. "Branch" is the share of branches taken at least once.

| File                                      | Line coverage | Branch coverage |
|-------------------------------------------|--------------:|----------------:|
| `logic/cart.c`                            | 91.5 %        | 88.5 %          |
| `logic/merch.c`                           | 92.0 %        | 79.7 %          |
| `logic/webstore.c`                        | 100 %         | (no branches)   |
| `generic_data_structures/hash_table.c`    | 78.0 %        | 62.5 %          |
| `generic_data_structures/linked_list.c`   | 55.8 %        | 40.0 %          |
| `generic_utils/hash_funcs.c`              | 100 %         | 100 %           |
| `generic_utils/utils.c`                   | 0 %           | 0 %             |
| `interface/*.c`                           | 0 %           | 0 %             |

The tests cover the backend in `logic/`. The hash table and linked list are
only tested indirectly through it, and `utils.c` and `interface/` read from
the keyboard and have no unit tests.

In `ioopm_list_remove` (`linked_list.c`) the redundant condition
`ioopm_list_size(list) == 1` is commented out, since it can never be false
there, and would otherwise leave a branch that cannot be taken.

## Run the program

```
make ui
```

This builds `ui_c` and starts it. It can also be started directly with
`./ui_c` after `make ui_c`. The program takes no arguments and needs no
input files: the store starts empty and everything is entered through the menu.
Nothing is saved when the program quits.

| Key | Action                                                                          |
|-----|---------------------------------------------------------------------------------|
| `A` | Add merchandise (name, description, price)                                      |
| `L` | List merchandise (20 at a time)                                                 |
| `D` | Delete merchandise                                                              |
| `E` | Edit merchandise                                                                |
| `S` | Show the shelves and stock of a merchandise                                     |
| `P` | Replenish: add stock on a shelf. A shelf name is a capital letter and two digits, e.g. `A01` |
| `C` | Create a cart (its number is printed)                                           |
| `R` | Remove a cart                                                                   |
| `+` | Add an item to a cart (cannot exceed the stock)                                 |
| `-` | Remove an item from a cart                                                      |
| `=` | Calculate the cost of a cart                                                    |
| `O` | Check out a cart: the stock is decreased and the cart is removed                |
| `Q` | Quit                                                                            |

The file `interface/webstoretest.txt` is an example of menu input that ends
with a quit. Try it with `./ui_c < interface/webstoretest.txt`.

## Ownership of generic data structures 
The modules hash_table and linked_list were taken from Markus Karlssons sprint 1,
and Utils were taken fron Alva Fällman Nilssons sprint 1.

## Documentation

The public functions are documented in the header files (Doxygen style):
`logic/` (the backend: `webstore.h`, `merch.h`, `cart.h`),
`interface/` (the menu frontend), `generic_data_structures/` (hash table and
linked list) and `generic_utils/` (input helpers and hash functions).

Run `make clean` to remove built files.

## AI
AI (Claude Pro) was used to create 