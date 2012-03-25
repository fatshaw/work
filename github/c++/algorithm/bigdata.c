#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*
 *	return sum of 1 to n
 *	max number add
 * */
void func(int n){
	int cnt = 1;
	char t[n+1];
	for(int i = 0;i<n;i++){
		t[i] = '0';
	}
	t[n] = 0;

	int index = 0;
	char hasinc = 0;
	for(;;){
		index  = n - 1;
		hasinc =  0;
		do{
			if(t[index]=='9'){
				t[index] = '0';
				hasinc = 1;
				index -= 1;
			}
			else{
				t[index]+=1;
				hasinc = 0;
			}
		}while(hasinc && index >= 0);
		if(index<0)break;
		printf("%s\n",t);
	}
}


int main()
{
	int n = 4;
	func(n);
}
