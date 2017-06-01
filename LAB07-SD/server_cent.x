struct request{
	long pid_c;
	long tempo_c;
};

struct release{
	long pid_c;
	long tempo_c;
	long recurso_n;
};

struct reply{
	long pid_c;
	long tempo_s;
	long recurso;
};

program SERVER_CENT{
	version SC_V{
        request REQ(request)=1;
        reply REP(request)=2;
		request REL(release)=3;
	}=1;
}=0x1101;
