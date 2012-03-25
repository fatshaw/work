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

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <Judy.h>

#include "fcap.h"
#include "flow.h"
#include "util.h"

long FLOW_GC_INTERVAL = 30;
#define TIMEOUT 5 


/* forward declarations */

static inline enum RC_FLOW flow_update_tcp(flow_t *f, struct tcphdr *t, int direction);
static inline flow_t *flow_get(flow_table_t *flt, flow_headerset_t *hs, int *direction, enum RC_FLOW *rc);
static enum RC_ITER flow_gc_fcap(void *, flow_table_t *, flow_t *, ipaddr_pair_t, proto_port_pair_t);
static enum RC_ITER flow_gc_fcap_shutdown(void *user, flow_table_t *flt, flow_t *flow, ipaddr_pair_t ipp, proto_port_pair_t pp);

/* functions */

flow_table_t *flow_new(char *logprefix){
	flow_table_t *flt;
	NEW0(flt);
	flt->logprefix = strdup(logprefix);
	return flt;
}

void flow_destroy(flow_table_t **flt){
	JudyLFreeArray((*flt)->array, PJE0);
	FREE((*flt)->logprefix);
	FREE(*flt);
}

enum RC_FLOW flow_update(flow_table_t *flt, flow_headerset_t *hs){
	enum RC_FLOW rc = RC_FLOW_UPDATED;

	if(!(hs->ip->protocol == IPPROTO_TCP || hs->ip->protocol == IPPROTO_UDP)){
		return RC_FLOW_INVALID_PROTO;
	}

	flt->pcap_time = hs->h->ts.tv_sec;
	time_t pcap_interval_new = flt->pcap_time - (flt->pcap_time % FLOW_GC_INTERVAL);
	if(flt->pcap_interval != pcap_interval_new){
		if(flt->pcap_interval != 0)
			flow_fcap_dump(flt, pcap_interval_new, false);
		flt->pcap_interval = pcap_interval_new;
	}

	int direction = 0;
	flow_t *f = flow_get(flt, hs, &direction, &rc);
	
	/* update flow */
	f->time[1] = (uint32_t)hs->h->ts.tv_sec;
	//f->time[1] = (uint32_t) (hs->h->ts.tv_sec*1E3 + hs->h->ts.tv_usec /1E3);
	f->pkts[direction]++;
	f->bytes[direction] += hs->h->len;
	if(f->proto == IPPROTO_TCP){
		if(flow_update_tcp(f, hs->t.tcp, direction) == RC_FLOW_ASSURED)
			rc = RC_FLOW_ASSURED;
	}
	return rc;
}

#if defined(_LP64)
static inline flow_t *flow_get(flow_table_t *flt, flow_headerset_t *hs, int *direction, enum RC_FLOW *rc){
	ipaddr_pair_t ipp;
	proto_port_pair_t ppp;
	flow_t **flow;
	flow_t *f = NULL;
	void **val_ipp;

	/* extract ip and port pairs */
	if(hs->ip->saddr < hs->ip->daddr){
		ipp = ipaddr_pair(hs->ip->saddr, hs->ip->daddr);
		ppp = proto_port_pair(hs->ip->protocol, my_ntohs(hs->t.tcp->source), my_ntohs(hs->t.tcp->dest));
	}else{
		*direction = 1;
		ipp = ipaddr_pair(hs->ip->daddr, hs->ip->saddr);
		ppp = proto_port_pair(hs->ip->protocol, my_ntohs(hs->t.tcp->dest), my_ntohs(hs->t.tcp->source));
	}
	/* (ip[0], ip[1]) key */
	if(NULL == (val_ipp = (void **) JudyLGet(flt->array, ipp, PJE0))){
		/* new ip pair */
		val_ipp = JudyLIns((void **) &flt->array, ipp, PJE0);
		*val_ipp = NULL;
	}
	/* (ip proto, port[0], port[1]) key */
	if(NULL == (flow = (flow_t **) JudyLGet(*val_ipp, ppp, PJE0))){
		/* initialize new flow */
		*rc = RC_FLOW_NEW;
		flow = (flow_t **) JudyLIns(val_ipp, ppp, PJE0);
		NEW0(*flow);
		f = *flow;
		f->time[0] = f->lu_time = (uint32_t) hs->h->ts.tv_sec;
//		f->time[0] = f->lu_time = (uint32_t) (hs->h->ts.tv_sec*1000 + hs->h->ts.tv_usec / 1000);
		
		f->direction = *direction;
		f->proto = hs->ip->protocol;

		if(hs->ip->protocol == IPPROTO_TCP){
			flow_tcp_t *ft;
			NEW0(ft);
			f->tcp = ft;
		}
	}else{
		/* existing flow */
		f = *flow;
	}
	return f;
}

