#include <stdio.h>

struct sc
{
	int a;
	int b;
};

struct sc cc[2]={{1,2},{2,1}};


extern void test(struct sc * psc);

int main()
{
	test(&cc[0]);
}
