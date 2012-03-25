#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define abc(s,m) (size_t)&(((s*)0)->m)
#define bcd(ptr,type,member) ({								\
		const typeof(((type*)0)->member) * __mptr = (ptr);	\
		(type*)((char*)__mptr - abc(type,member));})


struct test{
	int a;
	char b;
	long c;
};

void t2(){
	char buf[4];
	int x = 345;
	sprintf(buf,"%s","abcdef");
	buf[4]=0;
	buf[3]='k';
	printf("%d,%s\n",x,buf);
}

int main()
{
	t2();
	return 0;
	struct test t;
	t.a=-1;
	t.c =31;
	struct test * p ;
	p = bcd(&(t.c),struct test,c);
	printf("%ld\n",p->c);
	return 0;
}
