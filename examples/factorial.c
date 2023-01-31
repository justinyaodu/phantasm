#include <stdio.h>

#include "phantasm.h"

unsigned long factorial(int n) {
	mov	r0	$(1);
	mov	r1	$(n);
loop:
	cmp	r1	$(1);
	jle	done;
	mul	r0	r0	r1;
	sub	r1	r1	$(1);
	jmp	loop;
done:
	return *r0;
}

int main() {
	printf("5! = %ld\n", factorial(5));
	printf("15! = %ld\n", factorial(15));
}
