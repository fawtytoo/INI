TARGET = ini

CC = gcc

CFLAGS = -g -Wall

all:
	$(CC) $(CFLAGS) main.c ini.c -o $(TARGET)

clean:
	rm -f $(TARGET)
