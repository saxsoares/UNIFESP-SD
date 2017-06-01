#include <stdio.h>
#include <rpc/rpc.h>
#include <unistd.h>
#include <time.h>
#include "server_cent.h"

int main(int argc, char **argv){
    CLIENT *c;
    char *servidor;
    if (argc != 2){
        printf("uso: %s <ip_servidor>", argv[0]);
        return 0;
    }
    srand(time(NULL));
    servidor = argv[1];
    c = clnt_create(servidor, SERVER_CENT, SC_V, "tcp");
    if (!c){
        clnt_perror(c, servidor);
        return 0;
    }
    int *recurso;
    long pid = (long)getpid();
    long TEMPO = 0;
    printf("Programa iniciado: PID = %ld, TEMPO = %ld\n", pid, TEMPO);
    while(1){
        request_in req_in;
        reply_out  *rep_out = malloc(sizeof(reply_out));
        release new_token;

        req_in.pid_c = pid;
        req_in.tempo_c = ++TEMPO;

        printf("Request: (pid, tempo) = (%ld, %ld)\nAguardando...\n",
                                    req_in.pid_c, req_in.tempo_c);
        if(rep_out = (req_1(&req_in,  c))==FALSE){
            perror(clnt_sperror(c, argv[1]));
            return -1;
        }
        TEMPO = TEMPO > rep_out->tempo_s ? (TEMPO +1 ) : (rep_out->tempo_s + 1);

        printf("Token recebido: (valor, tempo) = (%ld, %ld\n",
                                    rep_out->recurso, rep_out->tempo_s);
        printf("\nProcessando...\n");
        recurso = rep_out->recurso;
        recurso = recurso < 0 ? 2 : (recurso * 2);
        sleep(4);
        new_token.pid_c = pid;
        new_token.tempo_c = ++TEMPO;
        new_token.recurso_n = recurso;

        if(result = (rel_1(&new_token,   c))==FALSE){
            perror(clnt_sperror(c, argv[1]));
            return -1;
        }
        printf("Devolvendo token.\n");
        printf("\tRecurso = %ld\n", new_token.recurso_n);
        printf("\tPid = %ld\n", new_token.pid_c);
        printf("\tTEMPO = %ld\n", new_token.tempo_c);
    }
    return 0;
}
