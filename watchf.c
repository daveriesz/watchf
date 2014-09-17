
/* getconf LFS_CFLAGS */

#if defined(SunOS)
#define _LARGEFILE_SOURCE
#define _FILE_OFFSET_BITS 64
#elif defined(IRIX) || defined(IRIX64)
#elif defined(HPUX)
#elif defined(LINUX) || defined(Linux)
#define _FILE_OFFSET_BITS 64
#elif defined(OSF1)
#elif defined(AIX)
#endif

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <math.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>

int watchf(char *);
void byebye(int);
void cprint(char *,int,int,int,int,int);
void sxprint(char *,int,int,int *);
char *commafy_ll(long long ll);

int main(int argc, char **argv)
{
	char *cp;

	if(argc < 2)
	{
		if(cp = strrchr(argv[0], '/')) cp++;
		else cp = argv[0];
		fprintf(stderr, "usage: %s <file>\n", cp);
		return 1;
	}

	signal(SIGINT, byebye);

	watchf(argv[1]);
	return 0;
}

int watchf(char *filename)
{
	struct stat stbuf, stb1;
	int r=0, q, e=0, e1=0;
	long long aa, bb, cc, dd, ee;
	struct timespec rqtp;
	
	rqtp.tv_sec  = 0;
	rqtp.tv_nsec = 100;

	stb1.st_size = -1;
	stbuf.st_size = 0;

	for(;;)
	{
		q = r;
		r = stat(filename, &stbuf);
		if( (q != r) || (r < 0 && e != errno) )
		{
			printf("\r                                                                                ");
			fflush(stdout);
		}
		if(r < 0)
		{
			e = errno;
			if(e != e1)
			{
				printf("\r%s: (%d) %s", filename, errno, strerror(errno));
				fflush(stdout);
			}
			e1 = e;
		}
		else if(stbuf.st_size != stb1.st_size)
		{
			printf("\r%s: %15s bytes", filename, commafy_ll(stbuf.st_size));
			fflush(stdout);

			stb1.st_size = stbuf.st_size;
		}
		nanosleep(&rqtp, NULL);
	}
}

#define WATCHF_SCHAR 16

void cprint(char *s, int e, int d, int c, int b, int a)
{
	char
		sa[WATCHF_SCHAR],
		sb[WATCHF_SCHAR],
		sc[WATCHF_SCHAR],
		sd[WATCHF_SCHAR],
		se[WATCHF_SCHAR];
	int beg=0;

	sxprint(se, e, 0, &beg);
	sxprint(sd, d, 0, &beg);
	sxprint(sc, c, 0, &beg);
	sxprint(sb, b, 0, &beg);
	sxprint(sa, a, 1, &beg);

/*	printf("\r%s: %.3d,%.3d,%.3d,%.3d,%.3d bytes", s,e,d,c,b,a); */
	printf("\r%s: %s%s%s%s%s bytes", s,se,sd,sc,sb,sa);
	fflush(stdout);
}

void sxprint(char *sx, int x, int last, int *beg)
{
	if(x == 0 && *beg == 0) sprintf(sx, last?"   ":"    ");
	else if(!*beg) { sprintf(sx, last?"%3d":"%3d,", x); *beg = 1; }
	else { sprintf(sx, last?"%.3d":"%.3d,", x); }
}

void byebye(int n)
{
	printf("\n");
	exit(0);
}

char *commafy_ll(long long ll)
{
	static char s[1024];
	char s2[8];
	long long md;
	static long state = 0;
	static char
		*fmt1="%.3lld,",
		*fmt2p="%lld,",
		*fmt2n="-%lld,",
		*fmt3="%.3lld",
		*fmt4p="%lld",
		*fmt4n="-%lld";
	static char *fmt2, *fmt4;
	
	if(state == 0)
	{
		bzero(s, 1024);
		if(ll < 0)
		{
			ll = -ll;
			fmt2 = fmt2n;
			fmt4 = fmt4n;
		}
		else
		{
			fmt2 = fmt2p;
			fmt4 = fmt4p;
		}
	}
	state++;

	md = ll / 1000;
	if(md >= 1000) commafy_ll(md);
	
	if(md%1000 > 0)
	{
		sprintf(s2, (*s)?fmt1:fmt2, md%1000);
		strcat(s, s2);
	}

	state--;
	
	if(state == 0)
	{
		sprintf(s2, (*s)?fmt3:fmt4, ll%1000);
		strcat(s, s2);
	}
	
	return s;
}