#else
static inline flow_t *flow_get(flow_table_t *flt, flow_headerset_t *hs, int *direction, enum RC_FLOW *rc){
	ipaddr_t ip[2];
	port_pair_t pp;
	void **val_ip[2];
	void **val_proto;
	flow_t **flow;
	flow_t *f;

	/* extract ip and port pairs */
	ip[0] = hs->ip->saddr;
	ip[1] = hs->ip->daddr;
	if(ip[0] < ip[1]){
		pp = port_pair(my_ntohs(hs->t.tcp->source), my_ntohs(hs->t.tcp->dest));
	}else{
		*direction = 1;
		SWAP(ip[0], ip[1]);
		pp = port_pair(my_ntohs(hs->t.tcp->dest), my_ntohs(hs->t.tcp->source));
	}
	/* ip[0] key */
	if(NULL == (val_ip[0] = (void **) JudyLGet(flt->array, ip[0], PJE0))){
		val_ip[0] = JudyLIns((void **) &flt->array, ip[0], PJE0);
		*val_ip[0] = NULL;
	}
	/* ip[1] key */
	if(NULL == (val_ip[1] = (void **) JudyLGet(*val_ip[0], ip[1], PJE0))){
		val_ip[1] = JudyLIns(val_ip[0], ip[1], PJE0);
		*val_ip[1] = NULL;
	}
	/* ip proto key */
	if(NULL == (val_proto = (void **) JudyLGet(*val_ip[1], hs->ip->protocol, PJE0))){
		val_proto = JudyLIns(val_ip[1], hs->ip->protocol, PJE0);
		*val_proto = NULL;
	}
	/* (port[0], port[1]) key */
	if(NULL == (flow = (flow_t **) JudyLGet(*val_proto, pp, PJE0))){
		/* initialize new flow */
		*rc = RC_FLOW_NEW;
		flow = (flow_t **) JudyLIns(val_proto, pp, PJE0);
		NEW0(*flow);
		f = *flow;
f->time[0] = f->lu_time = (uint32_t) hs->h->ts.tv_sec;
//		f->time[0] = f->lu_time = (uint32_t) hs->h->ts.tv_sec*1E3+ hs->h->ts.tv_usec /1E3;
		f->direction = *direction;
		f->proto = hs->ip->protocol;

		if(hs->ip->protocol == IPPROTO_TCP){
			flow_tcp_t *ft;
			NEW0(ft);
			f->tcp = ft;
		}
	}else{
		/* existing flow */
		f = *flow;
	}
	return f;
}
#endif

static inline enum RC_FLOW flow_update_tcp(flow_t *f, struct tcphdr *t, int direction){
	if(t->fin) f->tcp->flags[direction] |= TCP_FIN;
	if(t->syn) f->tcp->flags[direction] |= TCP_SYN;
	if(t->rst) f->tcp->flags[direction] |= TCP_RST;
	if(t->psh) f->tcp->flags[direction] |= TCP_PSH;
	if(t->ack) f->tcp->flags[direction] |= TCP_ACK;
	if(t->urg) f->tcp->flags[direction] |= TCP_URG;
	f->tcp->seq[direction] = my_ntohl(t->seq);
	f->tcp->ack[direction] = my_ntohl(t->ack_seq);
	if(
		!(f->tcp->assured) &&
		f->pkts[0] > 0 &&
		f->pkts[1] > 0 &&
		((f->tcp->ack[1] - f->tcp->seq[0] == 0) ||
		 (f->tcp->ack[0] - f->tcp->seq[1] == 0))
	){
		f->tcp->assured = 1;
		return RC_FLOW_ASSURED;
	}
	return RC_FLOW_UPDATED;
}

