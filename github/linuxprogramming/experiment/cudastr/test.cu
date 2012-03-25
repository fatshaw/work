#ifndef TEST
#define TEST
#include <cuda.h>
#include <cuda_runtime.h>
#include <iostream>
#include <cutil.h>
#include <cutil_inline.h>
using namespace std;

//typedef __device__ char * (*func)(char *dest, const char *src, int size);

__device__ char *StrCpy(char *dest, const char *src, int size)
{
	char4 *dest1 = (char4*)dest;
	const char4 *src1 = (char4*)src;

	int numChar4=size>>2;
	int i=0;
	for(i=0;i<numChar4;i++)
	{
		dest1[i]=src1[i];
	}
	for(i=(numChar4<<2);i<size;i++)
	{
		dest[i]=src[i];
	}
	return dest; 
}


__device__ char *StrCpy1(char *dest, const char *src, int size)
{
	char4 *dest1 = (char4*)dest;
	const char4 *src1 = (char4*)src;

	for(int i = 0 ; i < size;++i){
		dest[i] = src[i];
	}
	return dest; 
}

__global__ void testStrcpy1(char * str1 , char * str2 , int size){
	StrCpy1(str1,str2,size);
}

__global__ void testStrcpy(char * str1 , char * str2 , int size){
	StrCpy(str1,str2,size);
}




void callCuda(){
	const int SIZE =(1<<24);
	unsigned int timer=0;
	unsigned int timer2 = 0;
	cutilCheckError(cutCreateTimer(&timer));
	cutilCheckError(cutCreateTimer(&timer2));
	char * str1 = (char *)malloc(SIZE);
	memset(str1,0,SIZE);
	char * d_str1 = NULL;
	cutilSafeCall(cudaMalloc((void**)&d_str1,SIZE));
	cutilSafeCall(cudaMemset(d_str1,0,SIZE));
	cutilSafeCall(cudaMemcpy(d_str1,str1,SIZE,cudaMemcpyHostToDevice));
	char * d_str2 = NULL;
	cutilSafeCall(cudaMalloc((void**)&d_str2,SIZE));
	cutilSafeCall(cudaMemset(d_str2,0,SIZE));
	dim3 block(256,256);
	cutilCheckError(cutStartTimer(timer));
	testStrcpy<<<1,1>>>(d_str2,d_str1,SIZE);
	cutilSafeCall(cudaThreadSynchronize());
	cutilCheckError(cutStopTimer(timer));

	cutilCheckError(cutStartTimer(timer2));
	testStrcpy1<<<1,1>>>(d_str2,d_str1,SIZE);
	cutilSafeCall(cudaThreadSynchronize());
	cutilCheckError(cutStopTimer(timer2));

	printf("timer 1 = %f\n",cutGetTimerValue(timer));/**/
	printf("timer 2 = %f",cutGetTimerValue(timer2));/**/

	cutilCheckError(cutDeleteTimer(timer));
	cutilCheckError(cutDeleteTimer(timer2));
	cutilSafeCall(cudaFree(d_str1));
	cutilSafeCall(cudaFree(d_str2));
	cudaThreadExit();

}
#endif

/*
 * Copyright 1993-2009 NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property and 
 * proprietary rights in and to this software and related documentation and 
 * any modifications thereto.  Any use, reproduction, disclosure, or distribution 
 * of this software and related documentation without an express license 
 * agreement from NVIDIA Corporation is strictly prohibited.
 * 
 *//*

#ifndef _CLOCK_KERNEL_H_
#define _CLOCK_KERNEL_H_

// This kernel computes a standard parallel reduction and evaluates the
// time it takes to do that for each block. The timing results are stored 
// in device memory.
__global__ static void timedReduction(const float * input, float * output, clock_t * timer)
{
    // __shared__ float shared[2 * blockDim.x];
    extern __shared__ float shared[];

    const int tid = threadIdx.x;
    const int bid = blockIdx.x;

    if (tid == 0) timer[bid] = clock();

    // Copy input.
    shared[tid] = input[tid];
    shared[tid + blockDim.x] = input[tid + blockDim.x];

    // Perform reduction to find minimum.
    for(int d = blockDim.x; d > 0; d /= 2)
    {
        __syncthreads();

        if (tid < d)
        {
            float f0 = shared[tid];
            float f1 = shared[tid + d];
            
            if (f1 < f0) {
                shared[tid] = f1;
            }
        }
    }

    // Write result.
    if (tid == 0) output[bid] = shared[0];

    __syncthreads();

    if (tid == 0) timer[bid+gridDim.x] = clock();
}

#endif // _CLOCK_KERNEL_H_
*/