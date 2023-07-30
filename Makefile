# Define C compiler & flags
CC = gcc
CFLAGS = -g -Wall
TARGET = allocate

all: $(TARGET)

$(TARGET): main.c read.h read.c memory.h memory.c scheduling.h scheduling.c
	$(CC) $(CFLAGS) -o $(TARGET) main.c read.c memory.c scheduling.c

clean:
	$(RM) $(TARGET)