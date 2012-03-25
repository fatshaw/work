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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "fcap.h"
#include "util.h"
#include "mmap.h"
/* variables */

static char *fcapdump_readfile;
static bool fcapdump_tab = false;
static bool fcapdump_filterip = false;
static ipaddr_t print_ip;


struct flow_stats {
	uint64_t total_records;
	uint64_t total_pkts;
	uint64_t total_bytes;
};

/* forward declarations */

static void print_record(flow_entry_t *fe, void *);
static void parse_args(int argc, char **argv);
static void usage(char *progname, const char *msg);
static void tcpflags(char *dst, uint16_t flags);

/* functions */

int main(int argc, char **argv){
	struct flow_stats fs = {
		.total_records = 0,
		.total_pkts = 0,
		.total_bytes = 0
	};
	parse_args(argc, argv);
	fcap_reader_t *fr = fcap_open(fcapdump_readfile);
	if(fr == NULL) exit(EXIT_FAILURE);
	if(fcap_read_iter(fr, print_record, &fs) != 0)
		exit(EXIT_FAILURE);
	/*fprintf(stderr, "read %" PRIu64 " flow records consisting of %" PRIu64 " packets, %" PRIu64 " bytes\n",
		fs.total_records, fs.total_pkts, fs.total_bytes);*/
	fcap_close_reader(fr);
	exit(EXIT_SUCCESS);
}

static void print_record(flow_entry_t *fe, void *user){
	struct flow_stats *fs = (struct flow_stats *) user;

	if(fcapdump_filterip && fe->ip[0] != print_ip && fe->ip[1] != print_ip)
		return;

	fs->total_records += 1;
	fs->total_pkts += fe->pkts[0] + fe->pkts[1];
	fs->total_bytes += fe->bytes[0] + fe->bytes[1];

	char sip0[INET_ADDRSTRLEN];
	char sip1[INET_ADDRSTRLEN];
	fmt_ip4(sip0, fe->ip[0]);
	fmt_ip4(sip1, fe->ip[1]);

	char tcpflags0[TCPFLAGS_STRLEN];
	char tcpflags1[TCPFLAGS_STRLEN];
	tcpflags(tcpflags0, fe->tcpflags[0]);
	tcpflags(tcpflags1, fe->tcpflags[1]);

	time_t tm = (time_t) fe->time[0];
	char *stm = asctime(gmtime(&tm));
	stm[strlen(stm) - 1] = '\0';
	int duration = (int) (fe->time[1] - fe->time[0]);
	if(fcapdump_tab){
		printf("%s\t%d\t%s\t%s\t%hu\t%hu\t%s\t"
			"%d\t%d\t%" PRIi64 "\t%" PRIi64 "\n"
		,
			stm, duration,
			sip0, sip1, fe->port[0], fe->port[1],
			(fe->proto == IPPROTO_TCP) ? "TCP" : (fe->proto == IPPROTO_UDP) ? "UDP" : "unknown",
			fe->pkts[0], fe->pkts[1], fe->bytes[0], fe->bytes[1]
		);
	}else{
		printf("[%s for %d millseconds]\t"
			"%s:%hu -> %s:%hu "
			"proto %s "
			"[%d:%d] [%" PRIi64 ":%" PRIi64 "]"
			"%s%s%s%s%s"
			"%s%s\n"
		,
			stm, duration,
			sip0, fe->port[0], sip1, fe->port[1],
			(fe->proto == IPPROTO_TCP) ? "TCP" : (fe->proto == IPPROTO_UDP) ? "UDP" : "unknown",
			fe->pkts[0], fe->pkts[1], fe->bytes[0], fe->bytes[1],
			(fe->proto == IPPROTO_TCP) ? " [" : "",
			(fe->proto == IPPROTO_TCP) ? tcpflags0 : "",
			(fe->proto == IPPROTO_TCP) ? ":" : "",
			(fe->proto == IPPROTO_TCP) ? tcpflags1 : "",
			(fe->proto == IPPROTO_TCP) ? "]" : "",
			fe->assured ? " [ASSURED]" : "",
			fe->finished ? "" : " [UNFINISHED]"
		);
	}
	
#define TCP 0
#define UDP 1
#define UNKNOWN 2

		if(fe->port[0]<1024 && fe->port[0] != 80)return ;
		if(fe->port[1]<1024 && fe->port[1] != 80)return ;
		char buf[1024];
		char flow_addr[64];
		sprintf(flow_addr,"%s:%d->%s:%d(%s)",sip0,fe->port[0],sip1,fe->port[1],fe->proto == IPPROTO_TCP?"TCP"
						:(fe->proto == IPPROTO_UDP) ?"UDP":"UNKNOWN");

		sprintf(buf,"[%s],%d,%d,%d,%d,%lld,%lld,%d,%lld,%lld,%lld,%d,%lld,%s\n",
			flow_addr,
		duration,
		(fe->proto == IPPROTO_TCP) ? TCP : (fe->proto == IPPROTO_UDP) ? UDP : UNKNOWN,
		fe->pkts[0], 
		fe->pkts[1], 
		fe->bytes[0], 
		fe->bytes[1],
		fe->pkts[1]==0?(fe->pkts[0]):(fe->pkts[0])/fe->pkts[1],
		fe->bytes[1]==0?(fe->bytes[0]):(fe->bytes[0])/fe->bytes[1],
		fe->pkts[0]==0?(fe->bytes[0]):(fe->bytes[0])/fe->pkts[0],
		fe->pkts[1] ==0?(fe->bytes[1]):(fe->bytes[1])/fe->pkts[1],
		(fe->pkts[0]+fe->pkts[1])/2,
		(fe->bytes[0]+fe->bytes[1])/2,
		category);
		write(2,buf,strlen(buf));
}

static void tcpflags(char *dst, uint16_t flags){
	memset(dst, 0, TCPFLAGS_STRLEN);
	if(flags & TCP_FIN) strcat(dst, "F");
	if(flags & TCP_SYN) strcat(dst, "S");
	if(flags & TCP_RST) strcat(dst, "R");
	if(flags & TCP_PSH) strcat(dst, "P");
	if(flags & TCP_ACK) strcat(dst, "A");
	if(flags & TCP_URG) strcat(dst, "U");
}

static void usage(char *progname, const char *msg){
	fprintf(stderr, "Error: %s\n\n", msg);
	fprintf(stderr, "Usage: %s <options>\n", progname);
	fprintf(stderr, "program options:\n"
		"\t[ -r <dumpfile> fcap dump file ]\n"
		"\t[ -i <ipaddr> filter a specific IP addres ]\n"
		"\t[ -t use tab delimited output ]\n"
	);
	exit(EXIT_FAILURE);
}
	
static void parse_args(int argc, char **argv){
	int c;
	while((c = getopt(argc, argv, "r:i:t")) != EOF){
		switch(c) {
		case 'r':
			fcapdump_readfile = strdup(optarg);
			char * t = strstr(fcapdump_readfile,".");
			if(t!=NULL){
				category = (char*)calloc(1,t- fcapdump_readfile+1);
				snprintf(category,t - fcapdump_readfile+1,"%s",fcapdump_readfile);
			}	
			break;
		case 'i':
			fcapdump_filterip = true;
			scan_ip4(optarg, &print_ip);
			break;
		case 't':
			fcapdump_tab = true;
			break;
		default:
			usage(argv[0], "unrecognized option");
		}
	}
	if(!fcapdump_readfile)
		usage(argv[0], "need to specify an fcap dump file to read");
}