void flow_iter(flow_table_t *flt, flow_iter_fn_t fn, void *user){
	if(flt == NULL || flt->array == NULL)
		return;

	flow_t *flow = NULL;
	int rc = RC_ITER_NOOP;

	uint64_t count_flow = 0;
	uint64_t count_flow_tcp = 0;
	uint64_t count_flow_udp = 0;
	uint64_t count_flow_del = 0;
	uint64_t count_ip_pairs = 0;
	uint64_t count_pkt = 0;
	
	struct timeval tv_start;
	struct timeval tv_end;
	gettimeofday(&tv_start, NULL);

#if defined(_LP64)
	Word_t idx[2];
	void **val[2];
	ipaddr_pair_t ipp = 0;
	proto_port_pair_t ppp;

	idx[0] = idx[1] = 0;

	while((val[0] = (void **) JudyLNext(flt->array, &idx[0], PJE0)) != NULL){
		++count_ip_pairs;
		ipp = idx[0];
		while((val[1] = (void **) JudyLNext(*val[0], &idx[1], PJE0)) != NULL){
			++count_flow;
			ppp = idx[1];
			flow = (flow_t *) (*val[1]);
			count_pkt += flow->pkts[0] + flow->pkts[1];
			if(flow->proto == IPPROTO_TCP) ++count_flow_tcp;
			if(flow->proto == IPPROTO_UDP) ++count_flow_udp;

			if(fn){
				rc = fn(user, flt, flow, ipp, ppp);
				if(rc == RC_ITER_DEL){
					++count_flow_del;
					if(flow->tcp) FREE(flow->tcp);
					FREE(flow);
					JudyLDel(val[0], ppp, PJE0);
				}
			}

		}
		if(JudyLCount(*val[0], 0L, -1L, PJE0) == 0){
			JudyLDel((void **) &flt->array, ipp, PJE0);
		}
		idx[1] = 0;
	}
#else
	Word_t idx[4];
	void **val[4];
	ipaddr_pair_t ipp = 0;
	proto_port_pair_t ppp = 0;

	idx[0] = idx[1] = idx[2] = idx[3] = 0;

	/* ip0 */
	while((val[0] = (void **) JudyLNext(flt->array, &idx[0], PJE0)) != NULL){
		/* ip1 */
		while((val[1] = (void **) JudyLNext(*val[0], &idx[1], PJE0)) != NULL){
			ipp = ipaddr_pair(idx[0], idx[1]);
			++count_ip_pairs;
			/* ip proto */
			while((val[2] = (void **) JudyLNext(*val[1], &idx[2], PJE0)) != NULL){
				/* port pair */
				while((val[3] = (void **) JudyLNext(*val[2], &idx[3], PJE0)) != NULL){
					flow = *(flow_t **) val[3];
					ppp = proto_port_pair(idx[2], port_pair_p0(idx[3]), port_pair_p1(idx[3]));
					++count_flow;
					count_pkt += flow->pkts[0] + flow->pkts[1];
					if(flow->proto == IPPROTO_TCP) ++count_flow_tcp;
					if(flow->proto == IPPROTO_UDP) ++count_flow_udp;

					if(fn){
						rc = fn(user, flt, flow, ipp, ppp);
						if(rc == RC_ITER_DEL){
							++count_flow_del;
							if(flow->tcp) FREE(flow->tcp);
							FREE(flow);
							JudyLDel(val[2], idx[3], PJE0);
						}
					}
				}
				idx[3] = 0;
				if(JudyLCount(*val[2], 0, -1, PJE0) == 0){
					JudyLFreeArray(val[2], PJE0);
					JudyLDel(val[1], idx[2], PJE0);
				}
			}
			idx[2] = 0;
			if(JudyLCount(*val[1], 0, -1, PJE0) == 0){
				JudyLFreeArray(val[1], PJE0);
				JudyLDel(val[0], idx[1], PJE0);
			}
		}
		idx[1] = 0;
		if(JudyLCount(*val[0], 0, -1, PJE0) == 0){
			JudyLFreeArray(val[0], PJE0);
			JudyLDel((void **) &flt->array, idx[0], PJE0);
		}
	}
#endif
	gettimeofday(&tv_end, NULL);
	double start = tv_start.tv_sec + tv_start.tv_usec / 1E6;
	double end = tv_end.tv_sec + tv_end.tv_usec / 1E6;
	DEBUG("%" PRIi64 "] counted %" PRIu64 " flows (%" PRIu64 " tcp, %" PRIu64 " udp), "
		"%" PRIu64 " packets, %" PRIu64 " ip pairs, %" PRIu64 " deleted in %.3f seconds",
		(int64_t) flt->pcap_time,
		count_flow, count_flow_tcp, count_flow_udp, count_pkt, count_ip_pairs, count_flow_del,
		end - start);
}

