CC = gcc
CFLAGS = -Wall -g
ODIR = target
OUT = target/ricl
INC = -Iinclude

main: target/main.o target/tiles.o target/hand.o target/wall.o target/player.o target/table.o target/scoring.o target/cursor.o
		$(CC) $(CFLAGS) target/main.o target/tiles.o target/hand.o target/wall.o target/player.o target/table.o target/scoring.o target/cursor.o -o target/ricl

target/main.o: src/main.c 
		$(CC) -c $(INC) $(CFLAGS) src/main.c -o target/main.o

target/tiles.o: src/tiles.c
		$(CC) -c $(INC) $(CFLAGS) src/tiles.c -o target/tiles.o

target/hand.o: src/hand.c
		$(CC) -c $(INC) $(CFLAGS) src/hand.c -o target/hand.o

target/wall.o: src/wall.c
		$(CC) -c $(INC) $(CFLAGS) src/wall.c -o target/wall.o

target/player.o: src/player.c
		$(CC) -c $(INC) $(CFLAGS) src/player.c -o target/player.o

target/table.o: src/table.c
		$(CC) -c $(INC) $(CFLAGS) src/table.c -o target/table.o

target/scoring.o: src/scoring.c
		$(CC) -c $(INC) $(CFLAGS) src/scoring.c -o target/scoring.o

target/cursor.o: src/cursor.c
		$(CC) -c $(INC) $(CFLAGS) src/cursor.c -o target/cursor.o

clean: 
	rm -f $(ODIR)/*.o $(OUT)