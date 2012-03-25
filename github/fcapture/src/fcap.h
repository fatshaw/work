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

#ifndef FCAP_H
#define FCAP_H

#include <stdbool.h>
#include <stdint.h>

#include "flow.h"
#include "util.h"

static const uint32_t cap_magic = 0x10231984;
static const uint16_t cap_prog = 1;
static const uint16_t cap_version = 1;
static const char cap_footer[] = "DEADBEEF";

static const char cmd_tar[] = "tar -Ozxf ";
static const char cmd_gzip[] = "gzip -cd ";
extern long offset;
extern char * category;

#define TCP 0
#define UDP 1
#define UNKNOWN 2

typedef struct cap_header {
	uint32_t magic;
	uint16_t prog;
	uint16_t version;
	uint64_t timestamp;
	uint64_t numentries;
} __attribute__((__packed__)) cap_header_t;

typedef struct flow_entry {
	int64_t bytes[2];
	int32_t pkts[2];
	uint32_t time[2];
	ipaddr_t ip[2];
	uint16_t proto;
	uint16_t port[2];
	uint16_t tcpflags[2];
	unsigned assured:1;
	unsigned direction:1;
	unsigned finished:1;
} __attribute__((__packed__)) flow_entry_t;

typedef struct fcap {
	FILE *file;
	char *fname;
	cap_header_t *hdr;
} fcap_t;

typedef struct fcap_reader {
	FILE *file;
	char *fname;
} fcap_reader_t;

typedef void (*fcap_iter_fn_t)(flow_entry_t *, void *);

fcap_t *fcap_new(time_t, char *logprefix);
void fcap_close(fcap_t *);
void fcap_add_flow_entry(fcap_t *fcap, flow_t *flow, ipaddr_pair_t ipp, proto_port_pair_t ppp, bool finished);
bool fcap_verify_header(cap_header_t *hdr);
fcap_reader_t *fcap_open(char *fcap_readfile);
void fcap_close_reader(fcap_reader_t *fr);
int fcap_read_iter(fcap_reader_t *fr, fcap_iter_fn_t fn, void *user);

int fcap_write_mmap();

int fcap_add_mmap_buf(flow_entry_t * flow);


#endif
