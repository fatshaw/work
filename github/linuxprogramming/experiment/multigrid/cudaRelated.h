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
	int status;
	float cf[36];
	int clusterID;

};
void cudaMemFree(void * address);
void cudaMemAlloc( void **address,int size,bool isMemset  = false);
void cudaDistance(cudaGrid * h_cudaArray,cudaGrid * d_cudeArray,cudaGrid * h_curGrid,cudaGrid * d_curGrid,int size,int * d_result,int * h_result,int type,int * h_pointCnt,int * d_pointCnt);
void cudaMap(cudaGrid * cudaArray,int localGridNum,int * gridnum,int * stride1d,int * stride2d,int * h_intermediate);
void cudaMemoryCopy(void * h_address, void * d_address,int size);
void memAlloc(void ** address,int size);
void memDealloc(void * address);
#endif /* CUDARELATEDHEADH_H_ */
