#include "square.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>


void *proc(void *args){
	int prod_id = *(int *)args;
    int i;
    for(i = 0; i < 10; i++){
        printf("%d\tDormindo %d\n",prod_id, i);
        sleep(1);
    }
    return 0;
}

bool_t squareproc_1_svc(square_in *in, square_out *out, struct svc_req *rqstp){
//	static square_out out;
    pthread_t prod;
    int arg;
    printf("Req with %ld\n", in->arg);
	prod = in->arg;
    pthread_create(&prod, NULL, proc, &arg);

	out->res = in->arg * in->arg;
	sleep(7);
	return TRUE;
}

bool_t reqproc_1_svc (req_in *reqin, req_out *out, struct svc_req *rqstp){

	out->flag = 'A';
	out->a_int = 11;
	out->other_int = 22;

	return TRUE;
}
