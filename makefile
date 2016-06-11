# the compiler: gcc for C program.
CC = gcc

# compiler flags:
# -o: 
CFLAGS  = -o

# the build target executable:
TARGET = /bin/MyProg

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) $(TARGET) main.c

clean:
	$(RM) $(TARGET)
