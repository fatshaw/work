#include <stdio.h>
#include <string.h>
void swap(char * t ,char * p)
{
	int a = *t;
	*t=*p;
	*p=a;
}

void reverse(char * test,int l, int r)
{
	while(l<r)
	{
		swap(&test[l++],&test[r--]);
	}
	
}

void permutate(char * test, int ind,int len)
{
	if(ind >= len-1){
		printf("test=%s\n",test);
		return ;
	}
	int i,j;
	int flag = 0;
	for(i=ind;i<len;i++)
	{
		if(test[ind]==test[i]&&ind != i)continue;
		flag =0;
		for(j=ind+1;j<i;j++){
			if(test[j]==test[i]){
				flag = 1;
				break;
			}
		}
		if(flag)continue;
		swap(&test[ind],&test[i]);
		permutate(test,ind+1,len);
		swap(&test[ind],&test[i]);
	}
}

int ascent(const void * q,const void * p)
{
	char * tq = (char*)q;
	char * tp = (char*)tp;
	if(*tq > *tp)return 1;
	if(*tq == *tp)return 0;
	return -1;
}

int descent(const void * q,const void * p)
{
	char * tq = (char*)q;
	char * tp = (char*)tp;
	if(*tq > *tp)return 1;
	if(*tq == *tp)return 0;
	return -1;
}

int next_permutate(char * test,int len)
{
	int last = len-1;
	int i = last-1;
	while(i>=0)
	{
		if(test[i]<test[i+1])break;
		--i;
	}
	if(i<0)return -1;
	
	int j = last;
	for(j=last;j>i;--j)
	{
		if(test[j]>test[i])break;
	}
	swap(&test[j],&test[i]);
	reverse(test,i+1,last);
	return 0;
}

void print_permutate(char * test,int len)
{
	int i = 0;
	for(i = 0;i<len;++i){
		printf("%c,",test[i]);
	}
	printf("\n");
}

int main()
{
	char  test[] ="12234";
	int len =(int)strlen(test);
//	permutate(test,0,len);
//	permutate_loop(test,0,len);
	print_permutate(test,len);
	while(next_permutate(test,len)==0)
	{
		print_permutate(test,len);
	}
	return 0;	
}
