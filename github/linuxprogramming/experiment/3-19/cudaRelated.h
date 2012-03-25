/*
 * cudaRelatedHeadh.h
 *
 *  Created on: 2010-3-18
 *      Author: fatshaw
 */

#ifndef CUDARELATEDHEADH_H_
#define CUDARELATEDHEADH_H_
#include "ConstVar.h"

struct cudaGrid{
	char globalGridKey[73];//36*2 + 1
	float cf[36];

};

void cudaMap(cudaGrid * cudaArray,int localGridNum,int * gridnum,int * stride1d,int * stride2d,int * h_intermediate);
int* cudaAlloc(int size);
void cudaDealloc(int* address);
#endif /* CUDARELATEDHEADH_H_ */
