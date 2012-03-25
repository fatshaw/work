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
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "fcap.h"
#include "flow.h"
#include "util.h"

/* functions */

fcap_t *fcap_new(time_t t, char *logprefix){
	fcap_t *fcap;
	NEW0(fcap);

	cap_header_t *caphdr;
	NEW0(caphdr);

	fcap->hdr = caphdr;
	fcap->hdr->magic = cap_magic;
	fcap->hdr->prog = cap_prog;
	fcap->hdr->version = cap_version;
	fcap->hdr->timestamp = (uint64_t) t;

	struct tm *the_time = gmtime(&t);
	char buf[64];
	strftime(buf, sizeof(buf), ".%Y%m%d.%H%M.%s.fcap", the_time);

	fcap->fname = calloc(strlen(logprefix) + strlen(buf) + 2, 1);
	strcat(fcap->fname, logprefix);
	strcat(fcap->fname, buf);
	DEBUG("opening fcap %s", fcap->fname);

	if((fcap->file = fopen(fcap->fname, "w")) == NULL){
		DEBUG("unable to open fcap: %s", strerror(errno));
		FREE(fcap->fname);
		FREE(fcap);
	}else{
		fwrite(fcap->hdr, sizeof(*(fcap->hdr)), 1, fcap->file);
	}
	return fcap;
}

void fcap_close(fcap_t *fcap){
	DEBUG("closing fcap %s", fcap->fname);
	fwrite(cap_footer, sizeof(cap_footer) - 1, 1, fcap->file);
	rewind(fcap->file);
	fwrite(fcap->hdr, sizeof(*(fcap->hdr)), 1, fcap->file);
	if(fclose(fcap->file) != 0){
		DEBUG("error closing fcap: %s", strerror(errno));
	}
	FREE(fcap->fname);
	FREE(fcap->hdr);
	FREE(fcap);
}

void fcap_add_flow_entry(fcap_t *fcap, flow_t *flow, ipaddr_pair_t ipp, proto_port_pair_t ppp, bool finished){
	flow_entry_t fe;
	memset(&fe, 0, sizeof(fe));

	fe.bytes[0] = flow->bytes[0];
	fe.bytes[1] = flow->bytes[1];
	fe.pkts[0] = flow->pkts[0];
	fe.pkts[1] = flow->pkts[1];
	fe.time[0] = flow->time[0];
	fe.time[1] = flow->time[1];
	fe.ip[0] = ipaddr_pair_ip0(ipp);
	fe.ip[1] = ipaddr_pair_ip1(ipp);
	fe.port[0] = proto_port_pair_p0(ppp);
	fe.port[1] = proto_port_pair_p1(ppp);
	fe.proto = flow->proto;
	fe.direction = flow->direction;
	fe.finished = finished ? 1 : 0;
	if(flow->tcp && fe.proto == IPPROTO_TCP){
		fe.tcpflags[0] = flow->tcp->flags[0];
		fe.tcpflags[1] = flow->tcp->flags[1];
		fe.assured = flow->tcp->assured;
	}
	fwrite(&fe, sizeof(fe), 1, fcap->file);
	fcap->hdr->numentries += 1;
}

bool fcap_verify_header(cap_header_t *hdr){
	if(
		hdr->magic == cap_magic &&
		hdr->prog == cap_prog &&
		hdr->version == cap_version
	){
		return true;
	}else{
		return false;
	}
}

fcap_reader_t *fcap_open(char *fcap_readfile){
	fcap_reader_t *fr;
	NEW0(fr);
	char *cmd = NULL;
	
	if(strstr(fcap_readfile, ".fcap.tgz")){
		cmd = calloc(sizeof(cmd_tar) + strlen(fcap_readfile) + 1, 1);
		strcat(cmd, cmd_tar);
		strcat(cmd, fcap_readfile);
		if((fr->file = popen(cmd, "r")) == NULL)
			goto open_err;
	}else if(strstr(fcap_readfile, ".fcap.gz")){
		cmd = calloc(sizeof(cmd_gzip) + strlen(fcap_readfile) + 1, 1);
		strcat(cmd, cmd_gzip);
		strcat(cmd, fcap_readfile);
		if((fr->file = popen(cmd, "r")) == NULL)
			goto open_err;
	}else if(strcmp(fcap_readfile, "-") == 0){
		if((fr->file = fdopen(0, "r")) == NULL)
			goto open_err;
	}else if((fr->file = fopen(fcap_readfile, "r")) == NULL){
		goto open_err;
	}

	FREE(cmd);
	fr->fname = strdup(fcap_readfile);
	return fr;

open_err:
	FREE(cmd);
	DEBUG("Error: %s", strerror(errno));
	FREE(fr);
	return NULL;
}

void fcap_close_reader(fcap_reader_t *fr){
	fclose(fr->file);
	FREE(fr->fname);
	FREE(fr);
}

int fcap_read_iter(fcap_reader_t *fr, fcap_iter_fn_t fn, void *user){
	for(;;){
		cap_header_t hdr;
		fread(&hdr, sizeof(hdr), 1, fr->file);
		if(feof(fr->file))
			break;
		if(!fcap_verify_header(&hdr)){
			DEBUG("unknown/invalid fcap header");
			return -1;
		}else{
			time_t tm = (time_t) hdr.timestamp;
			char *stm = asctime(gmtime(&tm));
			stm[strlen(stm) - 1] = '\0';
			/*DEBUG("opening fcap dump file %s (%" PRIu64 " records) from %s UTC",
				fr->fname, hdr.numentries, stm);*/
		}
		
		ssize_t r;
		flow_entry_t fe;
		uint64_t numentry = 0;
		for(; numentry < hdr.numentries ; ++numentry){
			r = fread(&fe, sizeof(fe), 1, fr->file);
			if(feof(fr->file))
				break;
			if(r != 1){
				DEBUG("fread(3) returned %zd", r);
				return -1;
			}
			if(fe.direction){
				// swap direction-dependent values
				SWAP(fe.bytes[0], fe.bytes[1]);
				SWAP(fe.pkts[0], fe.pkts[1]);
				SWAP(fe.ip[0], fe.ip[1]);
				SWAP(fe.port[0], fe.port[1]);
				SWAP(fe.tcpflags[0], fe.tcpflags[1]);
			}
			fn(&fe, user);
		}
		char buf[sizeof(cap_footer)];
		fread(buf, sizeof(cap_footer) -1, 1, fr->file);
		if(memcmp(buf, cap_footer, sizeof(cap_footer - 1)) != 0){
			DEBUG("corrupt footer");
			return -1;
		}
	}

	return 0;
}
