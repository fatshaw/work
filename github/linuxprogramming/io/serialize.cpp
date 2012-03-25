#include <iostream>
#include <fstream>
#include <string>

using namespace std;


class Test
{
   public:
    string a;
    int b;
    uint32_t c;
    uint16_t d;
    double e;
    bool f;
    char g;
    Test():a("test"),b(-1),c(10),d(2),e(12.1),f(true),g('a')
    {
        
    }

    friend ostream & operator << (ostream & out, const Test & t);
        
};
ostream & operator << (ostream & out, const Test & t){
        cout<<"a = "<<t.a<<endl;
        cout<<"b = "<<t.b<<endl;
        cout<<"c = "<<t.c<<endl;
        cout<<"d = "<<t.d<<endl;
        cout<<"e = "<<t.e<<endl;
        cout<<"f = "<<t.f<<endl;
        cout<<"g = "<<t.g<<endl;
        return out;
}

int main()
{
    Test t;
    fstream fout("./a",ios::binary|ios::out);
    fout.write((char*)&t,sizeof(t));
    cout<<"t"<<endl;
    cout<<t<<endl;
    cout<<"t1"<<endl;
    Test t1;
    fstream fin("./a",ios::binary|ios::in);
    fin.read((char*)&t1,sizeof(t1));
    cout<<t1<<endl;
    return 0 ;
}
