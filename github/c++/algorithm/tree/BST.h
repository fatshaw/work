#ifndef _BST_H
#define _BST_H
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "stack.h"

using namespace std;

#ifndef N
#define N 1000
#endif
extern "C" int rrandom(int low,int high);

#define NIL 0
#define MAX  1000000 

int ROOT=0;

template<typename T>
class Node;

template<typename T, class NodeType>
class BST;

template<typename T>
inline T max_3(T m1,T m2, T m3)
{
    T max = m1;
    if(m2>max){
        max = m2;
    }
    if(m3>max)
    {
        max = m3;
    }
    return max;
}

template<typename T>
inline T min_3(T m1,T m2, T m3)
{
    T min = m1;
    if(m2<min){
        min = m2;
    }
    if(m3<min)
    {
        min = m3;
    }
    return min;
}

struct AugmentDS
{
    int size;
    int reverse_order;
    int span;
    AugmentDS():size(0),reverse_order(0),span(MAX){}
};

template<typename T>
class Node
{
    public:
        int left;
        int right;
        int p;
        T data;
    public:
        Node():left(NIL),right(NIL),p(NIL),data(0){}
        virtual	void display(){cout<<"data="<<data;}
};

template<typename T>
class AugmentNode:public Node<T>,public AugmentDS
{
    friend class BST<T, AugmentNode<T> >;
    public:
    virtual void display(){
		cout<<"data = "<<this->data<<",size="<<size<<",reverse_order="<<reverse_order<<"span="<<span<<endl;
    }
};

template<typename T, class NodeType>
class BST
{
    protected:
        NodeType * nodes;
        int n;
    protected:
        int minimum(int x);
	int maximum(int x);
        int successor(int x);
	int predecessor(int x);
    public:
        BST(int number = N);
        virtual void insert(T & x,int node = ROOT);
        virtual void remove(T & x);
        int search(T & x,int node = ROOT);
        virtual void inorder_walk(int node = ROOT);
        virtual void inorder_walk_norecur(int node = ROOT);
};

template<typename T, class NodeType>
int BST<T,NodeType>::search(T&x,int node)
{
    if(node == NIL || nodes[node].data == x)return node;
    if(nodes[node].data >= x)
    {
        node = nodes[node].left;
    }
    else{
        node = nodes[node].right;
    }

}

template<typename T, class NodeType>
int BST<T,NodeType>::minimum(int x)
{
    while(nodes[x].left != NIL)
    {
        x = nodes[x].left;
    }
    return x;
}
/*
 * predecessor: if node has left subtree then the predecessor is the maximum node of the left subtree.
 * 		if node has no left subtree then the predecessor is the first right parent if there exists the one.
 * */
template<typename T, class NodeType>
int BST<T,NodeType>::predecessor(int x)
{
	if(nodes[x].left ! = NIL)return maximum(nodes[x].left);
	int p = nodes[x].p;
	while(nodes[p].left == x){
		x = p;
		p = nodes[p].p;
	}
	return p;
}

template<typename T, class NodeType>
int BST<T,NodeType>::maximum(int x)
{
	while(nodes[x].right){
		x = nodes[x].right;
	}
	return x;
}

/*
 * successor: 	if node has right subtree then the successor is the minimum node of the right subtree.
 * 		if node has no right subtree then the successor is the first left parent if there exists the one.
 * */
template<typename T, class NodeType>
int BST<T,NodeType>::successor(int x)
{
    if(nodes[x].right != NIL)return minimum(nodes[x].right);
    int p = nodes[x].p;
    while(nodes[p].right == x)
    {
        x=p;
        p = nodes[p].p;
    }
    return p;
}


template<typename T, class NodeType>
void BST<T,NodeType>::remove(T & x)
{
    int node = search(x);
    if(node == NIL)return;
    int p = nodes[node].p;
    if(nodes[node].left == NIL)
    {
        if(nodes[p].left == node)
        {
            nodes[p].left = nodes[node].right;
            nodes[node].left = nodes[node].right = nodes[node].p = NIL;
        }
    }
}


template<typename T, class NodeType>
BST<T,NodeType>::BST(int number):n(1){
    nodes = new NodeType[number+1];
}

template<typename T, class NodeType>
void BST<T,NodeType>::insert(T & x,int node) 
{
    int p = NIL;
    while(node != NIL)
    {
        p = node;
        if(nodes[node].data  >=x){
            node = nodes[node].left;
        }
        else{
            node = nodes[node].right;
        }
    }
    
    nodes[n].p=p;
    if(p!=NIL)
    {
        if(nodes[p].data >= x){
            nodes[p].left = n;
        }
        else{
            nodes[p].right = n;
        }
    }
    if(ROOT == NIL)ROOT=1;
    nodes[n].data = x;
    ++n;
}

template<typename T, class NodeType>
void BST<T,NodeType>::inorder_walk(int node)
{
    if(node == NIL)return;
    inorder_walk(nodes[node].left);
    nodes[node].display();
    inorder_walk(nodes[node].right);
}

template<typename T, class NodeType>
void BST<T,NodeType>::inorder_walk_norecur(int node)
{
    static Stack s(N);
    while(node != NIL || s.is_empty()!=1)
    {
        if(node !=NIL){
            s.push(node);
            node = nodes[node].left;
        }
        else{
            node = s.pop();
            nodes[node].display();
            node = nodes[node].right;
        }
    }
}

#endif
