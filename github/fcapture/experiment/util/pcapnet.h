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

#ifndef PCAPNET_H
#define PCAPNET_H

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <pcap.h>

/* macros */

#define HENDEL_PCAPERR(pcaphandle, pcapexprn) do{ \
	int rc = pcapexprn; \
	if(rc == -1){ \
		DEBUG("pcap error [" #pcapexprn "]: %s", pcap_geterr(pcaphandle)); \
		exit(EXIT_FAILURE); \
	} \
}while(0)

#define HENDEL_PCAPOPEN(hendel, dev, snaplen, promisc, timeout, errbuf) do{ \
	hendel = pcap_open_live(dev, snaplen, promisc, timeout, errbuf); \
	if(hendel == NULL){ \
		DEBUG("pcap_open_live error: %s", errbuf); \
		exit(EXIT_FAILURE); \
	} \
}while(0)

#define HENDEL_PCAPFOPEN(hendel, fp, errbuf) do{ \
	hendel = pcap_fopen_offline(fp, errbuf); \
	if(hendel == NULL){ \
		DEBUG("pcap_fopen_offline error: %s", errbuf); \
		exit(EXIT_FAILURE); \
	} \
}while(0)

/* declarations */

typedef struct pcap_args {
	pcap_t *handle;
	pcap_t *handle_out;
	pcap_dumper_t *dumper;
	char *dev;
	char *dev_out;
	char *fname;
	char *fname_out;
	char *bpf_string;
	int snaplen;
	int datalink;
	int datalink_out;
	bool no_promisc;

	bool can_readif;
	bool can_inject;
	bool can_readfile;
	bool can_writefile;
} pcap_args_t;

inline const u_char *pcapnet_start_network_header(int datalink, const u_char *pkt, int *etype, size_t *len);
void pcapnet_check_datalink_type(int dlt);
void pcapnet_init(pcap_args_t *);
void pcapnet_init_device(pcap_t **, char *dev, char *bpf_string, int snaplen, bool promisc);
void pcapnet_init_file(pcap_t **, char *fname, char *bpf_string);
void pcapnet_close(pcap_args_t *);
void pcapnet_packet_loop(pcap_args_t *, pcap_handler cb);
void pcapnet_break_loop(pcap_args_t *);
void pcapnet_parse_args(int argc, char **argv, const char *options, pcap_args_t *pa, void (*usage)(const char *msg), bool error);
void pcapnet_usage(pcap_args_t *);
char *pcapnet_optstring(pcap_args_t *, const char *prog_options);

#endif
