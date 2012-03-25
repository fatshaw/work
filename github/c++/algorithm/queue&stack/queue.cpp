#include <iostream>
using namespace std;

template<typename T>
class Queue
{
private:
	T  queue[8];
	int maxsize;
	int front;
	int last;
public:
	Queue():maxsize(8),front(0),last(0){}
	int enqueue(T & value);
	T  dequeue();
	void clear(){memset(queue,0,sizeof(T)*8);}
};

template<typename T>
int Queue<T>::enqueue(T & value)
{
	if((last+1%maxsize)==front)return -1;
	last = (last+1)%maxsize;
	queue[last]=value;
	return 0;
}

template<typename T>
T Queue<T>::dequeue()
{
	if(front == last)return -1;
	front = (front + 1) % maxsize;
	T tmp = queue[front];
	return tmp;
	
}


int main()
{
	int a[]={1,2,3,4,5,6,7,8};
	Queue<int> q;
	for(int i = 0 ; i < 8;i++){
		q.enqueue(a[i]);
		q.dequeue();
	}
	q.clear();
	for(int i = 0 ; i < 8;i++){
		q.enqueue(a[i]);
	}
	for(int i = 0 ; i < 8;i++){
		cout<<q.dequeue()<<",";
	}
	return 0;
}
