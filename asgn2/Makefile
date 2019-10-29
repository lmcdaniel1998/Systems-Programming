fw: main.o sorts.o hash.o getwords.o readline.o putwords.o
	gcc -o fw main.o readline.o getwords.o hash.o sorts.o putwords.o

main.o: main.c sorts.o hash.o getwords.o readline.o putwords.o
	gcc -c  main.c

putwords.o: putwords.c
	gcc -Wall -c putwords.c

sorts.o: sorts.c
	gcc -Wall -c sorts.c

hash.o: hash.c
	gcc -Wall -c hash.c

getwords.o: getwords.c
	gcc -Wall -c getwords.c

readline.o: readline.c
	gcc -Wall -c readline.c

clean:
	rm *.o
