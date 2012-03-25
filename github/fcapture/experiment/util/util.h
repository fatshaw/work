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

#ifndef UTIL_H
#define UTIL_H

#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

/* this file includes some structures and constants adapted from various
 * header files to aid portability */

/* macros */

#define INET_ADDRSTRLEN	16
#define IP_MIN_HLEN	20
#define UDP_HLEN	8
#define ETH_ALEN	6
#define ETH_HLEN	14
#define ETHERTYPE_IP	0x0800
#define ETHERTYPE_IPV6	0x86dd
#define ETHERTYPE_VLAN	0x8100

#define IPPROTO_TCP	6
#define IPPROTO_UDP	17

#define TCP_FIN 0x01
#define TCP_SYN 0x02
#define TCP_RST 0x04
#define TCP_PSH 0x08
#define TCP_ACK 0x10
#define TCP_URG 0x20
#define TCPFLAGS_STRLEN sizeof("FSRPAU")

#define DEBUG(format, ...) fprintf(stderr, "%s] " format "\n", __func__, ## __VA_ARGS__);
#define ERROR(format, ...) do{ \
	fprintf(stderr, "%s] Error: " format "\n", __func__, ## __VA_ARGS__); \
	exit(EXIT_FAILURE); \
}while(0)

#define MALLOC(target, size) do{ \
	target = malloc(size); \
	if(target == NULL){ \
		DEBUG("malloc failed, aborting"); \
		abort(); \
	} \
}while(0)

#define NEW(target) do{ \
	target = malloc(sizeof(*target)); \
	if(target == NULL){ \
		DEBUG("malloc failed, aborting"); \
		abort(); \
	} \
}while(0)

#define NEW0(target) do{ \
	target = calloc(1, sizeof(*target)); \
	if(target == NULL){ \
		DEBUG("malloc failed, aborting"); \
		abort(); \
	} \
}while(0)

#define FREE(target) do{ \
	if(target){ \
		free(target); \
		target = NULL; \
	} \
}while(0)

/* General-purpose swap macro from http://www.spinellis.gr/blog/20060130/index.html */
#define SWAP(a, b) do {\
	char c[sizeof(a)]; \
	memcpy((void *)&c, (void *)&a, sizeof(c)); \
	memcpy((void *)&a, (void *)&b, sizeof(a)); \
	memcpy((void *)&b, (void *)&c, sizeof(b)); \
} while (0)

#define likely(x)	__builtin_expect((x), 1)
#define unlikely(x)	__builtin_expect((x), 0)

#ifdef HAVE_SYS_ENDIAN_H
#include <sys/endian.h>
#else
#ifdef HAVE_MACHINE_ENDIAN_H
#include <machine/endian.h>
#else
#ifdef HAVE_ENDIAN_H
#include <endian.h>
#endif
#endif
#endif

#if defined(_BYTE_ORDER) && !defined(__BYTE_ORDER)
#define __BYTE_ORDER _BYTE_ORDER
#else
#if !defined(_BYTE_ORDER) && !defined(__BYTE_ORDER)
#error "unable to determine byte order"
#endif
#endif

#if defined(_LITTLE_ENDIAN) && !defined(__LITTLE_ENDIAN)
#define __LITTLE_ENDIAN _LITTLE_ENDIAN
#endif

#if defined(_BIG_ENDIAN) && !defined(__BIG_ENDIAN)
#define __BIG_ENDIAN _BIG_ENDIAN
#endif

#if __BYTE_ORDER == __LITTLE_ENDIAN
#define my_ntohs(x) bswap16(x)
#define my_htons(x) bswap16(x)
#define my_ntohl(x) bswap32(x)
#define my_htonl(x) bswap32(x)
#else
#define 
#define my_ntohs(x) (x)
#define my_htons(x) (x)
#define my_ntohl(x) (x)
#define my_htonl(x) (x)
#endif

/* declarations */

struct ether_header {
	uint8_t ether_dhost[ETH_ALEN];
	uint8_t ether_shost[ETH_ALEN];
	uint16_t ether_type;
} __attribute__ ((__packed__));

struct iphdr {
#if __BYTE_ORDER == __LITTLE_ENDIAN
	unsigned int ihl:4;
	unsigned int version:4;
#elif __BYTE_ORDER == __BIG_ENDIAN
	unsigned int version:4;
	unsigned int ihl:4;
#endif
	uint8_t tos;
	uint16_t tot_len;
	uint16_t id;
	uint16_t frag_off;
	uint8_t ttl;
	uint8_t protocol;
	uint16_t check;
	uint32_t saddr;
	uint32_t daddr;
};

struct tcphdr {
	uint16_t source;
	uint16_t dest;
	uint32_t seq;
	uint32_t ack_seq;
#if __BYTE_ORDER == __LITTLE_ENDIAN
	uint16_t res1:4;
	uint16_t doff:4;
	uint16_t fin:1;
	uint16_t syn:1;
	uint16_t rst:1;
	uint16_t psh:1;
	uint16_t ack:1;
	uint16_t urg:1;
	uint16_t res2:2;
#elif __BYTE_ORDER == __BIG_ENDIAN
	uint16_t doff:4;
	uint16_t res1:4;
	uint16_t res2:2;
	uint16_t urg:1;
	uint16_t ack:1;
	uint16_t psh:1;
	uint16_t rst:1;
	uint16_t syn:1;
	uint16_t fin:1;
#endif
	uint16_t window;
	uint16_t check;
	uint16_t urg_ptr;
};

struct udphdr {
	uint16_t source;
	uint16_t dest;
	uint16_t len;
	uint16_t check;
};

typedef uint32_t ipaddr_t;
typedef uint16_t port_t;

unsigned scan_ulong(const char *s, unsigned long *u);
unsigned fmt_ulong(char *s, unsigned long u);
unsigned scan_ip4(const char *s, ipaddr_t *n);
unsigned fmt_ip4(char *s, ipaddr_t n);
uint32_t bswap32(uint32_t x);
uint16_t bswap16(uint16_t x);
void reset_getopt(void);
void make_pidfile(char *pidfile);

#endif
