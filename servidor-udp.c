<pre><p>#include &lt;stdio.h&gt;
#include &lt;stdlib.h&gt;
#include &lt;sys/socket.h&gt;
#include &lt;netinet/in.h&gt;
#include &lt;arpa/inet.h&gt;

int main(int argc, char **argv) {
	if (argc != 2){
		printf(&quot;uso: %s &lt;porta&gt;\n&quot;, argv[0]);
		return 0;
	}

	int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock &lt; 0) {
		perror (&quot;socket()&quot;);
		return -1;
	}
	struct sockaddr_in srv_addr;
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(atoi(argv[1]));
	srv_addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(sock, (struct sockaddr *)&amp;srv_addr, sizeof(srv_addr)) &lt; 0) {
		perror(&quot;bind()&quot;);
		return -1;
	}
	#define MSG_LEN (64 * 1024)
	char req[MSG_LEN];
	socklen_t addrlen;
	struct sockaddr_in caddr;
	int nr, ns;
	while (1) {
		addrlen = sizeof(caddr);
		nr = recvfrom(sock, req, MSG_LEN, 0, (struct sockaddr *)&amp;caddr,
			&amp;addrlen);
		if (nr &lt; 0) {
			perror(&quot;recvfrom()&quot;);
			return -1;
		}

		ns = sendto(sock, req, nr, 0, (struct sockaddr *)&amp;caddr,
				addrlen);
		if (ns &lt; 0) {
			perror(&quot;sendto()&quot;);
			return -1;
		}
	}
	return 0;
}
</p></pre>