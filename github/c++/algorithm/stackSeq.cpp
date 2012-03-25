/*
 * push:	sequence of elements to be pushed
 * pop:		sequence of elements to be poped
 * n:		number of elements in the stack
 * @return	0:success
 *			1:failure
 * */
int isStackSeq(const int * push,const int * pop,int n){
	int i = 0, j = 0;
	while(i < n){
		if(push[i] == pop[0])break;
		++i;
	}
	
	int left = i,right = i;
	while(++j<n){
		if(left>0&&push[(left- 1)] == pop[j]){
			left =(left-1);
		}
		else if(right<n-1&&push[(right+1)] == pop[j]){
			right =( right + 1);
		}
		else{
			break;
		}
	}
	if(j<n)return -1;
	return 0;
}