#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

inline void swap(int & a,int& b){int t = a;a =b ;b = t;}

void shuffle(int * a,int n)
{
	for(int i = 0;i<n;i++)
	{
		swap(a[i],a[i+(rand()%n-i)]);
	}
}

#define BLACK 1280
#define WHITE 1000
int main()
{
	srand(time(NULL));
	int A[2];
	int B[2]={0,1};
	int bottles[BLACK+WHITE];
	for(int i = 0;i< BLACK;i++){
		bottles[i] = 0;
	}

	for(int i = BLACK;i<BLACK+WHITE;++i){
		bottles[i]=1;
	}
		
	shuffle(bottles,BLACK+WHITE);


	int whiteCntA=0;
	int whiteCntB=0;
	for(int i = 0;i<10000;i++){
		A[0]=bottles[rand()%(BLACK+WHITE)];
		A[1]=bottles[rand()%(BLACK+WHITE)];
		if(A[(rand()%2)]){
			++whiteCntA;
			if(A[rand()%2])++whiteCntA;
		}
		else{
			if(B[rand()%2])++whiteCntA;
		}
	}

	printf("whiteCntA=%d\n",whiteCntA);

	for(int i = 0 ; i< 10000; ++i){
		if(B[rand()%2])++whiteCntB;
		if(B[rand()%2])++whiteCntB;
	}
	
	printf("whiteCntB=%d\n",whiteCntB);

}
