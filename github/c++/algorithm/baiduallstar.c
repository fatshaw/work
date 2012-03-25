#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void clock(int * array,int n)
{
	int temp = array[n-1];
	for(int i = n-1;i>0 ;i--)
	{
		array[i]=array[i-1];
	}
	array[0]=temp;
}

int isSorted(int * array,int n)
{
	for(int i = 0;i<n;i++)
	{
		if(array[i]!=i+1)return 0;
	}
	return 1;
}

void swap(int &a,int &b)
{
	int t = a;
	a=b;
	b=t;
}

int canSort(int * array,int n,int a,int b,int level)
{
	if(level == n)return 0;
	if(isSorted(array,n))return 1;
	if(abs(array[a-1]-array[b-1]) == abs(a-b)){
		swap(array[a-1],array[b-1]);
		level = 0;
	}
	if(isSorted(array,n))return 1;
	clock(array,n);
	return canSort(array,n,a,b,level+1);
}

int reduce(int sum,int cnt,char * result)
{
	int tmp = cnt;
	for(int i = cnt;i>=2;i--){
		if(sum%i == 0 && tmp %i==0){
			sum /= i;
			tmp /= i;
			i = tmp;
		}
	}
	sprintf(result,"%d/%d",sum,tmp);
}

void avgmax(int *array,int m,int n,int ind,char * result)
{
	double * avg = new double[n];
	memset(avg,0,sizeof(double)*n);
	int sum = 0;
	double max = 0;
	for(int i = ind;i<n;i++)
	{
		sum+=array[i];
		avg[i]=sum/(i-ind+1);
		if(avg[i]>max&&i-ind+1>=m){
			max=avg[i];
			if(sum%(i-ind+1) != 0){
				reduce(sum,(i-ind+1),result);
			}
			else
			{
				sprintf(result,"%d",(int)max);
			}
		}
	}
	delete[]avg;
}

int qest3(int * array,int n,int m)
{
	int max = -1;
	int temp  = 0;
	char result[100];
	for(int i = 0;i<n-m;i++)
	{
		avgmax(array,m,n,i,result);
	}
	printf("%s\n",result);
}

void qest2(int * array,int n,int s,int x,int p)
{
	int * span = new int[n+x-1];
	for(int i = 0;i <n-1;i++){
		span[i]=array[i+1]-array[i];
	}
	int *span2 = new int[n+x-2];
	for(int i = 0; i< n-2;i++){
		span2[i] = span[i+1]-span[i];
	}
	int span3 = span2[1]-span2[0];

	int next = array[n-1];
	for(int i = 0;i<x;i++){
		span2[n-3+i+1] = span2[n-3+i]+span3;
		span[n-2+i+1]=span[n-2+i]+span2[n-3+i+1];
		next += span[n-2+i+1];
		printf("%d",next%p);
		if(i<x-1)printf(" ");
	}
	printf("\n");
	delete[]span;
	delete[]span2;
}


int main()
{
	int n,p,s,x;
	scanf("%d %d %d %d",&n,&p,&s,&x);
	int * array = new int[n];
	for(int i  =  0 ;i < n;i++)
	{
		scanf("%d",&array[i]);
	}
	qest2(array,n,s,x,p);
	delete[]array;
	return 0;
}

/*
int main()
{
	int n;
	int a,b;
	while(1){
		scanf("%d",&n);
		scanf("%d %d",&a,&b);
		if(n == 0||a == 0||b==0)break;
		int * array = new int[n];
		memset(array,0,sizeof(int)*n);
		for(int i = 0; i < n;i++)
		{
			scanf("%d",&array[i]);
		}
		int res = canSort(array,n,a,b,0);
		if(res){
			printf("Yes\n");
		}
		else{
			printf("No\n");
		}
		delete [] array;
	}
	return 0;

}*/

