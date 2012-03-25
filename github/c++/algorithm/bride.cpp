#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 1<<20
#define MAX_COUNTRY_NAME 101
#define MAX_COUNTRY_NUM 201
#define MAX_CHILD_COUNT MAX_COUNTRY_NUM

struct node
{
	int count;
	int cost;
	int visited;
	int dominatedCountryNum;
	int isChild;
	int children[MAX_CHILD_COUNT];
};

node tree[MAX_COUNTRY_NUM];
int dp[MAX_COUNTRY_NUM][MAX_COUNTRY_NUM];
char countries[MAX_COUNTRY_NUM+1][MAX_COUNTRY_NAME];

int findCountry(char * name)
{
	int i = 1;
	for(; i< MAX_COUNTRY_NUM;++i)
	{
		if(countries[i][0] == 0)break;
		if(strcmp(name,countries[i]) == 0)return i;
	}
	
	strcpy(countries[i],name);
	return i;
}

void init(int n)
{
	memset(tree,0,sizeof(tree));
	memset(dp,0x7f,sizeof(dp));
	memset(countries,0,sizeof(countries));
}

int calcuDonimatedNum(int nodeNo)
{
	if(tree[nodeNo].visited)return tree[nodeNo].dominatedCountryNum;
	tree[nodeNo].visited = 1;
	tree[nodeNo].dominatedCountryNum=1;
	for(int i = 0; i< tree[nodeNo].count;++i)
	{
		tree[nodeNo].dominatedCountryNum+=calcuDonimatedNum(tree[nodeNo].children[i]);
	}
	return tree[nodeNo].dominatedCountryNum;
}

void work(int nodeNo)
{
	if(tree[nodeNo].visited)return;
	tree[nodeNo].visited = 1;
	dp[nodeNo][0] = 0;
	for(int i = 0; i < tree[nodeNo].count;++i)work(tree[nodeNo].children[i]);
	for(int i = 0; i< tree[nodeNo].count;++i)
	{
		int son = tree[nodeNo].children[i];
		for(int j = tree[nodeNo].dominatedCountryNum-1;j>0;--j)
		{
			for(int k = 0;k<=tree[son].dominatedCountryNum;++k)
			if(dp[nodeNo][j] > (dp[nodeNo][j-k] + dp[son][k])){
				dp[nodeNo][j] = (dp[nodeNo][j-k] + dp[son][k]);
			}
		}
	}
	dp[nodeNo][tree[nodeNo].dominatedCountryNum] = tree[nodeNo].cost;
}

int main()
{
	FILE * file = fopen("./file","r");
	if(file == NULL)
	{
		file = stdin;
	}
	char countryname[MAX_COUNTRY_NAME];
	while(true)
	{
		char c = 0;
		int n = 0, m = 0,v = 0,donimatedNo = 0,countryNo = 0;
		fscanf(file,"%d%d",&n,&m);
		if(n < m || n <=0)break;
		init(n);
		for(int i = 0; i<n;i++)
		{
			fscanf(file,"%s%d",countryname,&v);
			countryNo = findCountry(countryname);
			tree[countryNo].cost = v;
			while(getc(file)==' ')
			{
				fscanf(file,"%s",countryname);
				donimatedNo=findCountry(countryname);
				tree[donimatedNo].isChild = 1;
				tree[countryNo].children[tree[countryNo].count++] = donimatedNo;
			}
		}
		
		for(int i = 1; i<=n;++i)
		{
				if(tree[i].isChild == 0)
				{
					tree[0].children[tree[0].count++]=i;	
				}
		}
		
		calcuDonimatedNum(0);
		for(int i = 0; i<=n;i++)
		{
			tree[i].visited = 0;
		}
		work(0);
		int asn = MAX;
		for(int i = m;i<=n;i++){
			if(dp[0][i] < asn)asn = dp[0][i];
		}
		printf("%d\n",asn);
	}
}

