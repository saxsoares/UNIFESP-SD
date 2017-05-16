#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#define MAX_LEN (64 *1024)

int main(int argc, char **argv){
	if (argc != 4) {
		printf("uso: %s <ip_srv> <porta_srv> <msg>\n", argv[0]);
		return 0;
	}
	int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock < 0) {
		perror("socket()");
		return -1;
	}
	int ns, nr;
	struct sockaddr_in saddr, addr;
	socklen_t addr_len;
	char resp[MAX_LEN];
	saddr.sin_port = htons(atoi(argv[2]));
	saddr.sin_family = AF_INET;
	inet_aton(argv[1], &saddr.sin_addr);

	ns = sendto(sock, argv[3], strlen(argv[3]), 0,
			(struct sockaddr *)&saddr,
			sizeof(saddr));

	if (ns < 0) {
		perror("sendto()");
		return -1;
	}
	bzero(resp, MAX_LEN);
	addr_len = sizeof(addr);
	nr = recvfrom(sock, resp, MAX_LEN, 0,
			(struct sockaddr *)&addr,
			&addr_len);

	if (nr < 0) {
		perror("recvfrom()");
		return -1;
	}
	printf("recebeu: %s\n", resp);
	return 0;
}
