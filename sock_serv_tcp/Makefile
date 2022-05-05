#
# Makefile by MED on 19-08-2018
#

#---------------------------------------
# Set the project name (executable file name)
TARGET = sock_serv

#---------------------------------------
# Set the compiler
CC = gcc    #for use of the GNU C compiler
# CC = clang    #for the LLVM Project Compiler

#---------------------------------------
# Set the compiler Flags
# CFLAGS = -g     #for DEBUG
# CFLAGS = -O2    #for RELEASE optimize O1 .. O2 .. Os
# CFLAGS = -O0    #for RELEASE no optimize

#---------------------------------------
# Set the linker Flags
# LDFLAGS =

#---------------------------------------
# Set the libraries used
# LIBS = -lm    #the math lib (libm.a) in /usr/lib or /usr/lib64
# LIBS = -lncurses    #the curses lib

#---------------------------------------
# List here the sources files
SRC = main.c
# SRC += libcsv.c


OBJS = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o $@
	gtags -q

%.o: %.c
	$(CC) -c $(CFLAGS) $^ -o $@

# test_ncurses:
# 	$(CC) test_ncurses.c $(LIBS)

clean:
	rm -f *.o
	rm -f *.out
	rm -f $(TARGET)
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.gcov

# *** EOF ***
