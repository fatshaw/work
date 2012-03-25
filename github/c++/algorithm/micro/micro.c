#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


int serial(int * a,int n)
{
	int i , j ;
	for(i = 0 ;i< n;++i){
		if(a[i] != 0)break;
	}

	for(j = n-1;j>=0;j--){
		if(a[j] != 0)break;
	}

	int start = a[i++];
	while(i<=j){
		++start;
		if(a[i]!=0 && a[i]!=start)break;
		++i;
	}
	if(i<=j)return -1;
	return 0;
}


void findSeq1(int n){
	int low = 1;
	int high = 2;
	int cnt = low+high;
	while(low < high && high <=n/2+1){
		if(cnt == n){
			for(int i =low;i<=high;i++)printf("%d,",i);
			printf("\n");
			cnt -= low;
			cnt += high+1;
			low ++;
			high++;
		}
		else if(cnt > n){
			cnt -= low;
			low+=1;
		}
		else{
			high+=1;
			cnt+=high;
		}
	}
}

void findSeq(int n){
	int high =(n+1)/2;
	int low = high - 1;
	int cnt = low+high;
	while(low >= 1){
		if(cnt == n){
			for(int i =low;i<=high;i++)printf("%d,",i);
			printf("\n");
			cnt = cnt - high + low - 1;
			--high;
			--low;
		}
		else if(cnt > n){
			low-=1;
			cnt =cnt - high + low;
			high-=1;
		}
		else{
			low-=1;
			cnt+=low;
		}
	}
}

void door(){
	int doors[101];
	for(int i = 1;i<=100;++i){
		doors[i] = 0;
	}

	for(int i = 1; i <=100;i++){//called number
		for(int j = 1;j<=100;++j){//door number
			if(j%i == 0){	//door number is multiple of the called number
				doors[j]=(doors[j]+1)%2;	//door state changes
			}
		}
	}

	int cnt = 0;
	for(int i = 1;i <=100;i++){
		if(doors[i]==0){
			cnt++;
		}else{
			printf("%d\n",i);
		}
	}
	printf("%d\n",cnt);
}

int main(){
/*	int a[]={0,0,1,0,3,0,5,0,9,0,0,0,0};
	printf("%d\n",serial(a,13));
	door();*/
	findSeq1(15);
}
