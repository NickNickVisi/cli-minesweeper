CC = gcc
CFLAGS = -Wall -Wextra
TARGET = minesweeper.out

$(TARGET): minesweeper.c
	$(CC) $(CFLAGS) minesweeper.c -o $(TARGET)

clean: $(TARGET)
	rm -f $(TARGET)