#ifndef CUDAKERNEL
#define CUDAKERNEL

#include "cudaRelated.h"

__global__ void mapGrid(cudaGrid * gridArray, int * intermediate, int * gridnum, int * stride1d, int * stride2d) {
	int blockidx = blockIdx.x;
	int blockidy = blockIdx.y;
	int threadid = threadIdx.x;
	int roundtime = 0;
	float distance = 0;
	cudaGrid * offsetAddress = gridArray + stride1d[blockidx];
//	cudaGrid * curColGrid = 0;
//	cudaGrid * curRowGrid  =0;
//	int blockOffset = 0;
//	int threadOffset = 0;
//	//intermediate[0] = 100;
//	if (blockidy < gridnum[blockidx] && threadid < gridnum[blockidx]) {
//		if (gridnum[blockidx] > MAXTHREAD) {
//			roundtime = gridnum[blockidx] / MAXTHREAD + 1;
//		}
//		for (int k = 0; k < roundtime; ++k) {
//			blockOffset = blockidy + k * MAXTHREAD;
//			curRowGrid =offsetAddress + blockOffset ;
//			for (int j = 0; j < roundtime; ++j) {
//				threadOffset = threadid + j * MAXTHREAD;
//				curColGrid = offsetAddress + threadOffset;
//				distance = 0;
//				for (int i = 0; i < DIMENSION_CUDA; ++i) {
//					distance += powf(curRowGrid->cf[i] - curColGrid->cf[i], 2);
//				}
//				intermediate[stride2d[blockidx] +blockOffset* gridnum[blockidx] + threadOffset ] = 1;
//
//				//			if ( distance > 0.0) {// sqrtf(distance) <= EPS_CUDA &&
//				//				intermediate[offset * MAXTHREAD + blockidy * MAXTHREAD + threadid]= 1;
//				//			}
//			}
//		}
//	}
}
__global__ void reduceGrid(int * intermediate, int * gridnum) {

}

#endif

