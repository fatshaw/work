/*
 * =====================================================================================
 *
 *       Filename:  oddeven.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/26/2011 12:27:51 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#include <stdio.h>
void change(int * a, int n){
	int i = 0;
	int j = n-1;
	while(i<j){
		while(i<j&&a[i]%2==1)i++;
		while(i<j&&a[j]%2==0)j--;
		if(i<j){
			int t = a[i];
			a[i]=a[j];
			a[j]=t;
		}
	}
}
int main()
{
	int n = 9;
	int a[]={1,2,3,4,5,6,7,8,11};
	change(a,n);
	for(int i =0 ; i < n;i++){
		printf("%d,",a[i]);
	}
}
