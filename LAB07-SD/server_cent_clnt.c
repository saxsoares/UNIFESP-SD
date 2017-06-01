/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include <memory.h> /* for memset */
#include "server_cent.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

request *
req_1(request *argp, CLIENT *clnt)
{
	static request clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, REQ,
		(xdrproc_t) xdr_request, (caddr_t) argp,
		(xdrproc_t) xdr_request, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

reply *
rep_1(request *argp, CLIENT *clnt)
{
	static reply clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, REP,
		(xdrproc_t) xdr_request, (caddr_t) argp,
		(xdrproc_t) xdr_reply, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

request *
rel_1(release *argp, CLIENT *clnt)
{
	static request clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, REL,
		(xdrproc_t) xdr_release, (caddr_t) argp,
		(xdrproc_t) xdr_request, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}
