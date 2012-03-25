#include "cudaRelated.h"
#include <cutil.h>
#include <cutil_inline.h>
#include "cudaKernel.cu"
#include <iostream>
using namespace std;

void memAlloc(void ** address, int size) {
	cudaHostAlloc(address, size, cudaHostAllocDefault);
}

void memDealloc(void * address) {
	if (address != 0)
		cudaFreeHost(address);
}

void cudaMemAlloc(void **address, int size,bool isMemset) {
	cudaMalloc(address, size);
	if(isMemset == true){
		cudaMemset(address, 0, size);
	}
}
void cudaMemFree(void * address) {
	if (address != 0)
		cudaFree(address);
}

void cudaMemoryCopy(void * h_address, void * d_address,int size){
	cudaMemcpy(d_address, h_address,size, cudaMemcpyHostToDevice);
}

void cudaDistance(cudaGrid * h_cudaArray, cudaGrid * d_cudeArray, cudaGrid * h_curGrid, cudaGrid * d_curGrid, int size, int * d_result, int * h_result,int type,int * h_pointCnt,int * d_pointCnt) {
	unsigned int timer = 0;
	cutCreateTimer(&timer);
	cutStartTimer(timer);
	cudaMemset(d_result,0,sizeof(int)*size);
	expand<<<size/MAXTHREAD,MAXTHREAD>>>(d_cudeArray,d_curGrid,d_result,type,d_pointCnt,size,eps,DIMENSION,minPts);
	cudaMemcpy(h_pointCnt,d_pointCnt,sizeof(int),cudaMemcpyDeviceToHost);
	cudaMemcpy(h_result, d_result, sizeof(int)*size, cudaMemcpyDeviceToHost);
	cudaThreadSynchronize();
	cutStopTimer(timer);
	//cout << "timer =  " << cutGetTimerValue(timer) << endl;
}

void cudaMap(cudaGrid * cudaArray, int localGridNum, int * gridnum, int * stride1d, int * stride2d, int * h_intermediate)//h_intermediate will be allocated outside.
{
	unsigned int timer = 0;
	for (int i = 0; i < 100; i++) {
		cout << h_intermediate[i] << ",";
	}
	cout << endl;
	int globalGridNum = stride1d[localGridNum - 1] + gridnum[localGridNum - 1];
	int globalGridNum2 = stride2d[localGridNum - 1] + gridnum[localGridNum - 1] * gridnum[localGridNum - 1];
	cutCreateTimer(&timer);
	cutStartTimer(timer);

	cudaGrid * d_cudaGrid = 0;
	cudaMalloc((void**) &d_cudaGrid, sizeof(cudaGrid) * globalGridNum);
	cudaMemset(d_cudaGrid, 0, sizeof(cudaGrid) * globalGridNum);
	cudaMemcpy(d_cudaGrid, cudaArray, sizeof(cudaGrid) * globalGridNum, cudaMemcpyHostToDevice);

	int * d_intermediate = 0;
	cudaMalloc((void**) &d_intermediate, sizeof(int) * globalGridNum2);
	cudaMemset(d_intermediate, 0, sizeof(int) * globalGridNum2);

	int * d_gridnum = 0;
	cudaMalloc((void**) &d_gridnum, sizeof(int) * localGridNum);
	cudaMemcpy(d_gridnum, gridnum, sizeof(int) * localGridNum, cudaMemcpyHostToDevice);

	int * d_stride1d = 0;
	cudaMalloc((void**) &d_stride1d, sizeof(int) * localGridNum);
	cudaMemcpy(d_stride1d, stride1d, sizeof(int) * localGridNum, cudaMemcpyHostToDevice);

	int * d_stride2d = 0;
	cudaMalloc((void**) &d_stride2d, sizeof(int) * localGridNum);
	cudaMemcpy(d_stride2d, stride2d, sizeof(int) * localGridNum, cudaMemcpyHostToDevice);

	cudaThreadSynchronize();
	dim3 blocksize(localGridNum, MAXTHREAD);

	mapGrid<<<blocksize,MAXTHREAD>>>(d_cudaGrid,d_intermediate,d_gridnum,d_stride1d,d_stride2d);
	cudaThreadSynchronize();

	cudaMemcpy(h_intermediate, d_intermediate, sizeof(int) * globalGridNum2, cudaMemcpyDeviceToHost);
	cudaThreadSynchronize();
	cutStopTimer(timer);
	cout << "timer =  " << cutGetTimerValue(timer) << endl;

	cudaFree(d_cudaGrid);
	cudaFree(d_intermediate);
	cudaFree(d_gridnum);
	cudaFree(d_stride1d);
	cudaFree(d_stride2d);
}

