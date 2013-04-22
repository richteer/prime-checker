#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "primecheck.h"

void help();
void handle_flags(int,char**,char**);

int cores = 1;
int verbosity = 0;

int main(int argc, char** argv) {
	char* n;	

	if (argc == 1)
		help();

	if (argc >= 2)
		handle_flags(argc,argv,&n);

	if (!preliminary_check(n)) {
		printf("Egads! This is no prime!\n");
		return 0;
	}
	/*------------------------*/
	mpz_t prime;
	mpz_t root;

	mpz_init(prime);
	mpz_init(root);

	mpz_set_str(prime,n,10);
	mpz_sqrt(root,prime);

	args* threadargs = (args*) malloc(sizeof(args)*cores);
	bool* results = (bool*) malloc(sizeof(bool)*cores);
	int i;
	for (i = 0; i < cores; i++) {
		threadargs[i].prime = &prime;
		threadargs[i].root = &root;
		threadargs[i].totalthreads = cores;
		threadargs[i].threadno = i + 1;
		threadargs[i].res = &results[i];
	}

	pthread_t* threads = (pthread_t*) malloc(sizeof(pthread_t)*cores); 


	for (i = 0; i < cores; i++) {
		pthread_create(&threads[i],NULL,prime_check,&threadargs[i]);
	}

	for (i = 0; i < cores; i++) {
		pthread_join(threads[i],NULL);
	}

	bool final = TRUE;
	for (i = 0; i < cores; i++) {
		if (verbosity)
			printf("Thread %i is %i\n",i,results[i]);
		final &= results[i];
	}

	if (final)
		printf("It's a prime\n");
	else
		printf("Egads! This is no prime!\n");

	return 0;

	/*	
	if (prime_check(n,cores)) {
		printf("It's a prime!\n");
	} else {
		printf("Egads! This is no prime!\n");
	}
	*/

}

void handle_flags(int argc, char** argv, char** n) {
	int i;	
	for (i = 1; i < argc; i++) {
		if (!strcmp(argv[i],"-c"))
			cores = atoi(argv[++i]);

		else if (!strcmp(argv[i],"-h"))			
			help();

		else if (!strcmp(argv[i],"-v"))
			verbosity = 1;
	
		else if (!strcmp(argv[i],"-vv"))
			verbosity = 2;

		else {		
			*n = argv[i];
		}
	}
}



void help() {
	printf("Usage: primecheck [OPTION] <integer>\n\n");
	printf("\t-c\t\tSpecify number of threads to use\n");
	printf("\t-h\t\tPrint this help and exit\n");
	exit(0);
}

