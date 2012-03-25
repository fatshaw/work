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

#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include <pcap.h>

#include "flow.h"
#include "pcapnet.h"
#include "util.h"
#include "version.h"

/* forward declarations */

static void fcapture_process_packet(u_char *, const struct pcap_pkthdr *, const u_char *);
static void fcapture_shutdown(void);
static void fcapture_parse_args(int argc, char **argv);
static void set_stop_running(int x);
static void setup_signals(void);
static void usage(const char *msg);

/* variables */

static pcap_args_t pa = {
	.can_readif = true,
	.can_readfile = true,
};
static flow_table_t *flow_table = NULL;
static uint64_t count_packet = 0;
static uint64_t count_bytes = 0;
static uint64_t count_flow_assured = 0;
static struct timeval tv_start;
static struct timeval tv_end;
static time_t pcap_time;
static bool stop_running = false;
static bool fcapture_daemon = false;
static char *fcapture_logprefix;
static char *fcapture_pidfile;
static char *program_name;

/* functions */

int main(int argc, char **argv){
	fcapture_parse_args(argc, argv);
	if(fcapture_daemon){
		daemon(0, 0);
		make_pidfile(fcapture_pidfile);
	}
	setlinebuf(stderr);
	DEBUG("fcapture %s built on %s",
		version_release, version_buildtime);
	setup_signals();
	flow_table = flow_new(fcapture_logprefix);
	pcapnet_init(&pa);
	gettimeofday(&tv_start, NULL);
	pcapnet_packet_loop(&pa, fcapture_process_packet);
	stop_running = true;
	fcapture_shutdown();
	return EXIT_SUCCESS;
}

static void fcapture_process_packet(u_char *user, const struct pcap_pkthdr *hdr, const u_char *pkt){
	pcap_args_t *pa = (pcap_args_t *) user;
	static flow_headerset_t hs = {
		.h = NULL,
		.ip = NULL,
		.t.tcp = NULL
	};
	hs.h = hdr;

	++count_packet;
	count_bytes += hdr->len;
	pcap_time = hdr->ts.tv_sec;

	int etype;
	size_t len = hdr->caplen;
	hs.ip = (struct iphdr *) pcapnet_start_network_header(pa->datalink, pkt, &etype, &len);
	if(hs.ip == NULL)
		return;

	if(etype == ETHERTYPE_IP){
		if(hdr->caplen >= ETH_HLEN + IP_MIN_HLEN){
			hs.t.tcp = (struct tcphdr *) hs.ip;
			int len_headers = ((char *) hs.ip - (char *) pkt) + hs.ip->ihl * 4 + UDP_HLEN;
			if(hdr->caplen >= len_headers){
				hs.t.tcp = (struct tcphdr *)((char *)(hs.t.tcp) + hs.ip->ihl * 4);
				if(hs.ip->protocol == IPPROTO_TCP || hs.ip->protocol == IPPROTO_UDP){
					int rc = flow_update(flow_table, &hs);
					if(rc == RC_FLOW_ASSURED) ++count_flow_assured;
				}
			}
		}
	}
	if(unlikely(stop_running)){
		fcapture_shutdown();
	}
}

static void fcapture_shutdown(void){
	pcapnet_close(&pa);
	gettimeofday(&tv_end, NULL);
	double start = tv_start.tv_sec + tv_start.tv_usec / 1E6;
	double end = tv_end.tv_sec + tv_end.tv_usec / 1E6;
	DEBUG("shutting down");
	DEBUG("found %" PRIu64 " assured flows", count_flow_assured);
	DEBUG("processed %" PRIu64 " packets (%.2f Mpps), %" PRIu64 " bytes (%.2f MBps) in %.3f seconds",
		count_packet, count_packet / (end - start) / 1E6,
		count_bytes, count_bytes / (end - start) / 1E6,
		end - start
	);
	flow_fcap_dump(flow_table, pcap_time, stop_running);
	flow_destroy(&flow_table);
	FREE(program_name);
	FREE(fcapture_logprefix);
	FREE(fcapture_pidfile);
	exit(EXIT_SUCCESS);
}

static void fcapture_parse_args(int argc, char **argv){
	program_name = strdup(argv[0]);
	char *my_options = pcapnet_optstring(&pa, "l:dP:");
	pcapnet_parse_args(argc, argv, my_options, &pa, usage, false);

	int c;
	while((c = getopt(argc, argv, my_options)) != EOF){
		switch(c) {
		case 'l':
			fcapture_logprefix = strdup(optarg);
			break;
		case 'd':
			fcapture_daemon = true;
			break;
		case 'P':
			fcapture_daemon = true;
			fcapture_pidfile = strdup(optarg);
		default:
			if(!strchr(my_options, c))
				usage("unrecognized option");
		}
	}
	if(!(pa.fname || pa.dev))
		usage("need to specify a packet capture source");
	if(!fcapture_logprefix)
		usage("need to specify a flow log filename prefix");
	if(fcapture_daemon && fcapture_logprefix[0] != '/')
		usage("use fully qualified filename prefix");
	FREE(my_options);
}

static void set_stop_running(int x){
	stop_running = true;
	pcapnet_break_loop(&pa);
}

static void setup_signals(void){
	signal(SIGINT, set_stop_running);
	signal(SIGTERM, set_stop_running);
	siginterrupt(SIGINT, 1);
	siginterrupt(SIGTERM, 1);
}

static void usage(const char *msg){
	fprintf(stderr, "Error: %s\n\n", msg);
	fprintf(stderr, "Usage: %s <options>\n", program_name);
	pcapnet_usage(&pa);
	fprintf(stderr, "program options:\n"
		"\t[ -l <prefix> flow log filename prefix ]\n"
		"\t[ -P <pidfile> ]\n"
		"\t[ -d daemonize ]\n"
	);
	exit(EXIT_FAILURE);
}
