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

#ifndef FLOW_H
#define FLOW_H

#include <stdint.h>

#include <Judy.h>
#include <pcap.h>

#define TIMEOUT_TCP_ESTABLISHED 3600
#define TIMEOUT_TCP_OPENING 120
#define TIMEOUT_UDP 120
//#define FLOW_GC_INTERVAL 300

#define UNCLASSIFIED "unclassified"

extern long FLOW_GC_INTERVAL;

enum RC_FLOW {
	RC_FLOW_INVALID_PROTO,
	RC_FLOW_UPDATED,
	RC_FLOW_ASSURED,
	RC_FLOW_NEW
};

enum RC_ITER {
	RC_ITER_NOOP,
	RC_ITER_DEL
};

typedef uint32_t port_pair_t;
typedef uint64_t proto_port_pair_t;
typedef uint64_t ipaddr_pair_t;

#define port_pair(p0, p1) ((uint32_t)(((port_t)(p0) << 16 | (port_t)(p1))))
#define port_pair_p0(pp) ((port_t)(pp >> 16))
#define port_pair_p1(pp) ((port_t)(pp & 0xffff))

#define proto_port_pair(proto, p0, p1) (((uint64_t)(((port_t)(p0) << 16 | (port_t)(p1)))) | ((uint64_t)proto << 32))
#define proto_port_pair_p0(ppp) ((port_t)((ppp & 0xffffffff) >> 16))
#define proto_port_pair_p1(ppp) ((port_t)(ppp & 0xffff))
#define proto_port_pair_proto(ppp) ((uint16_t)((ppp >> 32) & 0xffff))

#define ipaddr_pair(ip0, ip1) ((uint64_t)(ip0) << 32 | (uint64_t)(ip1))
#define ipaddr_pair_ip0(ipp) ((ipaddr_t)(ipp >> 32))
#define ipaddr_pair_ip1(ipp) ((ipaddr_t)(ipp & 0xffffffff))

typedef struct flow_table {
	Pvoid_t array;
	time_t pcap_time;
	time_t pcap_interval;
	char *logprefix;
} flow_table_t;

typedef struct flow_tcp {
	uint32_t seq[2];
	uint32_t ack[2];
	uint16_t flags[2];
	unsigned assured:1;
} __attribute__((__packed__)) flow_tcp_t;

typedef struct flow {
	uint32_t time[2];
	int32_t pkts[2];
	int64_t bytes[2];
	flow_tcp_t *tcp;
	uint32_t lu_time;
	uint16_t proto;
	unsigned direction:1;
} flow_t;

typedef struct flow_headerset {
	const struct pcap_pkthdr *h;
	struct iphdr *ip;
	union {
		struct tcphdr *tcp;
		struct udphdr *udp;
	} t;
} flow_headerset_t;

typedef union transport_hdr {
	struct tcphdr *tcp;
	struct udphdr *udp;
} transport_hdr;

typedef enum RC_ITER (*flow_iter_fn_t)(void *, flow_table_t *, flow_t *, ipaddr_pair_t, proto_port_pair_t);
enum RC_ITER flow_gc(void *, flow_table_t *, flow_t *, ipaddr_pair_t, proto_port_pair_t);
enum RC_ITER flow_print(void *, flow_table_t *, flow_t *, ipaddr_pair_t, proto_port_pair_t);
enum RC_ITER flow_delete(void *, flow_table_t *, flow_t *, ipaddr_pair_t, proto_port_pair_t);

flow_table_t *flow_new(char *logprefix);
void flow_destroy(flow_table_t **);
void flow_iter(flow_table_t *, flow_iter_fn_t, void *);
void flow_fcap_dump(flow_table_t *, time_t, bool stop_running);
enum RC_FLOW flow_update(flow_table_t *, flow_headerset_t *hs);

#endif
