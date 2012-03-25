#ifndef _LIST_
#define _LIST_
#include "Define.h"

template<typename T>
class List;

template<typename T>
class Node
{
	friend class List<T>;
	T * value;
	Node * next;
	Node(T*value):value(value),next(NULL){}
	~Node()
	{
		if(value)delete value;
	}
};

template<typename T>
class List
{
public:
	class Iterator
	{
	private:
		Node<T> * it;
	public:
		Iterator(Node<T>*it):it(it){}
		Iterator & operator =(Node<T>*it)
		{
			this->it = it;
			return *this;
		}
		bool operator ==(Iterator & it)const
		{
			return this->it->value == it.it->value;
		}

		bool operator !=(Iterator & it)const
		{
			return it.it != this->it;
		}

		Iterator & operator ++()
		{
			if(this->it!=NULL)it = it->next;
			return *this;
		}
		
		T * operator *()const
		{
			if(it!=NULL)
				return it->value;
			return NULL;
		}
	
	};
	
	Iterator begin()
	{
		return Iterator(head);
	}

	Iterator end()
	{
		Node<T>*end = tail;
		end = end->next;
		return Iterator(end);
	}

private:
	Node<T> * head;
	Node<T> * tail;
	int listLength;
public:
	List():head(NULL),tail(NULL),listLength(0){}

	~List()
	{
		Node<T>*p = head;
		for(;p!=NULL;)
		{
			Node<T>*temp = p;
			p = p->next;
			delete temp;
		}
	}

	void Append(T*value)
	{
		Node<T> * newnode = new Node<T>(value);
		if(head == NULL)
		{
			head = newnode;
			tail = newnode;
		}
		else
		{
			this->tail->next = newnode;
			this->tail = this->tail->next;
		}
		++listLength;
	}

	void PopFront()
	{
		Node<T>*p = head;
		head = head->next;
		delete p;
		--listLength;
	}

	T * Find(unsigned long id)
	{
		Node<T>*p = head;
		for(;p!=NULL;p=p->next)
		{
			if(p->value->GetId() == id)return p->value;
		}
		return NULL;
	}
	
	int Size(){return listLength;}
};

#endif