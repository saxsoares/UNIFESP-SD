/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "tempo.h"

bool_t
xdr_timeval_ (XDR *xdrs, timeval_ *objp)
{
	#pragma GCC diagnostic ignored "-Wunused-variable"
	register int32_t *buf;

	 if (!xdr_long (xdrs, &objp->tv_sec))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->tv_usec))
		 return FALSE;
	return TRUE;
}