#ifndef STACK_H
#define STACK_H
#include <string.h>
#include <stdlib.h>
#include <exception>
#include <time.h>
#include <stdio.h>
using namespace std;

#define N 100
#define RIGHT 0
#define LEFT 1

class overflowException:public exception
{
public:
	const char * what() const throw() 
	{
		return "overflowException\n";
	}
};


class underflowException:public exception
{
public:
	const char * what() const throw() 
	{
		return "underflowexception\n";
	}
};

class Stack
{
public:
	Stack(){}
	Stack(int n):n(n),pos(-1){a = new int[n];}
	virtual void push(int x)
	{
		if(a && pos < n-1){
			a[++pos]=x;
		}
		else{
			throw overflowException();
		}
	}	

	virtual int pop()
	{
		if(a && pos>=0){
			return a[pos--];
		}
		else{
			throw underflowException();
		}
	}

	int is_empty(){
		return pos == -1;
	}

protected:
	int * a;
	int n;	
	int pos;
};

class doubleStack
{
public:
	doubleStack(int n):n(n),lpos(-1),rpos(n){a = new int[n];}
	void push(int x, int dir)
	{
		if(dir == RIGHT)
		{
			if(lpos+1<rpos && a)
			{
				a[--rpos] = x;
			}
			else{
				throw overflowException();
			}
		}
		else if(dir = LEFT)
		{
			if(lpos+1<rpos && a){
				a[++lpos] =x;
			}
		}
	}

	int pop(int dir)
	{
		if(dir == RIGHT)
		{
			if(a && rpos<n){
				return a[rpos++];
			}else{
				throw underflowException();
			}
		}
		else if(dir == LEFT)
		{
			if(a && lpos >=0){
				return a[lpos--];
			}	
			else{
				throw underflowException();
			}
		}
	}
protected:
	int lpos;
	int rpos;
	int n ;
	int * a;
};


//generate double random number between 0 and 1, excluding 1. [0,1)
extern "C" double drandom();

//generate random number between low and high, excluding high. [low,high)
extern "C" int rrandom(int low,int high);
#endif
