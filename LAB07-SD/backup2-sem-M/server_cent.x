struct request_in{
	long pid_c;
	long tempo_c;
};

struct release{
	long pid_c;
	long tempo_c;
	long recurso_n;
};

struct reply_out{
	long pid_c;
	long tempo_s;
	long recurso;
};

program SERVER_CENT{
	version SC_V{
        reply_out REQ(request_in)=1;
		int REL(release)=2;
	}=1;
}=0x1101;
