# the compiler:
CC = gcc

# compiler flags
# Dev
# CFLAGS = -std=gnu11 -Wall -o
# Release
CFLAGS = -std=gnu11 -o

# the build target executable:
# Dev
# TARGET = ./Aviate
# Release
TARGET = /usr/bin/MyProg
TARGET = /usr/bin/myprog
TARGET = /usr/bin/Aviate
TARGET = /usr/bin/aviate

# the libraries linked to the program:
LINKS = -lncurses

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) $(TARGET) main.c $(LINKS)

clean:
	$(RM) $(TARGET)
