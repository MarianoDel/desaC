#
# Makefile by MED on 19-08-2018
#

#---------------------------------------
# Set the project name (executable file name)
TARGET = test_port

#---------------------------------------
# Set the compiler
CC = gcc    #for use of the GNU C compiler
# CC = clang    #for the LLVM Project Compiler

#---------------------------------------
# Set the compiler Flags
CFLAGS = -g     #for DEBUG
# CFLAGS = -O2    #for RELEASE optimize O1 .. O2 .. Os
# CFLAGS = -O0    #for RELEASE no optimize

#---------------------------------------
# Set the linker Flags
# LDFLAGS = -L/usr/lib64

#---------------------------------------
# Set the libraries used
# LIBS = -lm    #the math lib (libm.a) in /usr/lib or /usr/lib64
# LIBS = libportaudio.a    #si se encuentra en este mismo directorio
LIBS = -lportaudio
LIBS += -lm
LIBS += -lrt
LIBS += -lasound
# LIBS += -ljack
LIBS += -pthread

#---------------------------------------
# List here the sources files
SRC = main.c
SRC += list_devices.c
SRC += play_sine.c
# SRC += cdr_asterisk.c
# SRC += server_tcp.c

# formato pedido por portaudio
# gcc main.c libportaudio.a -lrt -lm -lasound -ljack -pthread -o YOUR_BINARY

OBJS = $(SRC:.c=.o)

all: $(TARGET)
	gtags -q

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm -f *.o
	rm -f $(TARGET)

# *** EOF ***
