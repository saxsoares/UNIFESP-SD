/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "server_cent.h"

bool_t
xdr_request_in (XDR *xdrs, request_in *objp)
{
    #pragma GCC diagnostic ignored "-Wunused-variable"
	register int32_t *buf;

	 if (!xdr_long (xdrs, &objp->pid_c))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->tempo_c))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_release (XDR *xdrs, release *objp)
{
    #pragma GCC diagnostic ignored "-Wunused-variable"
	register int32_t *buf;

	 if (!xdr_long (xdrs, &objp->pid_c))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->tempo_c))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->recurso_n))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_reply_out (XDR *xdrs, reply_out *objp)
{
    #pragma GCC diagnostic ignored "-Wunused-variable"
	register int32_t *buf;

	 if (!xdr_long (xdrs, &objp->pid_c))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->tempo_s))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->recurso))
		 return FALSE;
	return TRUE;
}
