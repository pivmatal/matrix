CC = gcc
FLAG_C = -c
CFLAGS = -Wall -Werror -Wextra -std=c11
SRC_C = s21_*.c
SRC_O = s21_*.o
SRC_COV = cov_*.o
GKOVFLAGS = -lgcov --coverage
TESTS_SOURSES = ./matrix.check
CHECKFLAGS = -lcheck
OS = $(shell uname)
ifeq ($(OS), Linux)
CHECKFLAGS += -lm -lpthread -lsubunit -lrt
endif

all: clean s21_matrix.a test clean

s21_matrix.a:
	$(CC) $(FLAG_C) $(CFLAGS) $(SRC_C)
	ar rc s21_matrix.a $(SRC_O)
	ranlib s21_matrix.a

test: clean s21_matrix.a
	checkmk clean_mode=1 matrix.check s21_matrix.h > test.c
	$(CC) test.c $(CHECKFLAGS) -lm -lcheck s21_matrix.a -o test
	./test

gcov_report: 
	$(CC) -fprofile-arcs -ftest-coverage test.c s21_matrix.c -o gcov_report $(CHECKFLAGS)
	./gcov_report
	lcov -t "test" -o test.info -c -d .
	genhtml -o report test.info

report: test gcov_report open

open:
	open -a "Google Chrome" report/src/index.html

clean:
	rm -rf test.c *.o test
	rm -rf $(SRC_O) *.a
	rm -rf *.gcov *.gcda *.gcno *.out *.info *.txt
	rm -rf gcov_report

style:
	cp ../materials/linters/.clang-format .
	clang-format -n *.c *.h

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem --inconclusive --check-config *.c *.h

leaks: test
	CK_FORK=no leaks --atExit -- ./test

valgrind: test
	CK_FORK=no valgrind --tool=memcheck --leak-check=yes --show-leak-kinds=all ./test
