#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define REQ_LEN (64*1024)
#define RESP_LEN REQ_LEN
#define MAX_CON 1024

struct param{
	pthread_t tid;
	int csock;
	struct sockaddr_in caddr;
};
typedef struct param param;

void *handle_con(void *args) {

	param p = *(param *)args;
	char request[REQ_LEN], response[RESP_LEN];
	int nr, ns;
	bzero(request, REQ_LEN);
	bzero(response, RESP_LEN);
	// recv
	nr = recv(p.csock, request, REQ_LEN, 0);
	if (nr < 0) {
		perror("recv()");
		pthread_exit(NULL);
	}
	printf("Cliente %s:%d enviou: '%s'\n",
			inet_ntoa(p.caddr.sin_addr), ntohs(p.caddr.sin_port),
			request);
	ns = send(p.csock, request, nr, 0);
	if (ns < 0) {
		perror("send()");
		pthread_exit(NULL);
	}
	sleep(10);
	close(p.csock);
	pthread_exit(NULL);
}


int main(int argc, char **argv) {
	if (argc != 2){
		printf("uso: %s <porta>\n", argv[0]);
		return 0;
	}
	// socket
	int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock < 0) {
		perror ("socket()");
		return -1;
	}

	/* int bind(int socket,	const struct sockaddr *address,	socklen_t address_len); */
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(atoi(argv[1]));
	saddr.sin_addr.s_addr = INADDR_ANY;
	if (bind(sock, (struct sockaddr *)&saddr, sizeof(saddr)) < 0){
		perror("bind()");
		return -1;
	}

	int nr;
	char req[REQ_LEN];
	struct sockaddr_in caddr;
	int caddr_len;
	int i;
	param p[MAX_CON];
	i = 0;
	while(1) {
		// accept
		bzero(&caddr, sizeof(caddr));
		caddr_len = sizeof(caddr);
		nr = recvfrom(sock,req, REQ_LEN, 0, (struct sockaddr *)&caddr, (socklen_t *)&caddr_len);
		if (nr < 0){
			perror("recvfrom()");
			return -1;
		}
		p[i].csock = nr;
		p[i].caddr = caddr;
		pthread_create(&p[i].tid, NULL, handle_con, &p[i]);
		i = (i+1) % MAX_CON;
	}

	return 0;
}

