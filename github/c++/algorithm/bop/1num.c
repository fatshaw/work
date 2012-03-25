#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

/*
 *	find how number 1 in the numbers from [1...n]
 * */
int numofone(int n)
{
	int k = 1;
	int cnt = 0;
	int tmp = n;
	int iEnd = 0;
	while(n> 0){
		cnt += n/10*k;
		iEnd = n%10;
		if(iEnd > 1){
			cnt += k;
		}
		else if(iEnd == 1){
			cnt += tmp%(k)+1;
		}
		k=k*10;
		n=n/10;
	}
	return cnt;
}

