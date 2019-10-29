htable: main.o readline.o huffman.o codes.o
	gcc -o htable main.o readline.o huffman.o codes.o

main.o: main.c readline.o huffman.o codes.o
	gcc -c  main.c

readline.o: readline.c
	gcc -Wall -c readline.c

huffman.o: huffman.c
	gcc -Wall -c huffman.c

codes.o: codes.c
	gcc -Wall -c codes.c

clean:
	rm *.o