void flow_fcap_dump(flow_table_t *flt, time_t tm, bool stop_running){
	fcap_t *fcap = fcap_new(tm, flt->logprefix);
	if(fcap == NULL){
		ERROR("fcap_new() failed, shutting down");
	}
	if(stop_running){
		flow_iter(flt, flow_gc_fcap_shutdown, fcap);
	}else{
		flow_iter(flt, flow_gc_fcap, fcap);
	}
	fcap_close(fcap);
	return;
}

enum RC_ITER flow_print(void *user, flow_table_t *flt, flow_t *flow, ipaddr_pair_t ipp, proto_port_pair_t ppp){
	char sip0[INET_ADDRSTRLEN];
	char sip1[INET_ADDRSTRLEN];
	fmt_ip4(sip0, ipaddr_pair_ip0(ipp));
	fmt_ip4(sip1, ipaddr_pair_ip0(ipp));

	char flagbuf[64];
	flagbuf[0] = '\0';
	if(flow->proto == IPPROTO_TCP){
		snprintf(flagbuf, sizeof(flagbuf), "flags0=[%s%s%s%s%s%s] flags1=[%s%s%s%s%s%s] %s",
			flow->tcp->flags[0] & TCP_FIN ? "F" : " ",
			flow->tcp->flags[0] & TCP_SYN ? "S" : " ",
			flow->tcp->flags[0] & TCP_RST ? "R" : " ",
			flow->tcp->flags[0] & TCP_PSH ? "P" : " ",
			flow->tcp->flags[0] & TCP_ACK ? "A" : " ",
			flow->tcp->flags[0] & TCP_URG ? "U" : " ",
			flow->tcp->flags[1] & TCP_FIN ? "F" : " ",
			flow->tcp->flags[1] & TCP_SYN ? "S" : " ",
			flow->tcp->flags[1] & TCP_RST ? "R" : " ",
			flow->tcp->flags[1] & TCP_PSH ? "P" : " ",
			flow->tcp->flags[1] & TCP_ACK ? "A" : " ",
			flow->tcp->flags[1] & TCP_URG ? "U" : " ",
			flow->tcp->assured ? " [ASSURED]" : ""
		);
	}

	DEBUG("ip0=%s ip1=%s proto=%hu port0=%hu port1=%hu time0=%u time1=%u pkts0=%u pkts1=%u "
		"bytes0=%" PRIu64 " bytes1=%" PRIu64 " direction=%u %s",
		sip0, sip1, flow->proto,
		proto_port_pair_p0(ppp), proto_port_pair_p1(ppp),
		flow->time[0], flow->time[1],
		flow->pkts[0], flow->pkts[1],
		flow->bytes[0], flow->bytes[1],
		flow->direction,
		flagbuf
	);
	return RC_ITER_NOOP;
}

enum RC_ITER flow_delete(void *user, flow_table_t *flt, flow_t *flow, ipaddr_pair_t ipp, proto_port_pair_t ppp){
	return RC_ITER_DEL;
}

