#ifndef _INTERVAL_TREE_H
#define _INTERVAL_TREE_H
#include "BST.h"

struct Interval
{
	int low;
	int high;
	Interval(int low = 0,int high = 0):low(low),high(high){}
	bool operator>=(const Interval & interval)
	{
		return this->low >=interval.low;
	}
	bool operator>(const Interval & interval)
	{
		return this->low > interval.low;
	}
	bool operator==(const Interval & interval){
		return this->low == interval.low;
	}
	friend ostream & operator<<(ostream & out,const Interval & interval)
	{
		out<<"low="<<interval.low<<",high="<<interval.high;
		return out;
	}
};


template<typename T>
class IntervalNode:public Node<T>
{
	public:
		int maxRight;
		IntervalNode(int maxRight=0):maxRight(maxRight){}
		virtual void display(){
			cout<<"data:"<<this->data<<",maxRight="<<maxRight<<endl;
		}
};

template<typename T, class NodeType>
class IntervalTree:public BST<T,NodeType>
{
	using BST<T,NodeType>::nodes;
	using BST<T,NodeType>::n;
private:
	int overlap(int i,int node){
		if(nodes[i].data.high<nodes[node].data.low || nodes[i].data.low > nodes[node].data.high)return 0;
		return 1;
	}
public:
	void insert(T & x,int node=ROOT);
	int interval_search(int i,int node=ROOT);
	int maxInterval(int node = ROOT);
};

template<typename T,class NodeType>
int IntervalTree<T,NodeType>::maxInterval(int node)
{
	static int interval = 0;
	static int max = 0;
	static int maxIntervals = 0;
	if(node == NIL)return 0;
	maxInterval(nodes[node].left);
	if(max == 0)
	{
		max = nodes[node].data.high;
		interval = 1;
	}
	else
	{
		if(max>=nodes[node].data.low)
		{
			++interval;
		}
		else
		{
			interval = 1;
		}
		max = max>nodes[node].data.high	?max:nodes[node].data.high;
	}
	if(interval > maxIntervals)maxIntervals = interval;
	maxInterval(nodes[node].right);
	return maxIntervals;
}

template<typename T, class NodeType>
int IntervalTree<T,NodeType>::interval_search(int i, int node)
{
	while(node != NIL && overlap(i,node)==0)
	{
		if(nodes[node].left != NIL && nodes[nodes[node].left].maxRight > nodes[i].data.low)
		{
			node = nodes[node].left;
		}
	}
	return node;
}

template<typename T, class NodeType>
void IntervalTree<T,NodeType>::insert(T & x,int node)
{
	int p = NIL;
	while(node != NIL)
	{
		if(nodes[node].maxRight < x.high)nodes[node].maxRight = x.high;
		p = node;
		if(nodes[node].data.low >= x.low)
		{
			node = nodes[node].left;
		}
		else
		{
			node = nodes[node].right;
		}
	}
	if(p!=NIL){
		if(nodes[p].data.low>=x.low){
			nodes[p].left = n;
		}
		else{
			nodes[p].right = n;
		}
	}
	if(ROOT == NIL)ROOT = 1;
	nodes[n].p = p;
	nodes[n].data = x;
	nodes[n].maxRight = x.high;
	++n;
}

#endif
