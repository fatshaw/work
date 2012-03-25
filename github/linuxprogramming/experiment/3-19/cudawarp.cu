#include "cudaRelated.h"
#include <cutil.h>
#include <cutil_inline.h>
#include "cudaKernel.cu"
#include <iostream>
using namespace std;

int* cudaAlloc(int size) {
	int * address = 0;
	cudaHostAlloc((void**) &address, size, cudaHostAllocDefault);
	return address;
}

void cudaDealloc(int * address) {
	cudaFreeHost(address);
}

void cudaMap(cudaGrid * cudaArray, int localGridNum, int * gridnum, int * stride1d, int * stride2d, int * h_intermediate)//h_intermediate will be allocated outside.
{
	unsigned int timer = 0;
	cout << h_intermediate[0] << endl;
	cutCreateTimer(&timer);
	cutStartTimer(timer);
	int globalGridNum = stride1d[localGridNum - 1] + gridnum[localGridNum - 1];
	int globalGridNum2 = stride2d[localGridNum - 1] + gridnum[localGridNum - 1] * gridnum[localGridNum - 1];

	cudaGrid * d_cudaGrid = 0;
	cudaMalloc((void**) &d_cudaGrid, sizeof(cudaGrid) * globalGridNum);
	cudaMemcpy(d_cudaGrid, cudaArray, sizeof(cudaGrid) * globalGridNum, cudaMemcpyHostToDevice);
	int * d_intermediate = 0;
	cudaMalloc((void**) &d_intermediate, sizeof(int) * globalGridNum2);
	int * d_gridnum = 0;
	cudaMalloc((void**) &d_gridnum, sizeof(int) * localGridNum);
	cudaMemcpy(d_gridnum, &gridnum, sizeof(int), cudaMemcpyHostToDevice);
	int * d_stride1d = 0;
	cudaMalloc((void**) &d_stride1d, sizeof(int) * localGridNum);
	cudaMemcpy(d_stride1d, stride1d, sizeof(int) * localGridNum, cudaMemcpyHostToDevice);
	
	int * d_stride2d = 0;
	cudaMalloc((void**) &d_stride2d, sizeof(int) * localGridNum);
	cudaMemcpy(d_stride1d, stride2d, sizeof(int) * localGridNum, cudaMemcpyHostToDevice);
	cudaThreadSynchronize();
	dim3 blocksize(localGridNum, MAXTHREAD);

	mapGrid<<<blocksize,MAXTHREAD>>>(cudaArray,d_intermediate,d_gridnum,stride1d,stride2d);
	cudaThreadSynchronize();

	cudaMemcpy(h_intermediate, d_intermediate, sizeof(int) * globalGridNum2, cudaMemcpyDeviceToHost);
	cudaThreadSynchronize();
	cutStopTimer(timer);
	cout << "timer =  " << cutGetTimerValue(timer) << endl;

	cout << h_intermediate[0] << endl;
	//		for(int i = 0;i<localGridNum;++i){
	//			cout<<"localgrid "<<i<<endl;
	//			for(int j = 0;j< MAXTHREAD;++j){
	//				cout<<"line "<<j<<endl;
	//				for(int k = 0;k<MAXTHREAD;++k){
	//					cout<<h_intermediate[i * MAXTHREAD * MAXTHREAD + j * MAXTHREAD + k]<<", ";
	//				}
	//				cout<<endl;
	//			}
	//		}

	cudaFree(d_cudaGrid);
	cudaFree(d_intermediate);
	cudaFree(d_gridnum);
	cudaFree(d_stride1d);
	cudaFree(d_stride2d);
}

