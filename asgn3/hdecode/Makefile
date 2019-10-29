hdecode: main.o readheader.o huffman.o codes.o header.o encode.o decode.o
	gcc -o hdecode main.o readheader.o huffman.o codes.o header.o encode.o decode.o

main.o: main.c readheader.o huffman.o codes.o header.o encode.o decode.o
	gcc -c  main.c

readheader.o: readheader.c
	gcc -Wall -c readheader.c

decod.o: decode.c
	gcc -Wall -c decode.c

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
