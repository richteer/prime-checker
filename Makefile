primecheck: primecheck.o main.o
	gcc -lgmp -lpthread -O3 primecheck.o main.o -march=corei7 -o primecheck

main.o: main.c primecheck.o
	gcc -c -lgmp -lpthread -O3 main.c -march=corei7 -o main.o

primecheck.o: primecheck.c primecheck.h
	gcc -c -lgmp -lpthread -O3 primecheck.c -march=corei7 -o primecheck.o

clean:
	rm -f *.o primecheck
