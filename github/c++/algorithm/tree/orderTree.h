#ifndef _ORDER_TREE_
#define _ORDER_TREE_
#include "BST.h"
template<typename T, class NodeType>
class OrderTree:public BST<T,NodeType>
{
	using BST<T,NodeType>::nodes;
	using BST<T,NodeType>::n;
	private:
	virtual void insertNode(T & x,int p, int & node)
	{
		nodes[n].data = x;
		nodes[n].size+=1;
		node = n;
		nodes[n].p = p;
		++n;
	}
	public:
	int reverse_order(int node = ROOT);
	virtual void insert(T&x,int node=ROOT);
	virtual void remove(T&x);
	T minSpan(int node = ROOT);
	T os_select(int i,int node = ROOT);
	int os_rank(int node);
};

template<typename T,class NodeType>
int OrderTree<T,NodeType>::os_rank(int node)
{
	if(node == NIL)return NIL;
	int r = nodes[nodes[node].left].size+1; 
	int p = nodes[node].p;
	while(p != NIL && nodes[p].right == node)
	{
		r+=nodes[nodes[p].left].size+1;
		node = p;
		p = nodes[node].p;
	}
	return r;	
}

template<typename T, class NodeType>
T OrderTree<T,NodeType>::os_select(int i,int node){
	if(node == NIL)return NIL;
	int left = nodes[nodes[node].left].size+1;
	if(left == i)return nodes[node].data;
	if(left < i)return os_select(i-left,nodes[node].right);
	return os_select(i,nodes[node].left);
}


template<typename T, class NodeType>
T OrderTree<T,NodeType>::minSpan(int node){
	static T max = nodes[node].span;
	if(node == NIL)return max;
	int lmax = minSpan(nodes[node].left);
	int rmax = minSpan(nodes[node].right);
	return min_3(lmax,rmax,nodes[node].span);
}

template<typename T, class NodeType>
void OrderTree<T,NodeType>::remove(T & x)
{
	int node = search(x);
	if(node == NIL)return;
	int p = nodes[node].p;
	if(nodes[node].left == NIL)
	{
		if(nodes[p].left == node)
		{
			nodes[p].left = nodes[node].right;
			nodes[node].left = nodes[node].right = nodes[node].p = nodes[node].data = nodes[node].size=NIL;
			while(p != NIL)
			{
				nodes[p].size--;
				p = nodes[p].p;
			}
		}
	}
}

template<typename T, class NodeType>
int OrderTree<T,NodeType>::reverse_order(int node){
	if(node != NIL){
		return nodes[node].reverse_order + reverse_order(nodes[node].left) + reverse_order(nodes[node].right);
	}
	return 0;
}


template<typename T, class NodeType>
void OrderTree<T,NodeType>::insert(T & x,int node) 
{
	if(n == 1){
        ROOT = 1;
		nodes[n].data =x;
		nodes[n].size = 1;
		nodes[n].p = NIL;
		++n;
		return;
	}
	if(node == NIL)return;
	nodes[node].size++;
	if(abs(nodes[node].data - x) < nodes[node].span){
		nodes[node].span = abs(nodes[node].data - x);
	}
	if(nodes[node].data < x){
		if(nodes[node].right!=NIL){
			insert(x,nodes[node].right);
		}
		else
		{
			insertNode(x,node,nodes[node].right);
		}
	}

	if(nodes[node].data >= x)
	{
		nodes[n].reverse_order+=nodes[nodes[node].right].size+1;
		if(nodes[node].left!=NIL){
			insert(x,nodes[node].left);
		}
		else
		{
			insertNode(x,node,nodes[node].left);
		}
	}
}

#endif
