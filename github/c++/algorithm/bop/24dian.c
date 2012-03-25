#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <set>
#define CALU_NUMBER 5
#define NUMBER 4
#define SET_NUMBER 16
int number[NUMBER];

using namespace std;

set<int> sets[SET_NUMBER];

void f(int);
int check(){
	for(set<int>::iterator it = sets[SET_NUMBER-1].begin();it!=sets[SET_NUMBER-1].end();++it){
		if(*it==24)return 1;
	}
	return 0;
}

int game24(){
	for(int i = 0;i< NUMBER;++i){
		sets[(int)pow(2,i)].insert(number[i]);
	}

	for(int i =1;i<SET_NUMBER;i++){
		f(i);
	}

	return check();
}

/*
 * calculate four rule operation of the number in the set represented by the i
 * i:the binary representation of the subset of the whole set.
 * if the bit in the i is 1 then the number in the set is selected otherwise the number is not.
 * */
void f(int i){
	static int cnt = 0;
	if(sets[i].size()!=0){
		return ;
	}

	for(int x = 1;x<i;x++){
		if((x&i)==x){	//x is the subset of i
			f(x);
			f(i-x);
			for(set<int>::iterator it = sets[x].begin();it!=sets[x].end();++it)
			{
				for(set<int>::iterator it2 = sets[i-x].begin();it2!=sets[i-x].end();++it2)
				{
					int a=*it;
					int b=*it2;
					if(a+b==24&&i == 15)cout<<"a="<<a<<",b="<<b<<",i="<<i<<",x="<<x<<endl;
					sets[i].insert(a+b);
					if(a>b){
						sets[i].insert(a-b);
						if(a-b==24&&i==15)cout<<"a="<<a<<",b="<<b<<",i="<<i<<",x="<<x<<endl;
					}
					sets[i].insert(a*b);
					if(a*b==24&&i==15){cout<<"a="<<a<<",b="<<b<<",i="<<i<<",x="<<x<<endl;}
					if(b>a){
					sets[i].insert(b-a);
					if(b-a==24&&i==15)cout<<"a="<<a<<",b="<<b<<",i="<<i<<",x="<<x<<endl;
					}
					if(a!=0&&((b%a)==0))
					{
						if(b/a == 24&&i==15)cout<<"a="<<a<<",b="<<b<<",i="<<i<<",x="<<x<<endl;
						sets[i].insert(b/a);
					}
					if(b!=0&&((a%b)==0))
					{
						if(a/b == 24&&i==15)cout<<"a="<<a<<",b="<<b<<",i="<<i<<",x="<<x<<endl;
						sets[i].insert(a/b);
					}
				}
			}
		}
	}
}

int main()
{
	number[0] = 2;
	number[1] = 7;
	number[2] = 1;
	number[3] = 3;
	cout<<game24()<<endl;
}

