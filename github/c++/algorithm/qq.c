#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N 10
int top[N]={0,1,2,3,4,5,6,7,8,9};
int bottom[N];

int trybottom()
{
	int times = 0;
	int count = 0;
	char success = 1;
	while(1){
		times+=1;
		success = 1;
		for(int i = 0;i<N;i++){
			count = 0;
			for(int j = 0;j<N;j++){
				if(bottom[j] == i){
					count+=1;
				}
			}
			if(count != bottom[i]){
				bottom[i] = count;
				success = 0;
			}
		}
		if(success)break;
	}
	return times;
}

int main(){
	int times = trybottom();
	printf("%d\n",times);
	for(int i = 0;i<N;++i){
		printf("%d\n",bottom[i]);
	}
}
