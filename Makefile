CC = gcc
CFLAGS = -Wall
INC = -Iinclude

main: target/main.o target/tiles.o 
		$(CC) target/main.o target/tiles.o -o target/ricl

target/main.o: src/main.c 
		$(CC) -c $(INC) $(CFLAGS) src/main.c -o target/main.o

target/tiles.o: src/tiles.c
		$(CC) -c $(INC) $(CFLAGS) src/tiles.c -o target/tiles.o

clean: 
	rm -f $(ODIR)/*.o $(OUT)