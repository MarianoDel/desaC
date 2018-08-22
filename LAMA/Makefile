#
# Makefile by MED on 19-08-2018
#

#---------------------------------------
# Set the project name (executable file name)
TARGET = omcexecd

#---------------------------------------
# Set the compiler
CC = gcc    #for use of the GNU C compiler
# CC = clang    #for the LLVM Project Compiler

#---------------------------------------
# Set the compiler Flags
# CFLAGS = -g     #for DEBUG
# CFLAGS = -O2    #for RELEASE optimize O1 .. O2 .. Os
CFLAGS = -O0    #for RELEASE no optimize

#---------------------------------------
# Set the linker Flags
# LDFLAGS =

#---------------------------------------
# Set the libraries used
# LIBS = -lm    #the math lib (libm.a) in /usr/lib or /usr/lib64

#---------------------------------------
# List here the sources files
SRC = main.c
# SRC += acceso_remoto.c


OBJS = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm -f *.o
	rm -f $(TARGET)

# *** EOF ***
