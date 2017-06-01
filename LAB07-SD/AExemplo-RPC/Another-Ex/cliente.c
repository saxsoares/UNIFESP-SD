#include "square.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>

int main(int argc, char **argv){
	CLIENT *c;
	square_in in;
	square_out *outp = malloc(sizeof(square_out));

	#pragma GCC diagnostic ignored "-Wunused-variable"
	req_in req_in;
	#pragma GCC diagnostic ignored "-Wunused-variable"
	req_out *req_out = malloc(sizeof(req_out));

	if(argc !=3){
		printf("uso: cliente <host> <valor>\n");
		return 1;
	}

	c = clnt_create(argv[1], SQUARE_PROG, SQUARE_VERSION, "tcp");

	in.arg = atoi(argv[2]);

	if(squareproc_1(&in,outp, c) == FALSE){
		perror(clnt_sperror(c, argv[1]));
		exit(1);
	}
	printf("Resultado: %ld\n", outp->res);

	if(reqproc_1(&req_in, req_out,c) == FALSE){
		perror(clnt_sperror(c, argv[1]));
		exit(1);
	}

	printf("req_out->flag = %c\n", req_out->flag);
	printf("req_out->a_int = %d\n", req_out->a_int);
	printf("req_out->other_int = %d\n", req_out->other_int);
	return 0;

}
