/*$Id: map.cu 720 2009-11-10 10:13:52Z wenbinor $*/
/**
 *This is the source code for Mars, a MapReduce framework on graphics
 *processors.
 *Developers: Wenbin Fang (HKUST), Bingsheng He (Microsoft Research Asia)
 *Naga K. Govindaraju (Microsoft Corp.), Qiong Luo (HKUST), Tuyong Wang (Sina.com).
 *If you have any question on the code, please contact us at 
 *           wenbin@cse.ust.hk or savenhe@microsoft.com
 *
 *The license is a free non-exclusive, non-transferable license to reproduce, 
 *use, modify and display the source code version of the Software, with or 
 *without modifications solely for non-commercial research, educational or 
 *evaluation purposes. The license does not entitle Licensee to technical support, 
 *telephone assistance, enhancements or updates to the Software. All rights, title 
 *to and ownership interest in Mars, including all intellectual property rights 
 *therein shall remain in HKUST.
 */

#ifndef __MAP_CU__
#define __MAP_CU__

#include "MarsInc.h"
#include "global.h"

__device__ void MAP_COUNT_FUNC//(void *key, void *val, size_t keySize, size_t valSize)
{
	SM_KEY_T* pKey = (SM_KEY_T*)key;	//input key
	SM_VAL_T* pVal = (SM_VAL_T*)val;	//input value

	int bufOffset = pVal->linebuf_offset;	//offset of all the key before the current key
	int bufSize = pVal->linebuf_size;		//the current key length
	char* buf = pKey->ptrFile + bufOffset;	//get the head address of the current key with offset.

	char* keyword =  pKey->ptrKeyword; // a temp pointer to the keyword
	int keywordSize = pVal->keyword_size;// key word size.

	int cur = 0;
	char* p = buf;
	char* start = buf;

	while(1)
	{
		for (; *p != '\n' && *p != '\t' && *p != ' ' && *p != '\0'; ++p, ++cur);
		*p = '\0';
		++p;
		int wordSize = (int)(p - start);

		if (cur >= bufSize) break;
		char* k = keyword;
		char* s = start;
		// because we have already had the wordsize so it is a good idea to compare wordsize first
		if (wordSize == keywordSize)  
		{
			for (; *s == *k && *k != '\0'; s++, k++);
			if (*s == *k) EMIT_INTER_COUNT_FUNC(sizeof(int), sizeof(int));
			//here we just care about key count,so pair is in the form of <int,int>
		}

		start = p;
		bufOffset += wordSize;
	}
}

__device__ void MAP_FUNC//(void *key, void val, size_t keySize, size_t valSize)
{
	SM_KEY_T* pKey = (SM_KEY_T*)key;//current key
	SM_VAL_T* pVal = (SM_VAL_T*)val;//current value

	int bufOffset = pVal->linebuf_offset;//offset of key
	int bufSize = pVal->linebuf_size;//key length
	char* buf = pKey->ptrFile + bufOffset;//get buf through offset and start address of input file

	char* keyword =  pKey->ptrKeyword;//a temp pointer to the keywork
	int keywordSize = pVal->keyword_size;//key word size.
	int cur = 0;
	char* p = buf;
	char* start = buf;

	while(1)
	{
		for (; *p != '\n' && *p != '\t' && *p != ' ' && *p != '\0'; ++p, ++cur);
		*p = '\0';
		++p;
		int wordSize = (int)(p - start);
		int wordOffset = bufOffset;

		if (cur >= bufSize) break;

		char* k = keyword;
		char* s = start;
		
		if (wordSize == keywordSize) 
		{
			for (; *s == *k && *k != '\0'; s++, k++);
			if (*s == *k) 
			{
				//above the same with map_count
				int* o_offset = (int*)GET_OUTPUT_BUF(0);//get output buf for offset through thread id 
				int* o_size = (int*)GET_OUTPUT_BUF(sizeof(int));//get output buf for size through thread id
				*o_offset = wordOffset;
				*o_size = wordSize;
				EMIT_INTERMEDIATE_FUNC(o_offset, o_size, sizeof(int), sizeof(int));
				//intermediate pair <word,count>.eg. <"abc",1>
			}
		}

		start = p;
		bufOffset += wordSize;
	}
}
#endif //__MAP_CU__
