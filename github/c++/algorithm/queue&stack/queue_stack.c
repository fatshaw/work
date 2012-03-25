#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "stack.h"

class Queue_stack{
public:
	Stack s1;
	Stack s2;

	Queue_stack(int n = N):s1(n),s2(n){}

	void push_back(int x)
	{
		s1.push(x);
	}

	int pop()
	{
		if(s2.is_empty()){
			while(s1.is_empty()== 0){
				s2.push(s1.pop());
			}
		}
		if(s2.is_empty()== 0)return s2.pop();
	}
	
	int is_empty(){
		return (s1.is_empty()&&s2.is_empty());
	}

};

class Stack_queue{
public:
	Queue_stack q1,q2;
	void push_back(int x)
	{
		q1.push_back(x);
	}

	int pop()
	{
		int temp = 0;
		if(q1.is_empty()){
			if(q2.is_empty() == 0){
				while(1){
					temp = q2.pop();
					if(q2.is_empty() == 0){
						q1.push_back(temp);
					}else{
						break;
					}
				}
				return temp;
			}
		}	
		else{
			while(1){
				temp = q1.pop();
				if(q1.is_empty() == 0){
					q2.push_back(temp);
				}else{
					break;
				}
			}
			return temp;
		}
	}
};

int main()
{
	printf("test for queue_stack\n");
	Queue_stack q;
	for(int i = 0 ; i < 20;++i){
		q.push_back(i);
	}

	for(int i = 0 ;i < 10;++i){
		printf("%d\n",q.pop());
	}

	for(int i = 0; i< 25;++i)
	{
		q.push_back(i);
	}

	for(int i = 0 ; i < 35;++i)
	{
		printf("%d\n",q.pop());
	}
	
	printf("test for stack_queue\n");
	Stack_queue s;
	for(int i = 0 ; i < N;++i){
		s.push_back(i);
	}
	
	for(int i = 0 ; i < N;++i)
	{
		printf("%d\n",s.pop());
	}
	return 0;
}
