all : test1 test2

all1 : test1

all2 : test2

test1 : test1.o exercice1.o exercice2.o 
	gcc -o test1 test1.o exercice1.o exercice2.o 

test2 : test2.o exercice1.o exercice2.o exercice3.o exercice4.o exercice5.o exercice6.o exercice7.o exercice8.o exercice9.o
	gcc -o test2 test2.o exercice1.o exercice2.o exercice3.o exercice4.o exercice5.o exercice6.o exercice7.o exercice8.o exercice9.o -lssl -lcrypto

test1.o : test1.c structures.h
	gcc -c test1.c

test2.o : test2.c structures.h
	gcc -c test2.c

exercice1.o : structures.h exercice1.c
	gcc -c exercice1.c

exercice2.o : structures.h exercice2.c
	gcc -c exercice2.c

exercice3.o : structures.h exercice3.c
	gcc -c exercice3.c

exercice4.o : structures.h exercice4.c
	gcc -c exercice4.c

exercice5.o : structures.h exercice5.c
	gcc -c exercice5.c

exercice6.o : structures.h exercice6.c
	gcc -c exercice6.c

exercice7.o : structures.h exercice7.c
	gcc -c exercice7.c -lssl -lcrypto

exercice8.o : structures.h exercice8.c
	gcc -c exercice8.c

exercice9.o : structures.h exercice9.c
	gcc -c exercice9.c

clean : 
	rm -f main *.o
	