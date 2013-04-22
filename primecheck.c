#include "primecheck.h"

bool preliminary_check(char* n) {
	mpz_t num;
	mpz_t rem;
	mpz_t temp;

	mpz_init(num);
	mpz_init(rem);
	mpz_init(temp);

	mpz_set_str(num,n,10);

	mpz_mod_ui(rem,num,2);
	if (!mpz_sgn(rem)) {
		if (verbosity)
			printf("This number is even\n");
		mpz_clear(num);
		mpz_clear(rem);
		mpz_clear(temp);
		return FALSE; 
	}
	bool ismin1 = TRUE;
	mpz_add_ui(temp,num,1);
	mpz_mod_ui(temp,temp,6);
	if (mpz_sgn(temp)) {
		mpz_sub_ui(temp,num,1);
		mpz_mod_ui(temp,temp,6);
		if (mpz_sgn(temp)) {
			if (verbosity)
				printf("Not of form 6k+-1\n");
			mpz_clear(num);
			mpz_clear(rem);
			mpz_clear(temp);
			return FALSE;
		}
		if (verbosity)
			printf("Is of form 6k+1\n");
		ismin1 = FALSE;
	}
	if (verbosity && ismin1)
		printf("Is of form 6k-1\n");
	return TRUE;
}

void* prime_check(void* b) {
	//printf("core %u reporting in\n",a->threadno);
    args *a = (args*)b;

	mpz_t d;
	mpz_t rem;

	mpz_init(d);
	mpz_init(rem);

	unsigned start = (a->threadno*2) + 1;
	unsigned adder = (a->totalthreads)*2;

	if (verbosity == 2)
		printf("Thread %u is starting at %u and going up by %u\n",a->threadno,start,adder);

	mpz_set_ui(d,start);
	while (mpz_cmp(*(a->root),d) > 0) {
		mpz_mod(rem,*(a->prime),d);
		if (verbosity == 2)
			gmp_printf("Thread %u: %Zd -> %Zd\n",a->threadno,d,rem);
		if (!mpz_sgn(rem)) {
			if (!(a->threadno-1))
				gmp_printf("Divisible by %Zd\n",d);
			*(a->res) = FALSE;
			mpz_clear(rem);
			mpz_clear(d);
			return;
		}
		mpz_add_ui(d,d,adder);
	}	
	
	*(a->res) = TRUE;
	mpz_clear(rem);
	mpz_clear(d);
	return;
}

