#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct test
{
	int a:5;
int:2;
	int b:2;		//2 bit and other 30 bit will be extended by the highest bit in the 2 bit
	int c:4;
};

int main()
{
	struct test t;
	struct test * tp = &t;
	tp+=1;
	memcpy(&t,"EMC EXAMINATION",sizeof(t));
	printf("%d,%d,%x,%x,%d\n",t.a,t.b,t,*tp,t.c);
}
