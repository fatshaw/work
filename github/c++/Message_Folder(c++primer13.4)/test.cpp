#include <iostream>
#include "Folder.h"
#include "Message.h"
using namespace std;

class Base
{
private:
	int b;
	char c[10];
public:
	int a;
	Base():b(0),a(0){
	}
	Base(int a):a(a){}
	static void  stac(){};
	virtual int add(int a = 1 , int b =2){
		return a+b;
	}

	virtual ~Base(){
		cout<<"base "<<endl;
	}
};

class Derived_private : private Base{
protected:
	int d;
public:


	Derived_private(int a):Base(10),d(a){}

	Derived_private(const Derived_private & dp):Base(dp.a),d(dp.d)
	{

	}

	void test(){
		Base::a;
		a;
	}

	virtual ~Derived_private(){
		cout<<"derived private"<<endl;
	}
};

class Derived: public Base{
protected:
	int c;
public:
	Derived():Base(),c(0){}
	virtual int add(int a =3  ,int b =4 ){
		return Base::add(a,b) + c;
	}

	virtual ~Derived(){
		cout<<"Derivcd "<<endl;
	}

};

int main()
{

	Message * m = new Message("message1");

	Folder * f = new Folder("folder1");

	f->save(m);
	

	delete f;
	delete m;

	cout<<"well done"<<endl;
	
}