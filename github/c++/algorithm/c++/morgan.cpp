#include <iostream>
#include <vector>
#include <list>
#include <set>
using namespace std;


struct B{
	int a;
};

struct T:public B
{
	public:
		int a;
		T():a(1){cout<<"t"<<endl;}
		
		void * operator new(size_t sz){
			cout<<"new"<<endl;
			void * p = ::operator new(sz);
			return p;
		}

		void * operator new(size_t sz,void * t){
			return t;
		}
		~T(){cout<<"~T"<<endl;a = 11;}
		int getA(){return a;}

		int operator (){return a;}
};

int main()
{
	T * t = (T*)operator new(sizeof(T));
	T * tt = new(t)T;
	T * t1 = new(t)T;
	tt->~T();
	cout<<tt->getA()<<endl;
	cout<<t1->getA()<<endl;
	int b = *t;
	cout<<b<<endl;
}
