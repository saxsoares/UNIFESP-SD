<pre><p>#include &lt;stdio.h&gt;
#include &lt;stdlib.h&gt;
#include &lt;strings.h&gt;
#include &lt;sys/socket.h&gt;
#include &lt;netinet/in.h&gt;
#include &lt;arpa/inet.h&gt;
#include &lt;unistd.h&gt;
#include &lt;pthread.h&gt;

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
	if (nr &lt; 0) {
		perror(&quot;recv()&quot;);
		pthread_exit(NULL);
	}
	printf(&quot;Cliente %s:%d enviou: '%s'\n&quot;,
			inet_ntoa(p.caddr.sin_addr), ntohs(p.caddr.sin_port),
			request);
	ns = send(p.csock, request, nr, 0);
	if (ns &lt; 0) {
		perror(&quot;send()&quot;);
		pthread_exit(NULL);
	}
	sleep(10);
	close(p.csock);
	pthread_exit(NULL);
}


int main(int argc, char **argv) {

	struct sockaddr_in saddr;

	// socket
	int ls = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);


	/* int bind(int socket,
			const struct sockaddr *address,
			socklen_t address_len); */
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(atoi(argv[1]));
	saddr.sin_addr.s_addr = INADDR_ANY;
	if (bind(ls, (struct sockaddr *)&amp;saddr, sizeof(saddr)) &lt; 0){
		perror(&quot;bind()&quot;);
		return -1;
	}

	if (listen(ls, MAX_CON) &lt; 0){
		perror(&quot;listen()&quot;);
		return -1;
	}
	int csock;
	struct sockaddr_in caddr;
	int caddr_len;
	int i;
	param p[MAX_CON];
	i = 0;
	while(1) {
		// accept
		bzero(&amp;caddr, sizeof(caddr));
		caddr_len = sizeof(caddr);
		csock = accept(ls,
				(struct sockaddr *)&amp;caddr,
				(socklen_t *)&amp;caddr_len);
		if (csock &lt; 0){
			perror(&quot;accept()&quot;);
			return -1;
		}
		p[i].csock = csock;
		p[i].caddr = caddr;
		pthread_create(&amp;p[i].tid, NULL, handle_con, &amp;p[i]);
		i = (i+1) % MAX_CON;
	}

	return 0;
}
</p></pre>