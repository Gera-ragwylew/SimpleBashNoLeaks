CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c11
FOLDER=tests/
TEST_GREP_FILES = $(FOLDER)test_grep2.txt
TEST_GREP_FLAGS = -e -i -v -c -l -n -h -s -o

all: s21_grep test_grep extended_test_grep1 extended_test_grep2 extended_test_grep3 extended_test_grep4 extended_test_grep5 extended_test_grep6 extended_test_grep7 extended_test_grep8 extended_test_grep9

s21_grep: s21_grep.c
	$(CC) $(CFLAGS) -c s21_grep.c 
	$(CC) $(CFLAGS) s21_grep.o -o s21_grep

test_grep:
	@for file in $(TEST_GREP_FILES); do \
		for flag in $(TEST_GREP_FLAGS); do \
			echo "Running test: grep $$flag pattern $$file"; \
			valgrind --tool=memcheck --leak-check=yes ./s21_grep $$flag Within $$file > s21_grep_output.txt; \
			grep $$flag Within $$file > grep_output.txt; \
			diff s21_grep_output.txt grep_output.txt; \
			rm -rf s21_grep_output.txt grep_output.txt; \
		done; \
	done

extended_test_grep1:
	@echo "Running test: grep -f filepattern $(FOLDER)test_grep.txt"
	@valgrind --tool=memcheck --leak-check=yes ./s21_grep -f $(FOLDER)pattern.txt $(FOLDER)test_grep.txt > s21_grep_output.txt
	@grep -f $(FOLDER)pattern.txt $(FOLDER)test_grep.txt > grep_output.txt
	@diff s21_grep_output.txt grep_output.txt

extended_test_grep2:
	@echo "Running test: grep -E There [123]|We $(FOLDER)test_grep.txt"
	@valgrind --tool=memcheck --leak-check=yes ./s21_grep -E "There [123]|We" $(FOLDER)test_grep.txt > s21_grep_output.txt
	@grep -E "There [123]|We" $(FOLDER)test_grep.txt > grep_output.txt
	@diff s21_grep_output.txt grep_output.txt

extended_test_grep3:
	@echo "Running test: grep -iv pattern $(FOLDER)test_grep2.txt"
	@valgrind --tool=memcheck --leak-check=yes ./s21_grep -iv within $(FOLDER)test_grep2.txt > s21_grep_output.txt
	@grep -iv within $(FOLDER)test_grep2.txt > grep_output.txt
	@diff s21_grep_output.txt grep_output.txt

extended_test_grep4:
	@echo "Running test: grep -in pattern $(FOLDER)test_grep2.txt"
	@valgrind --tool=memcheck --leak-check=yes ./s21_grep -in within $(FOLDER)test_grep2.txt > s21_grep_output.txt
	@grep -in within $(FOLDER)test_grep2.txt > grep_output.txt
	@diff s21_grep_output.txt grep_output.txt

extended_test_grep5:
	@echo "Running test: grep -e pattern -e pattern -e pattern $(FOLDER)test_grep2.txt"
	@ valgrind --tool=memcheck --leak-check=yes ./s21_grep -e within -e And -e lolka $(FOLDER)test_grep2.txt > s21_grep_output.txt
	@grep -e within -e And -e lolka $(FOLDER)test_grep2.txt > grep_output.txt
	@diff s21_grep_output.txt grep_output.txt

extended_test_grep6:
	@echo "Running test: grep -h pattern $(FOLDER)test_grep2.txt $(FOLDER)test_grep.txt"
	@valgrind --tool=memcheck --leak-check=yes ./s21_grep -h within $(FOLDER)test_grep2.txt $(FOLDER)test_grep.txt > s21_grep_output.txt
	@grep -h within $(FOLDER)test_grep2.txt $(FOLDER)test_grep.txt > grep_output.txt
	@diff s21_grep_output.txt grep_output.txt

extended_test_grep7:
	@echo "Running test: grep -of filepattern $(FOLDER)test_grep.txt"
	@valgrind --tool=memcheck --leak-check=yes ./s21_grep -of $(FOLDER)pattern.txt $(FOLDER)test_grep.txt > s21_grep_output.txt
	@grep -of $(FOLDER)pattern.txt $(FOLDER)test_grep.txt > grep_output.txt
	@diff s21_grep_output.txt grep_output.txt

extended_test_grep9:
	@echo "Running test: grep -vn pattern $(FOLDER)empty.txt"
	@valgrind --tool=memcheck --leak-check=yes ./s21_grep -vn within $(FOLDER)empty.txt > s21_grep_output.txt
	@grep -vn within $(FOLDER)empty.txt > grep_output.txt
	@diff s21_grep_output.txt grep_output.txt

extended_test_grep8:
	@echo "Running test: grep -hoeivns pattern $(FOLDER)test_grep2.txt $(FOLDER)test_grep.txt"
	@valgrind --tool=memcheck --leak-check=yes ./s21_grep -clhoeivns within $(FOLDER)test_grep2.txt $(FOLDER)test_grep.txt > s21_grep_output.txt
	@grep -clhoEivns within $(FOLDER)test_grep2.txt $(FOLDER)test_grep.txt > grep_output.txt
	@diff s21_grep_output.txt grep_output.txt

rebuild: clean all

clean:
	rm -rf *.o s21_grep s21_grep_output.txt grep_output.txt