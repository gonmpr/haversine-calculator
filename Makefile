CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
TARGET = main
LDFLAGS = -lm

SRC = \
	main.c \
	reference_haversine.c \
	parser/pairs_parser.c \
	parser/jparse.c \
	parser/parser.c \
	parser/load_file.c \
	profiler/profiler.c \
	$(LDFLAGS)	

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

