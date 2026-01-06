CC = gcc
#DEBUG = -O0 -g3 -fno-inline
#for debug: CK_FORK = no DEBUGINFOD_URLS="https://debuginfod.ubuntu.com" gdb ./tests/test
CFLAGS = -Wall -Wextra -Werror -std=c11
CFLAGS+ = $(DEBUG)
LIB_NAME = s21_calc.a
QT_PROJECT_DIR = ./smartcalc
QT_BUILD_DIR = ./build
DIST_ARCHIVE = smartcalc-dist.tar.gz
LIB_DIR = $(QT_PROJECT_DIR)/lib
LIB_DST_NAME = $(LIB_DIR)/lib$(LIB_NAME)
SOURCES = $(wildcard s21_*.c)
OBJECTS = $(SOURCES:.c=.o)
HEADER = s21_calc.h
CHECK_FILE = ./tests/s21_calc_test.check
LFLAGS = $(shell pkg-config --cflags --libs check)
DOXYGEN = doxygen
LATEXDIR = ./latex ./html

.PHONY: all install uninstall test gcov_report clean rebuild dvi dist remove

all: $(LIB_NAME)

install: rebuild dvi $(LIB_NAME)
	mkdir -p $(LIB_DIR)
	cp $(LIB_NAME) $(LIB_DST_NAME)
	cp $(HEADER) $(LIB_DIR)
	mkdir -p $(QT_BUILD_DIR)
	cp -r $(LATEXDIR) $(QT_BUILD_DIR)
	cd $(QT_BUILD_DIR) && qmake6 ../$(QT_PROJECT_DIR)/smartcalc.pro
	cd $(QT_BUILD_DIR) && make

uninstall: remove

$(LIB_NAME): $(OBJECTS)
	ar rcs $(LIB_NAME) $(OBJECTS)
	ranlib $(LIB_NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: rebuild
	checkmk cleanmode=1 $(CHECK_FILE) > $(CHECK_FILE:.check=.c)
	gcc --coverage $(DEBUG) -g $(SOURCES) $(CHECK_FILE:.check=.c) $(LFLAGS) -L. $(LIB_NAME) -o ./tests/test -lm
	./tests/test

gcov_report: test
	lcov -t "test" -o ./tests/gcov.info -c -d . --rc branch_coverage=1 --ignore-errors unsupported,unsupported
	genhtml -o gcov_report ./tests/gcov.info --rc branch_coverage=1 --ignore-errors range,range
	open gcov_report/index.html

clean:
	-rm -f $(OBJECTS)
	-rm -f ./tests/*.gcno ./tests/*.gcda ./tests/*.info ./tests/test ./*.gcno ./*.gcda ./*.info ./test $(CHECK_FILE:.check=.c)
	-rm -rf ./gcov_report
	-rm -rf $(LATEXDIR)
	#Remove tmp files after install GUI
	-rm -rf ./build-*

rebuild: remove $(LIB_NAME)

doc:
	$(DOXYGEN) Doxyfile

dvi: doc
	cd $(LATEXDIR) && make && make all

dist: install
	-rm -f $(QT_BUILD_DIR)/*.cpp $(QT_BUILD_DIR)/*.h $(QT_BUILD_DIR)/*.o $(QT_BUILD_DIR)/.qmake.stash $(QT_BUILD_DIR)/Makefile
	tar -czvf $(DIST_ARCHIVE) -C $(QT_BUILD_DIR) .

remove: clean
	-rm -f $(LIB_NAME)
	-rm -rf $(LIB_DIR)
	-rm -rf $(QT_BUILD_DIR)
	-rm -f $(DIST_ARCHIVE)

check_clang:
	clang-format -style=file:../materials/linters/.clang-format -n --verbose $(SOURCES)

fix_clang:
	clang-format -style=file:../materials/linters/.clang-format -i $(SOURCES)
