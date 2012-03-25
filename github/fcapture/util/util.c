/*

Copyright (C) 2007 Robert S. Edmonds 

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#include <errno.h>
#include <getopt.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "util.h"

/* functions */

/* scan_* and fmt_* derived from public domain libdjbdns code */

unsigned scan_ulong(const char *s, unsigned long *u){
	unsigned pos = 0;
	unsigned long result = 0;
	unsigned long c;

	while((c = (unsigned long) (unsigned char) (s[pos] - '0')) < 10){
		result = result * 10 + c;
		++pos;
	}
	*u = result;
	return pos;
}

unsigned fmt_ulong(char *s, unsigned long u){
	unsigned len = 1;
	unsigned long q = u;

	while(q > 9){
		++len; q /= 10;
	}
	if(s){
		s += len;
		do{
			*--s = '0' + (u % 10); u /= 10;
		} while(u);
	}
	return len;
}

unsigned scan_ip4(const char *s, ipaddr_t *n){
	unsigned i;
	unsigned len;
	unsigned long u;
	char *ip = (char *) n;

	len = 0;
	i = scan_ulong(s,&u); if (!i) return 0; ip[0] = u; s += i; len += i;
	if (*s != '.') return 0; ++s; ++len;
	i = scan_ulong(s,&u); if (!i) return 0; ip[1] = u; s += i; len += i;
	if (*s != '.') return 0; ++s; ++len;
	i = scan_ulong(s,&u); if (!i) return 0; ip[2] = u; s += i; len += i;
	if (*s != '.') return 0; ++s; ++len;
	i = scan_ulong(s,&u); if (!i) return 0; ip[3] = u; s += i; len += i;
	return len;
}

unsigned fmt_ip4(char *s, ipaddr_t n){
	unsigned i;
	unsigned len = 0;
	char *ip = (char *) &n;

	i = fmt_ulong(s,(unsigned long) (unsigned char) ip[0]); len += i; if (s) s += i;
	if (s) *s++ = '.'; ++len;
	i = fmt_ulong(s,(unsigned long) (unsigned char) ip[1]); len += i; if (s) s += i;
	if (s) *s++ = '.'; ++len;
	i = fmt_ulong(s,(unsigned long) (unsigned char) ip[2]); len += i; if (s) s += i;
	if (s) *s++ = '.'; ++len;
	i = fmt_ulong(s,(unsigned long) (unsigned char) ip[3]); len += i; if (s) s += i;
	*s = '\0';
	return len;
}

uint32_t bswap32(uint32_t x){
	return  ((x << 24) & 0xff000000 ) |
		((x <<  8) & 0x00ff0000 ) |
		((x >>  8) & 0x0000ff00 ) |
		((x >> 24) & 0x000000ff );
}

uint16_t bswap16(uint16_t x){
	return  (x & 0xff) << 8 | (x & 0xff00) >> 8;
}

void reset_getopt(void){
#ifdef __GLIBC__
	optind = 0;
#else
	optind = 1;
#endif
#ifdef HAVE_OPTRESET
	optreset = 1;
#endif
}

void make_pidfile(char *pidfile){
	if(pidfile == NULL)
		return;
	pid_t pid = getpid();
	FILE *fpidfile = fopen(pidfile, "w");
	if(fpidfile == NULL)
		ERROR("unable to open pidfile for writing: %s", strerror(errno));
	fprintf(fpidfile, "%d\n", pid);
	fclose(fpidfile);
}
