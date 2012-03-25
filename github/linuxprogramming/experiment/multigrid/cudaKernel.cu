#ifndef CUDAKERNEL
#define CUDAKERNEL

#include "cudaRelated.h"

__global__ void mapGrid(cudaGrid * gridArray, int * intermediate, int * gridnum, int * stride1d, int * stride2d) {
	int blockidx = blockIdx.x;
	int blockidy = blockIdx.y;
	int threadid = threadIdx.x;
	int roundtime = 1;
	float distance = 0;
	float temp = 0;
	cudaGrid * offsetAddress = gridArray + stride1d[blockidx];
	cudaGrid * curColGrid = 0;
	cudaGrid * curRowGrid = 0;
	int blockOffset = 0;
	int threadOffset = 0;
	if (blockidy < gridnum[blockidx] && threadid < gridnum[blockidx]) {
		if (gridnum[blockidx] > MAXTHREAD) {
			roundtime = gridnum[blockidx] / MAXTHREAD + 1;
		}
		for (int k = 0; k < roundtime; ++k) {
			blockOffset = blockidy + k * MAXTHREAD;
			curRowGrid = offsetAddress + blockOffset;
			for (int j = 0; j < roundtime; ++j) {
				threadOffset = threadid + j * MAXTHREAD;
				curColGrid = offsetAddress + threadOffset;
				distance = 0;
				for (int i = 0; i < DIMENSION_CUDA; ++i) {
					temp = powf(curRowGrid->cf[i] - curColGrid->cf[i], 2);
					distance += temp;
				}
				if (distance > 0) {
					//					intermediate[1]=1;
					intermediate[stride2d[blockidx] + blockOffset * gridnum[blockidx] + threadOffset] = 1;
				}
			}
		}
	}
}

__global__ void reduceGrid(int * intermediate, int * gridnum) {

}


__device__ bool strcmp(char * str1,char * str2){
	while(*str1 && *str2 && *str1++ == *str2++);
	if(*str1 == '\0' && *str2 == '\0')return true;
	else return false;
}

__global__ void expand(cudaGrid * cudaArray,cudaGrid * curGrid, int * result,int type,int * d_pointCnt,int size,float eps,int dimension,int minpts){
	int threadid = threadIdx.x +blockIdx.x* MAXTHREAD;
	if(threadid >= size){
		return;
	}
	cudaGrid * grid = cudaArray + threadid;
	if(strcmp(curGrid->globalGridKey,grid->globalGridKey))return ;
	if(grid->status == SPARSE)return;
	if(type == FINDNEIGHBOR && grid->clusterID > UNCLASSIFIED && *d_pointCnt >= minpts)return;
	if(type == INCREMENTALFINDNEIGHBOR && grid->clusterID == curGrid->clusterID)return;
	float distance = 0;
	for(int i =0 ;i < dimension;++i){
		distance += powf(grid->cf[i] - curGrid->cf[i],2);
	}
	if(distance < eps *eps/* && distance !=0*/){
		result[threadid] = 1;
		atomicAdd(d_pointCnt,1);
	}

}

#endif

