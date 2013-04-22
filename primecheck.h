#ifndef _primecheck_
#define _primecheck_

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <stdio.h>

typedef enum { FALSE, TRUE } bool;

typedef struct {
	mpz_t* prime;
	mpz_t* root;
	unsigned totalthreads;
	unsigned threadno;
	bool* res;
} args;

extern int verbosity;

void* prime_check(void*);
bool preliminary_check(char* n);

#endif
