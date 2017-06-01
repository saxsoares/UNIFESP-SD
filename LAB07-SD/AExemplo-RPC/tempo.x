struct timeval_{
    long tv_sec;
    long tv_usec;
};

program TEMPOPROG{
    version TEMPOVERS    {
        timeval_ TEMPO(void)=1;
    }=1;
}=0x12345;
