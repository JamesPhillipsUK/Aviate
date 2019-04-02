# the compiler: gcc for C program.
CC = gcc

# compiler flags:
# -o: 
CFLAGS  = -lncurses -o

# the build target executable:
TARGET = /usr/bin/MyProg
TARGET = /usr/bin/Aviate
# for development
# TARGET = ./Aviate

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) $(TARGET) main.c

clean:
	$(RM) $(TARGET)
