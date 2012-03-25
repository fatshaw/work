#include <iostream>
using namespace std;

class Base1
{
        public:
                virtual void test(){cout<<"base1 test"<<endl;}
};

class Base2
{
        public:
                void test(){cout<<"base2 test"<<endl;}
};


class Test:public Base1,public Base2
{
        public:
                void test(){cout<<"Test test"<<endl;}

};

int main()
{
        Base2 * b = new Test;
        b->test();
}