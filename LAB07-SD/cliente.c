#include <stdio.h>
#include <rpc/rpc.h>
#include <unistd.h>
#include <time.h>
#include "server_cent.h"

int main(int argc, char **argv){
    srand(time(NULL));
    CLIENT *c;

    if (argc != 2){
        printf("uso: %s <ip_servidor>", argv[0]);
        return 0;
    }

    char *servidor;
    servidor = argv[1];
    long token;
    long pid = (long)getpid();
    long tempo = 0;
    int *result = malloc(sizeof(int));

    c = clnt_create(servidor, SERVER_CENT, SC_V, "tcp");
    if (!c){
        clnt_perror(c, servidor);
        return 0;
    }

    printf("%ld Iniciado, TEMPO = %ld\n", pid, tempo);
    request  req_in;
    request  *rep_request = malloc(sizeof(request));
    request  *rep_release = malloc(sizeof(request));
    reply    *rep_reply   = malloc(sizeof(reply));
    release  new_token;

    while(1){

        req_in.pid_c = pid;
        req_in.tempo_c = ++TEMPO;

        printf("Request: (pid, tempo) = (%ld, %ld)\nAguardando...\n", req_in.pid_c, req_in.tempo_c);
        if((rep_request = req_1(&req_in,  c))==NULL){
            perror(clnt_sperror(c, argv[1]));
            return -1;
        }
        tempo = tempo > rep_request->tempo_s ? (tempo +1 ) : (rep_request->tempo_s + 1);

        if((rep_reply = rep_1(&req_in,  c))==NULL){
            perror(clnt_sperror(c, argv[1]));
            return -1;
        }
        tempo = tempo > rep_reply->tempo_s ? (tempo +1 ) : (rep_reply->tempo_s + 1);
        printf("Token concedido.\n");
        printf("Token->valor = %ld\n", rep_reply->recurso);
        printf("Token->tempo_s = %ld\n", rep_reply->tempo_s);

        printf("Processando...\n");
        token = rep_reply->recurso;
        token = token + 3;
        sleep(7);

        new_token->pid_c = pid;
        new_token->recurso_n = token;
        new_token->tempo_c = ++tempo;
        printf("New Token = %ld\n", new_token->recurso_n);
        if((rep_release = rel_1(&new_token,  c))==NULL){
            perror(clnt_sperror(c, argv[1]));
            return -1;
        }
        printf("Released: tempo = %ld\n", rep_release->tempo_c)

    }
    return 0;
}
