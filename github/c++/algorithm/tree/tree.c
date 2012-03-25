#include <stdlib.h>
#include <string.h>
#include <stdio.h>
struct node
{
	int left;
	int right;
	int value;
};

inline int max(int a,int b){
	return a>b?a:b;
}

#define N 100
struct node trees[N];

int depth(int nodeId){
	if(nodeId == 0){
		return 0;
	}
	int l = depth(trees[nodeId].left);
	int r = depth(trees[nodeId].right);
	return l>r?l+1:r+1;
}

/*	find two nodes with max distance in the tree
 * */
int findMax(int nodeId){
	if(nodeId == 0) return 0;
	int left = depth(trees[nodeId].left);
	int right = depth(trees[nodeId].right);
	int distance = left + right;
	int l = findMax(trees[nodeId].left);
	int r = findMax(trees[nodeId].right);
	if(distance < l)distance = l;
	if(distance < r)distance = r;
	return distance;
}
/*
build perfect binary search tree.
*/
void build(int node,int l, int r){
	if(l>r)return;
	int m = (l + r) / 2;
	trees[node].value = m;
	if(l<=m-1)
		trees[node].left = 2*node; 
	if(m+1<=r)
		trees[node].right = 2*node+1;
	build(node*2,l,m-1);
	build(node*2+1,m+1,r);
}

void inorder(int node){
	if(node == 0)return;
	inorder(trees[node].left);
	printf("%d\n",trees[node].value);
	inorder(trees[node].right);
}

/*
	calculate max dependent set of tree with node no as the root.
	the parameter select will determine whether the node no is included in the max dependent set.
	
	no: index of the node no in the trees
	select: 0=the node is included in the max dependent set.
			1=the node is not included in the max dependent set.
	
	@return size of max dependent set of the tree with no 
	as the root given the situation that the node is selected or not.
*/
#define INCLUDE 1
#define EXCLUDE 0
int maxDepSet(const int no,int select)
{
	if(no == 0)return 0;
	static int dp[N][2]={};
	if(dp[no][select]!=0)return dp[no][select];
	int l,r,sum = 0;
	if(select == EXCLUDE)
	{
		l = max(maxDepSet(trees[no].left,INCLUDE),maxDepSet(trees[no].left,EXCLUDE));
		r = max(maxDepSet(trees[no].right,INCLUDE),maxDepSet(trees[no].right,EXCLUDE));
		sum = l + r;
	}
	else
	{
		l = maxDepSet(trees[no].left,EXCLUDE);
		r = maxDepSet(trees[no].right,EXCLUDE);
		sum = l+ r+ 1;
	}
	dp[no][select] = sum;
	return sum;
}

int maxDepSet()
{
	return max(maxDepSet(1,0),maxDepSet(1,1));
}

int main()
{
	memset(trees,0,sizeof(trees));
	build(1,1,15);
	//	inorder(1);	
//	printf("%d\n",findMax(1));
	printf("%d\n",maxDepSet());
	return 0;
}
