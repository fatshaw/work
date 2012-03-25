#include "common.h"
#include "apipoold_protocol.h"

unsigned long getTimeSpan(struct timeval tv_begin, struct timeval tv_end){
	unsigned long timespan = (tv_end.tv_sec - tv_begin.tv_sec)*1000000 
				+ ((tv_end.tv_usec >= tv_begin.tv_usec) ? (tv_end.tv_usec - tv_begin.tv_usec) : (tv_begin.tv_usec - tv_end.tv_usec));
	return timespan;
}

