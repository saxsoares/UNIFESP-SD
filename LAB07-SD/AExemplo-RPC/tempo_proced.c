#include <stdio.h>
#include <sys/time.h>
#include "tempo.h"

#include "tempo.h"

bool_t
tempo_1_svc(void *argp, timeval_ *result, struct svc_req *rqstp)
{
	bool_t retval;
	/*
	 * insert server code here
	 */

	return retval;
}

int
tempoprog_1_freeresult (SVCXPRT *transp, xdrproc_t xdr_result, caddr_t result)
{
	xdr_free (xdr_result, result);

	/*
	 * Insert additional freeing code here, if needed
	 */

	return 1;
}


//   #pragma GCC diagnostic ignored "-Wunused-variable"
