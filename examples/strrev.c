#include <stdio.h>

#include "phantasm.h"

void strrev(char* str) {
		mov	r1	$$(str);
		mov	r2	r1;

	find_end_loop:
		ldb	r3	r2	$(0);
		cmp	r3	$(0);
		je	found_end;
		add	r2	r2	$(1);
		jmp	find_end_loop;

	found_end:
		sub	r2	r2	$(1);

	reverse:
		cmp	r1	r2;
		jge	done;

		ldb	r3	r1	$(0);
		ldb	r4	r2	$(0);
		stb	r3	r2	$(0);
		stb	r4	r1	$(0);

		add	r1	r1	$(1);
		sub	r2	r2	$(1);

		jmp	reverse;

	done:
}

int main() {
	char strings[] = "\0hi\0hello";
	char* ptrs[] = {
		&strings[0],
		&strings[1],
		&strings[4],
	};

	for (size_t i = 0; i < 3; i++) {
		printf("before strrev: '%s'\n", ptrs[i]);
		strrev(ptrs[i]);
		printf("after strrev: '%s'\n", ptrs[i]);
	}
}
