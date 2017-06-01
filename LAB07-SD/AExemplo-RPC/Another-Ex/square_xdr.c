/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "square.h"

bool_t
xdr_square_in (XDR *xdrs, square_in *objp)
{
    #pragma GCC diagnostic ignored "-Wunused-variable"
	register int32_t *buf;

	 if (!xdr_long (xdrs, &objp->arg))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_square_out (XDR *xdrs, square_out *objp)
{
    #pragma GCC diagnostic ignored "-Wunused-variable"
	register int32_t *buf;

	 if (!xdr_long (xdrs, &objp->res))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_req_out (XDR *xdrs, req_out *objp)
{
    #pragma GCC diagnostic ignored "-Wunused-variable"
	register int32_t *buf;

	 if (!xdr_char (xdrs, &objp->flag))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->a_int))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->other_int))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_req_in (XDR *xdrs, req_in *objp)
{
	register int32_t *buf;

	 if (!xdr_char (xdrs, &objp->flag))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->a_int))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->other_int))
		 return FALSE;
	return TRUE;
}
