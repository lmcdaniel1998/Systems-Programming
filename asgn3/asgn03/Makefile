all: hencode hdecode

hencode: hencode_main.o readfile.o huffman.o codes.o header.o encode.o
	gcc -o hencode hencode_main.o readfile.o huffman.o codes.o header.o encode.o

hdecode: hdecode_main.o readheader.o huffman.o codes.o decode.o
	gcc -o hdecode hdecode_main.o readheader.o huffman.o codes.o decode.o

hencode_main.o: hencode_main.c readfile.o huffman.o codes.o header.o encode.o
	gcc -c -Wall hencode_main.c

hdecode_main.o: hdecode_main.c readheader.o huffman.o codes.o decode.o
	gcc -c -Wall hdecode_main.c

readline.o: readfile.c
	gcc -Wall -c readfile.c

readheader.o: readheader.c
	gcc -Wall -c readheader.c

huffman.o: huffman.c
	gcc -Wall -c huffman.c

codes.o: codes.c
	gcc -Wall -c codes.c

header.o: header.c
	gcc -Wall -c header.c

encode.o: encode.c
	gcc -Wall -c encode.c

decode.o: decode.c
	gcc -Wall -c decode.c

clean:
	rm *.o
