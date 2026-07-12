CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
TARGET = main

SRC = \
	main.c \
	pairs_parser.c \
	parser/jparse.c \
	parser/parser.c \
	parser/load_file.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

