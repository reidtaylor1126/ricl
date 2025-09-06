CC = gcc
CFLAGS = -Wall
INC = -Iinclude

main: target/main.o target/tiles.o target/hand.o target/wall.o
		$(CC) target/main.o target/tiles.o target/hand.o target/wall.o -o target/ricl

target/main.o: src/main.c 
		$(CC) -c $(INC) $(CFLAGS) src/main.c -o target/main.o

target/tiles.o: src/tiles.c
		$(CC) -c $(INC) $(CFLAGS) src/tiles.c -o target/tiles.o

target/hand.o: src/hand.c
		$(CC) -c $(INC) $(CFLAGS) src/hand.c -o target/hand.o

target/wall.o: src/wall.c
		$(CC) -c $(INC) $(CFLAGS) src/wall.c -o target/wall.o

clean: 
	rm -f $(ODIR)/*.o $(OUT)