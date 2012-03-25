#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(char * a, char * b)
{
	char t = *a;
	*a = *b;
	*b = t;
}

void change(char * str,int n)
{
	char * p = str + 1;
	char * q = str + n;
	char * r = q;
	while(q<str+2*n-1){
		swap(p++,q++);
		if(p<r){
			swap(p,r);
		}
		++p;
	}
}


inline int max(int a,int b){return a>b?a:b;}
/*
	max sum of non neighbouring elements in the array
*/
int sum(int * a,int n)
{
	int dp[n+1];
	memset(dp,0,sizeof(dp))	;
	dp[1]=a[0];
	for(int i = 1; i< n;++i)
	{
		dp[i+1]=max(dp[i],dp[i-1]+a[i]);
		if(dp[i+1]==dp[i])a[i]=0;
	}
	return dp[n];
}


int search(int *a,int l,int r,int x,int big)
{
	int m;
	while(l<=r){
		m = ( l+r )>> 1;
		if(a[m] == x)return m;
		if(a[m]<x){
			if(big)r=m-1;
			else l = m+1;
		}
		else {
			if(big)l=m+1;
			else{
				r = m-1;
			}
		}
	}
}

int find(int * a,int n,int x)
{
	bool big  = a[0]>a[1];
	int i = 0;
	for(i = 1;i<n-1;++i){
		if(a[i]>a[i+1] == big)continue;
		break;
	}
	int p = 0,q=i+1,max,min;
	if(big){max = a[p];min = a[q-1];}
	else{max=a[q-1];min=a[p];}

	if((x>=min&&x<=max)){
		return search(a,p,q-1,x,big);
	}

	if(big){max = a[q];min = a[n-1];}
	else{max=a[n-1];min=a[q];}
	
	if((x>=min&&x<=max)){
		return search(a,q,n-1,x,big);
	}
	return -1;
}

int main()
{
	char a[]="12345678910";
	change(a,5);
	printf("%s\n",a);	
	/*
	 * find element in the circular sorted list
	int a[]={4,3,2,1,7,6,5};
	int m = find(a,7,3);
	printf("%d\n",m);
	*/

	/*
	 * get max sum of non neighboring elements.
	int a[]={3,2,4,1,3,5,2};
	printf("%d\n",sum(a,7));
	int i = 6;
	while(i>=0){
		if(a[i]==0){--i;continue;}
		printf("%d\n",a[i]);
		i-=2;
	}*/
}
