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
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <pcap.h>

#include "pcapnet.h"
#include "util.h"

/* functions */

inline const u_char *pcapnet_start_network_header(
		int datalink,
		const u_char *orig_pkt,
		int *etype, /* modified */
		size_t *len /* modified */
		){
	const u_char *pkt = orig_pkt;
	switch(datalink){
		case DLT_NULL: {
			if(*len < 4)
				return NULL;
			*len -= 4;
			pkt += 4;
			uint32_t x = *(const uint32_t *) pkt;
			if     (x == PF_INET)  *etype = ETHERTYPE_IP;
			else if(x == PF_INET6) *etype = ETHERTYPE_IPV6;
			break;
		}
		case DLT_LOOP: {
			if(*len < 4)
				return NULL;
			*len -= 4;
			pkt += 4;
			uint32_t x = my_ntohl(*(const uint32_t *) pkt);
			if     (x == PF_INET)  *etype = ETHERTYPE_IP;
			else if(x == PF_INET6) *etype = ETHERTYPE_IPV6;
			break;
		}
		case DLT_EN10MB: {
			if(*len < ETH_HLEN)
				return NULL;
			const struct ether_header *ether = (const struct ether_header *) pkt;
			*etype = my_ntohs(ether->ether_type);
			*len -= ETH_HLEN;
			pkt += ETH_HLEN;
			if(*etype == ETHERTYPE_VLAN){
				if(*len < 4)
					return NULL;
				*len -= 4;
				*etype = my_ntohs(*(const uint16_t *)(pkt + 2));
				pkt += 4;
			}
			break;
		}
#ifdef DLT_LINUX_SLL
		case DLT_LINUX_SLL: {
			if(*len < 16)
				return NULL;
			*etype = my_ntohs(*(const uint16_t *)(pkt + 14));
			*len -= 16;
			pkt += 16;
			break;
		}
#endif
	}
	return pkt;
}

void pcapnet_check_datalink_type(int dlt){
	switch(dlt) {
		case DLT_NULL:
		case DLT_LOOP:
		case DLT_EN10MB:
#ifdef DLT_LINUX_SLL
		case DLT_LINUX_SLL:
#endif
			break;
		default:
			ERROR("datalink type %s not supported",
				pcap_datalink_val_to_name(dlt));
	}
}

void pcapnet_init(pcap_args_t *pa){
	if(pa->dev && pa->fname)
		ERROR("cannot read packets from device and file simultaneously");
	if(!(pa->dev || pa->fname))
		ERROR("need a packet capture source");
	if(pa->snaplen < 1U || pa->snaplen > 65536U){
		pa->snaplen = 1518;
	}
	if(pa->dev){
		DEBUG("opening capture interface %s%s%s%s",
			pa->dev,
			pa->bpf_string ? " with filter '" : "",
			pa->bpf_string ? pa->bpf_string : "",
			pa->bpf_string ? "'" : ""
		);
		pcapnet_init_device(&pa->handle, pa->dev, pa->bpf_string, pa->snaplen, pa->no_promisc == false);
	}
	if(pa->dev_out){
		DEBUG("opening injection interface %s", pa->dev_out);
		if(pa->dev && strcmp(pa->dev, pa->dev_out) == 0){
			pa->handle_out = pa->handle;
		}else{
			pcapnet_init_device(&pa->handle_out, pa->dev_out, NULL, 1, true);
		}
	}
	if(pa->fname){
		pcapnet_init_file(&pa->handle, pa->fname, pa->bpf_string);
		DEBUG("reading from pcap file %s link-type %s%s%s%s",
			pa->fname,
			pcap_datalink_val_to_name(pcap_datalink(pa->handle)),
			pa->bpf_string ? " with filter '" : "",
			pa->bpf_string ? pa->bpf_string : "",
			pa->bpf_string ? "'" : ""
		);
	}
	if(pa->fname_out){
		if(!(pa->dumper = pcap_dump_open(pa->handle, pa->fname_out))){
			ERROR("pcap_dump_open: %s", pcap_geterr(pa->handle));
		}
	}
	if(pa->handle){
		pa->datalink = pcap_datalink(pa->handle);
		pcapnet_check_datalink_type(pa->datalink);
	}
	if(pa->handle_out){
		pa->datalink_out = pcap_datalink(pa->handle_out);
	}
}

void pcapnet_init_device(pcap_t **pcap, char *dev, char *bpf_string, int snaplen, bool promisc){
	struct bpf_program pcap_filter;
	char pcap_errbuf[PCAP_ERRBUF_SIZE];

	HENDEL_PCAPOPEN(*pcap, dev, snaplen, promisc, 0, pcap_errbuf);
	if(bpf_string != NULL){
		HENDEL_PCAPERR(*pcap, pcap_compile(*pcap, &pcap_filter, bpf_string, 1, 0));
		HENDEL_PCAPERR(*pcap, pcap_setfilter(*pcap, &pcap_filter));
	}
}

