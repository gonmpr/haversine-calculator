CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
TARGET = main
LDFLAGS = -lm

SRC = \
	main.c \
	reference_haversine.c \
	pairs_parser.c \
	parser/jparse.c \
	parser/parser.c \
	parser/load_file.c \
	timer/timer.c \
	$(LDFLAGS)	

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

