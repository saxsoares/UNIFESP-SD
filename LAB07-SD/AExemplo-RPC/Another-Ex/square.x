struct square_in{
	long arg;
};

struct square_out{
	long res;
};

struct req_out{
	char flag;
	int a_int;
	int other_int;
};
struct req_in{
	char flag;
	int a_int;
	int other_int;
};
program SQUARE_PROG{
	version SQUARE_VERSION{
		square_out SQUAREPROC(square_in) = 1;
		req_out REQPROC(req_in) = 2;	
	} = 1;
} = 0x12340000;