enum RC_ITER flow_gc(void *user, flow_table_t *flt, flow_t *flow, ipaddr_pair_t ipp, proto_port_pair_t ppp){
	if(flow->tcp && flow->tcp->assured && (flow->tcp->flags[0] & TCP_FIN && flow->tcp->flags[1] & TCP_FIN)){
		return RC_ITER_DEL;
	}
	if(flow->tcp && flow->tcp->assured && (flow->tcp->flags[0] & TCP_RST && flow->tcp->flags[1] & TCP_RST)){
		return RC_ITER_DEL;
	}
	if(flow->tcp && flow->tcp->assured
		&& (flt->pcap_time - flow->time[0] >= TIMEOUT_TCP_ESTABLISHED)
		&& (flt->pcap_time - flow->time[1] >= TIMEOUT_TCP_ESTABLISHED)){
		return RC_ITER_DEL;
	}
	if(flow->tcp
		&& (flt->pcap_time - flow->time[0] >= TIMEOUT_TCP_OPENING)
		&& (flt->pcap_time - flow->time[1] >= TIMEOUT_TCP_OPENING)){
		return RC_ITER_DEL;
	}
	if(flow->proto == IPPROTO_UDP
		&& (flt->pcap_time - flow->time[0] >= TIMEOUT_UDP)
		&& (flt->pcap_time - flow->time[1] >= TIMEOUT_UDP)){
		return RC_ITER_DEL;
	}
	return RC_ITER_NOOP;
}

static enum RC_ITER flow_gc_fcap(void *user, flow_table_t *flt, flow_t *flow, ipaddr_pair_t ipp, proto_port_pair_t ppp){
	fcap_t *fcap = (fcap_t *) user;

	if(flow->tcp && flow->tcp->assured && (flow->tcp->flags[0] & TCP_FIN && flow->tcp->flags[1] & TCP_FIN)){
		fcap_add_flow_entry(fcap, flow, ipp, ppp, true);
		return RC_ITER_DEL;
	}
	if(flow->tcp && flow->tcp->assured && (flow->tcp->flags[0] & TCP_RST && flow->tcp->flags[1] & TCP_RST)){
		fcap_add_flow_entry(fcap, flow, ipp, ppp, true);
		return RC_ITER_DEL;
	}
	if(flow->tcp && flow->tcp->assured
		&& (flt->pcap_time - flow->time[0] >= TIMEOUT_TCP_ESTABLISHED)
		&& (flt->pcap_time - flow->time[1] >= TIMEOUT_TCP_ESTABLISHED)){
		fcap_add_flow_entry(fcap, flow, ipp, ppp, true);
		return RC_ITER_DEL;
	}
	if(flow->tcp
		&& (flt->pcap_time - flow->time[0] >= TIMEOUT_TCP_OPENING)
		&& (flt->pcap_time - flow->time[1] >= TIMEOUT_TCP_OPENING)){
		fcap_add_flow_entry(fcap, flow, ipp, ppp, true);
		return RC_ITER_DEL;
	}
	if(flow->proto == IPPROTO_UDP
		&& (flt->pcap_time - flow->time[0] >= TIMEOUT_UDP)
		&& (flt->pcap_time - flow->time[1] >= TIMEOUT_UDP)){
		fcap_add_flow_entry(fcap, flow, ipp, ppp, true);
		return RC_ITER_DEL;
	}
	if(flt->pcap_time - flow->lu_time >= TIMEOUT){
		fcap_add_flow_entry(fcap, flow, ipp, ppp, false);
		flow->lu_time = flt->pcap_time;
		return RC_ITER_DEL;
	}
	return RC_ITER_NOOP;
}

static enum RC_ITER flow_gc_fcap_shutdown(void *user, flow_table_t *flt, flow_t *flow, ipaddr_pair_t ipp, proto_port_pair_t ppp){
	fcap_t *fcap = (fcap_t *) user;
	fcap_add_flow_entry(fcap, flow, ipp, ppp, false);
	return RC_ITER_DEL;
}
