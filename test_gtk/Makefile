#
# Makefile by MED on 19-08-2018
#

#---------------------------------------
# Set the project name (executable file name)
TARGET = gtk1

#---------------------------------------
# Set the compiler
CC = gcc    #for use of the GNU C compiler
# CC = clang    #for the LLVM Project Compiler

#---------------------------------------
# Set the compiler Flags
# CFLAGS = -g     #for DEBUG
# CFLAGS = -O2    #for RELEASE optimize O1 .. O2 .. Os
# CFLAGS = -O0    #for RELEASE no optimize
CFLAGS = `pkg-config --cflags gtk+-3.0`    #the gtk includes flags

#---------------------------------------
# Set the linker Flags
# LDFLAGS =

#---------------------------------------
# Set the libraries used
# LIBS = -lm    #the math lib (libm.a) in /usr/lib or /usr/lib64
# LIBS = -lncurses    #the curses lib
LIBS = `pkg-config --libs gtk+-3.0`    #the gtk lib

#---------------------------------------
# List here the sources files
# SRC = main.c
# SRC = main2.c
# SRC = main3.c
# SRC = main4.c
# SRC = main5.c
# SRC = main_glade.c
# SRC = main_glade_lcd.c
# SRC = main_glade_lcd2.c
SRC = main_glade_lcd3.c
# SRC = main_pixel.c    #para gtk2
# SRC += libcsv.c


OBJS = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm -f *.o
	rm -f $(TARGET)

example-4:
	gcc `pkg-config --cflags gtk+-3.0` -o example-4 example-4.c `pkg-config --libs gtk+-3.0`

# *** EOF ***
