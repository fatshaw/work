#include <iostream>
using namespace std;

struct sb
{
	char a;
	int b;
};

class A:public sb
{
};


extern "C" void test(A a)
{
	cout<<"test"<<endl;
	cout<<a.a<<a.b<<endl;
}

