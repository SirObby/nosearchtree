all: lichess

lichess: algorithm.o lichess.o
	$(CC) $(shell pkg-config --libs libcurl jansson) -g algorithm.o lichess.o -o lichess

%.o:%.c 
	$(CC) $(shell pkg-config --cflags libcurl jansson) -c -g $< -o $@