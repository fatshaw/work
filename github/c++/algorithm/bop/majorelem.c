#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 11
/*
 *	major problem:
 *	each major element count for more that one fourth of the whole set.
 *	
 *	numbers:input array of set
 *	n:number of element in the set
 *	result:array to put result in
 *
 *	@return 0:success. 1:fail
 * */
int major4(int * numbers,int n,int * result)
{
	if(n<=0|| numbers == NULL)return 1;
	int sets[4]={};
	int cnts[4]={};
	char isEqual = 0;
	for(int i = 0;i<N;i++){
		isEqual = 0;
		for(int j = 0;j<4;j++){
			if(sets[j] == numbers[i]){
				isEqual = 1;
				cnts[j]+=1;
				break;
			}
		}

		if(!isEqual){
			for(int j = 0;j<4;j++){
				if(cnts[j] == 0){
					sets[j] = numbers[i];
					cnts[j] = 1;
					break;
				}
			}
		}

		if(cnts[3]>0){
			for(int j = 0;j<4;j++){
				cnts[j]--;
				if(cnts[j] == 0)sets[j] = 0;
			}
		}	
	}
	result[0] = sets[0];
	result[1] = sets[1];
	result[2] = sets[2];
	return 0;
}

int main()
{
	int numbers[N]={2,2,1,1,3,3,2,3,1,4,4};
	int result[3];
	major4(numbers,N,result);
	for(int i = 0 ;i<3;++i){
		printf("%d\n",result[i]);
	}
}
