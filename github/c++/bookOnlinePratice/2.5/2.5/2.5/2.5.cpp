// 2.5.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

int funcx(int x)
{
	int count = 0 ;
	while(x)
	{
		count ++;
		x= x&(x-1);
	}
	return count;
}

class T
{
	int a;
public :
	T(){}
	T(const T&t)
	{

	}
};

void t()
{
	T tt;
	T gg(tt);

}

void t(T t)
{
	
}

T b()
{
	T t ;
	return t;
}


class Node
{
public :
	int n;
	Node * next ; 
	Node(int n)
	{
		this->n = n;
		this->next = NULL;
	}
};

class LinkedList
{
public :
	Node * head;
};

void add()
{
	LinkedList * l = new LinkedList();
	l->head = NULL;
	Node * cur = NULL;
	for(int i = 1 ;i < 10;i+=2)
	{
		Node * n = new Node(i);
		if(l->head == NULL)
		{
			l->head = cur = n;
		}
		else
		{
			cur->next = n;
			cur = n;
		}
	}

	LinkedList * t = new LinkedList();
	t->head = NULL;
	cur = NULL;
	for(int i = 1 ;i < 20;i+=3)
	{
		Node * n = new Node(i);
		if(t->head == NULL)
		{
			t->head = cur = n;
		}
		else
		{
			cur->next = n;
			cur = n;
		}
	}

	Node * n1 = l->head;
	Node * n2= t->head;
	Node * before = l->head;
	Node * temp = NULL;
	while(n2)
	{
		while(n1 && n1->n <= n2->n)
		{
			before = n1;
			n1= n1->next;
		}
		if(n1)
		{
			temp = n2;
			n2= n2->next;
			before->next = temp;
			temp->next = n1;
		}
		else
		{
			before->next = n2;
			break;
		}	
	}
	cout<<"t"<<endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
add();
	int neededNub = 0 ;
	int lastValue = 0;
	int a,b,c,d,e,f,y,x;
	int u[4] = {0,5,3,1};
	while(true)
	{
		cin>>a>>b>>c>>d>>e>>f;
		if(a==0 && b==0 && c==0 && d==0 && e==0 && f==0)
			break;

		neededNub = f + e + d + (c+3)/4;
		y = d*5 + u[c%4];
		if(b > y)
		{
			neededNub += (b - y + 8 )/9;
		}
		x = neededNub*36 - 36*f - 25*e - d*16 - c*9 - b *4;
		if(a > x)
		{
			neededNub += (a-x+35)/36;
		}
		cout<<neededNub<<endl;
	}
	system("pause");

	return 0;
}