void pcapnet_init_file(pcap_t **pcap, char *fname, char *bpf_string){
	struct bpf_program pcap_filter;
	char pcap_errbuf[PCAP_ERRBUF_SIZE];

	FILE *fp;
	if(strcmp(fname, "-") == 0){
		fp = fdopen(0, "r"); /* stdin */
	}else{
		fp = fopen(fname, "r");
	}
	if(fp == NULL){
		ERROR("f(d)open: %s", strerror(errno));
	}
	HENDEL_PCAPFOPEN(*pcap, fp, pcap_errbuf);
	if(bpf_string != NULL){
		HENDEL_PCAPERR(*pcap, pcap_compile(*pcap, &pcap_filter, bpf_string, 1, 0));
		HENDEL_PCAPERR(*pcap, pcap_setfilter(*pcap, &pcap_filter));
	}
}

void pcapnet_close(pcap_args_t *pa){
	if(pa->dumper){
		pcap_dump_flush(pa->dumper);
		pcap_dump_close(pa->dumper);
		pa->dumper = NULL;
	}
	if(pa->handle_out){
		pcap_close(pa->handle_out);
		pa->handle_out = NULL;
	}
	if(pa->handle){
		pcap_close(pa->handle);
		pa->handle = NULL;
	}
	FREE(pa->dev);
	FREE(pa->dev_out);
	FREE(pa->fname);
	FREE(pa->fname_out);
}

void pcapnet_packet_loop(pcap_args_t *pa, pcap_handler cb){
	if(!pa || !(pa->handle))
		ERROR("pcap handle not initialized");
	pcap_loop(pa->handle, -1, cb, (void *)pa);
}

void pcapnet_break_loop(pcap_args_t *pa){
	if(!pa || !(pa->handle))
		ERROR("pcap handle not initialized");
	pcap_breakloop(pa->handle);
}

void pcapnet_parse_args(int argc, char **argv, const char *options,
		pcap_args_t *pa, void (*usage)(const char *msg), bool error){
	int c;
	while((c = getopt(argc, argv, options)) != EOF){
		switch(c) {
		case 'i':
			if(pa->can_readfile)
				pa->dev = strdup(optarg);
			else goto print_usage;
			break;
		case 'o':
			if(pa->can_inject)
				pa->dev_out = strdup(optarg);
			else goto print_usage;
			break;
		case 'r':
			if(pa->can_readfile)
				pa->fname = strdup(optarg);
			else goto print_usage;
			break;
		case 'w':
			if(pa->can_writefile)
				pa->fname_out = strdup(optarg);
			else goto print_usage;
			break;
		case 'f':
			pa->bpf_string = strdup(optarg);
			break;
		case 'p':
			pa->no_promisc = true;
			break;
		case 's':
			pa->snaplen = strtoul(optarg, NULL, 0);
			break;
		default:
			if(error)
				fprintf(stderr, "Error: unrecognized option");
		}
	}
	reset_getopt();
	return;
print_usage:
	if(usage)
		usage("unrecognized option");
	else{
		fprintf(stderr, "Error: unrecognized option");
		pcapnet_usage(pa);
	}
}

void pcapnet_usage(pcap_args_t *pa){
	fprintf(stderr, "pcapnet options:\n");
	if(pa->can_readif)
		fprintf(stderr, "\t[ -i <if> input interface ]\n");
	if(pa->can_inject)
		fprintf(stderr, "\t[ -o <if> output interface ]\n");
	if(pa->can_readfile)
		fprintf(stderr, "\t[ -r <pcap> input file ]\n");
	if(pa->can_writefile)
		fprintf(stderr, "\t[ -w <pcap> output file ]\n");
	fprintf(stderr,
		"\t[ -f <bpf filter> ]\n"
		"\t[ -s <snaplen> ]\n"
		"\t[ -p non-promiscuous ]\n"
	);
}

char *pcapnet_optstring(pcap_args_t *pa, const char *prog_options){
	char *all_pcapnet_options = "i:o:r:w:f:s:p";
	char *this_options;
	if(prog_options){
		this_options = calloc(1, 1 + strlen(all_pcapnet_options) + strlen(prog_options));
		strcat(this_options, prog_options);
	}else{
		this_options = calloc(1, 1 + strlen(all_pcapnet_options));
	}
	if(pa->can_readfile)
		strcat(this_options, "i:");
	if(pa->can_inject)
		strcat(this_options, "o:");
	if(pa->can_readfile)
		strcat(this_options, "r:");
	if(pa->can_writefile)
		strcat(this_options, "w:");
	strcat(this_options, "f:s:p");
	return this_options;
}
