#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <time.h>
using namespace std;

#define N 100

template<typename T>
class Deque
{
private:
	T * array;
	int last; //first empty element
	int head;//first non-empty element
	int n;
public:
	Deque(int n = N):last(0),head(0),n(n){array = new int[n];memset(array,0,sizeof(T)*N);}
	void push_back(T x){
		if((last+1)% n != head){
			array[last]=x;
			last = (last + 1 )%n;
		}
       	}
	void push_front(T x){
		if((last+1)%n != head)
		{
			head = (head + n - 1 )%n;
			array[head] = x;
		}
	}
	T pop_back(){
		if(last != head){
			last = (last + n -1 )% n;
			return array[last];
		}else{
			throw exception();
		}
	}
	T pop_front(){
		if(last != head){
			T temp = array[head];
			head = (head + 1 )% n;
			return temp;
		}
		else{
			throw exception();
		}
	}
};

extern "C" double drandom();

int main()
{
	srand(time(0));
	Deque<int> deque;
	int d[N];
	memset(d,0,sizeof(int)*N);
	for(int i = 0; i <N-1;++i){
		if(drandom()<0.5){
			deque.push_back(i);
		}
		else{
			deque.push_front(i);
		}
	}
	for(int i = 0; i< N-1;++i)
	{
		int e = deque.pop_front();
		if(d[e]== 1)throw exception();
		d[e]=1;
	}
	return 0;
}
