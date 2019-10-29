hencode: main.o readfile.o huffman.o codes.o header.o encode.o
	gcc -o hencode main.o readfile.o huffman.o codes.o header.o encode.o

main.o: main.c readfile.o huffman.o codes.o header.o encode.o
	gcc -c  main.c

readline.o: readfile.c
	gcc -Wall -c readfile.c

huffman.o: huffman.c
	gcc -Wall -c huffman.c

codes.o: codes.c
	gcc -Wall -c codes.c

header.o: header.c
	gcc -Wall -c header.c

encode.o: encode.c
	gcc -Wall -c encode.c

clean:
	rm *.o
