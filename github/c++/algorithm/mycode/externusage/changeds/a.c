#include <stdio.h>

extern struct s
{
	char a[4];
}c;


extern void test();
int main()
{
	test();
	printf("%s",c.a);
	return 0;
}
