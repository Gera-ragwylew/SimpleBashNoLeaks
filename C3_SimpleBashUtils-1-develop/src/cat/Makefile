CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c11
FOLDER=tests/
TEST_CAT_FILES = $(FOLDER)test1.txt $(FOLDER)test2.txt $(FOLDER)symboltest.txt
TEST_CAT_FLAGS = -b -e -n -s -t -v

all: s21_cat test_cat

s21_cat: s21_cat.c
	$(CC) $(CFLAGS) -c s21_cat.c
	$(CC) $(CFLAGS) s21_cat.o -o s21_cat

test_cat:
	@for file in $(TEST_CAT_FILES); do \
		for flag in $(TEST_CAT_FLAGS); do \
			echo "Running test: cat $$flag $$file"; \
			./s21_cat $$flag $$file > s21_cat_output.txt; \
			cat $$flag $$file > cat_output.txt; \
			diff s21_cat_output.txt cat_output.txt; \
			rm -rf s21_cat_output.txt cat_output.txt; \
		done; \
	done

rebuild: clean all

clean:
	rm -rf *.o s21_cat