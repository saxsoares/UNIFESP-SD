/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _SERVER_CENT_H_RPCGEN
#define _SERVER_CENT_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


struct request {
	long pid_c;
	long tempo_c;
};
typedef struct request request;

struct release {
	long pid_c;
	long tempo_c;
	long recurso_n;
};
typedef struct release release;

struct reply {
	long pid_c;
	long tempo_s;
	long recurso;
};
typedef struct reply reply;

#define SERVER_CENT 0x1101
#define SC_V 1

#if defined(__STDC__) || defined(__cplusplus)
#define REQ 1
extern  request * req_1(request *, CLIENT *);
extern  request * req_1_svc(request *, struct svc_req *);
#define REP 2
extern  reply * rep_1(request *, CLIENT *);
extern  reply * rep_1_svc(request *, struct svc_req *);
#define REL 3
extern  request * rel_1(release *, CLIENT *);
extern  request * rel_1_svc(release *, struct svc_req *);
extern int server_cent_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define REQ 1
extern  request * req_1();
extern  request * req_1_svc();
#define REP 2
extern  reply * rep_1();
extern  reply * rep_1_svc();
#define REL 3
extern  request * rel_1();
extern  request * rel_1_svc();
extern int server_cent_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_request (XDR *, request*);
extern  bool_t xdr_release (XDR *, release*);
extern  bool_t xdr_reply (XDR *, reply*);

#else /* K&R C */
extern bool_t xdr_request ();
extern bool_t xdr_release ();
extern bool_t xdr_reply ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_SERVER_CENT_H_RPCGEN */
