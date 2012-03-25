#include <iostream>
#include <vector>
using namespace std;

struct tree_node
{
	tree_node * parent;
	tree_node * left;
	tree_node * right;
	int leftLevel;
	int rightLevel;
	int value;
};

class Tree
{
	private:
		tree_node * root;
		tree_node * cur;
		int size;
	public:
		~Tree();
		int depth();
		int depth(tree_node * node);
		Tree():root(NULL),cur(NULL),size(0){}
		void insert(int a);
		int findPair();
		int findPair(tree_node * node);
		int ldepth(tree_node * node);
		int ldepth_loop(tree_node * node);
		void insertBtree(int a);
		void insertBtree(tree_node *& node,int a);
		void display(int a);
		void pretraverse(tree_node * node);
		void posttraverse(tree_node * node);
		void middletraverse(tree_node * node);
		int findnode(int a);
		int findnode(tree_node * curnode,int a);
		int findfather(int a,int b);
		tree_node * findfather(tree_node * curnode,int a, int b);
		int findnode_trace(tree_node * curnode, int a, tree_node ** trace,int i);
		int findnode_trace(int a,tree_node ** trace);
		int findparent(int a, int b);
		int destroy(tree_node * curnode);
		void swapLeftRight(){swapLeftRight(root);}
		void swapLeftRight(tree_node*node);
		int getLeafNum(){getLeafNum(root);}
		int getLeafNum(tree_node * node);
		int isBST(){int pro = -100; return isBST(root,pro);}
		int isBST(tree_node * node,int & pro);	
		int findX(int x){int pro =-100; return findX(root,x,pro);}
		int findX(tree_node *node,int x,int &pro);
		int findMax(tree_node * node);
		void findMaxPath(tree_node * node,vector<tree_node*>&vNodes,vector<tree_node*>&maxNodes);
		void findMaxPath();
		int findMaxLength(){return findMaxLength(root);}
		int findMaxLength(tree_node*node);
		int countElemNum(tree_node * node);
		int countElemNum(){return countElemNum(root);}
		tree_node* buildLinkedList(tree_node * node,tree_node *&pro);
		void buildLinkedList(){
			tree_node * pro = NULL;
			tree_node * head = buildLinkedList(root,pro);
			for(;head!=NULL;head=head->right){
				cout<<head->value<<"->";
			}
			cout<<endl;
		}
};
/* build a double linked list.
 * each node's left pointer points to it predecessor. 
 * each node's right pointer points to its successor.
 * */
tree_node* Tree::buildLinkedList(tree_node*node,tree_node *& pro){
	if(node==NULL)return NULL;
	tree_node * head = NULL;
	if(node->left)
		head = buildLinkedList(node->left,pro);
	if(head == NULL)head = node;
	if(pro!=NULL){
		pro->right = node;
	}
	node->left = pro;
	pro = node;
	if(node->right)
		buildLinkedList(node->right,pro);
	return head;
}

int Tree::countElemNum(tree_node * node)
{
	if(node == NULL)return 0;
	return countElemNum(node->left) + countElemNum(node->right)+ 1;
}

/*recursive version.
 * calculate deep of each node many times. time = O(nlogn).*/
#if 0
int Tree::findMaxLength(tree_node * node){
	if(node == NULL)return 0;
	int l = depth(node->left);
	int r = depth(node->right);
	int cur = l+r;
	l = findMaxLength(node->left);
	r = findMaxLength(node->right);
	if(cur<l)cur=l;
	if(cur<r)cur=r;
	return cur;
}
#endif

/*dynamic programming version. record leftlevel and rightlevel in each node 
 * so that we do not need calculate deep of each node many times;*/
int Tree::findMaxLength(tree_node * node){
	int left = 0,right = 0,max = 0;
	if(node == NULL)return 0;
	if(node->left == NULL)node->leftLevel = 0;
	if(node->right == NULL)node->rightLevel = 0;
	if(node->left)
		left = findMaxLength(node->left);
	if(node->right)
		right = findMaxLength(node->right);

	if(node->left){
		int temp = node->left->leftLevel > node->left->rightLevel?node->left->leftLevel:node->left->rightLevel;
		node->leftLevel = temp + 1;	//weight is 1. if weight is not 1 we can change it to any value.
	}
	if(node->right)
	{
		int temp = node->right->leftLevel > node->right->rightLevel?node->right->leftLevel:node->right->rightLevel;
		node->rightLevel = temp + 1;
	}

	max = node->leftLevel + node->rightLevel;

	if(left < right)
		left = right;
	if(left > max)
		max = left;
	return max;
}

void Tree::findMaxPath(){
	vector<tree_node * >vNodes;
	vector<tree_node*>maxNodes;findMaxPath(root,vNodes,maxNodes);
	vector<tree_node*>::iterator it;
	for(it = maxNodes.begin();it!=maxNodes.end();++it){
		cout<<"->node"<<(*it)->value;
	}
	cout<<endl;
}

void Tree::findMaxPath(tree_node * node,vector<tree_node*>&vNodes,vector<tree_node*>&maxNodes)
{
	if(node == NULL){
		if(maxNodes.size()<vNodes.size()){
			maxNodes = vNodes;
		}
		return;
	}
	vNodes.push_back(node);
	findMaxPath(node->left,vNodes,maxNodes);
	findMaxPath(node->right,vNodes,maxNodes);
	vNodes.pop_back();
}

int Tree::findMax(tree_node * node)
{
	if(node == NULL)return -1;
	while(node->right)
	{
		node=node->right;
	}
	return node->value;
}

/*
 *    traverse binary search tree in terms of inorder traverse.
 *    we can get sorted list with all elements in the tree with this method.
 *    we should record the previous element of current element and if current element is greater 
 * 	  than x and previous element is less than x then we get the answer.
 * */
int Tree::findX(tree_node * node,int x,int &pro)
{
	int flag = 0;
	if(node)
	{
		flag = findX(node->left,x,pro);
		if(flag > 0)return flag;
		if(node->value >=x && pro<x)return pro;
		pro = node->value;
		flag = findX(node->right,x,pro);
	}
	return flag;
}

/*
 *    use inorder traverse to traverse the binary tree.
 *	  we can get sorted list with all the elements in the tree.
 *    during the traverse we need record the previous element we have visited and compare the current element with the prevous one
 *    if the current element is less than previous one than it is not a binary search tree.
 **/
int Tree::isBST(tree_node * node,int &pro)
{
	int flag = 1;
	if(node)
	{
		flag = isBST(node->left,pro);
		if(flag == 0 || node->value < pro)return 0;
		pro = node->value;
		flag  =isBST(node->right,pro);
	}
	return flag;
}

int Tree::getLeafNum(tree_node*node)
{
	if(node == NULL)return 0;
	if(node->left == NULL && node->right == NULL)return 1;
	return getLeafNum(node->left)+getLeafNum(node->right);
}

void Tree::swapLeftRight(tree_node * node)
{
	if(node == NULL)return;
	tree_node * tmp = node->left;
	node->left = node->right;
	node->right = tmp;
	swapLeftRight(node->left);
	swapLeftRight(node->right);
}

Tree::~Tree()
{
	destroy(root);
}

int Tree::destroy(tree_node * curnode)
{
	if(curnode == NULL)
	{
		return 0;
	}
	destroy(curnode->left);
	destroy(curnode->right);
	delete curnode;
}

int Tree::findfather(int a , int b)
{
	if(!findnode(root,a) || !findnode(root,b))return -1;
	tree_node * node = findfather(root,a,b);
	if(node != NULL)
		return node->value;
	else
		return -1;
}

tree_node * Tree::findfather(tree_node * curnode ,int a,int b)
{
	if(curnode == NULL)return NULL;
	if(findnode(curnode->left,a) && findnode(curnode->left,b))return findfather(curnode->left,a,b);
	else if(findnode(curnode->right,a) && findnode(curnode->right,b))return findfather(curnode->right,a,b);
	else return curnode;
}

int Tree::findnode(int a)
{
	return findnode(root,a);
}

int Tree::findnode(tree_node * curnode,int a)
{
	if(curnode == NULL)return 0;
	if(curnode->value == a)return 1;
	return ( findnode(curnode->left,a)||findnode(curnode->right,a) );
}

int Tree::findparent(int a, int b)
{
	if(!findnode(root,a) || !findnode(root,b))return -1;
	const int s = size;
	tree_node **  trace1  =  new tree_node*[size];
	tree_node **  trace2 =  new tree_node*[size];
	memset(trace1,0,sizeof(tree_node*)*s);
	memset(trace2,0,sizeof(tree_node*)*s);
	findnode_trace(a,trace1);
	findnode_trace(b,trace2);
	int i = 0;
	int commonparent = 0;
	while(trace1[i]&&trace2[i])
	{
		if(trace1[i++] == trace2[i++])continue;
		break;
	}	
	if(trace1[i] == trace2[i])
	{
		commonparent = a;
	}
	else if(trace1[i] == NULL)
	{
		commonparent = a;
	}
	else if (trace2[i] ==NULL)
	{
		commonparent = b;
	}
	else
	{
		commonparent = trace1[--i]->value;
	}
	cout<<"the nearest common parent value is"<<commonparent<<endl;
	delete [] trace1;
	delete [] trace2;
	return commonparent;
}

int Tree::findnode_trace(int a,tree_node ** trace)
{
	trace[0] = root;
	findnode_trace(root,a,trace,1);
}

int Tree::findnode_trace(tree_node * curnode, int a, tree_node ** trace,int i)
{
	if(curnode==NULL)return -1;
	if(curnode->value == a)return 0;
	trace[i] = curnode->left;
	if(findnode_trace(curnode->left,a,trace,++i) == -1)
	{
		trace[--i] = curnode->right;
		if(findnode_trace(curnode->right,a,trace,++i) == -1)
		{
			trace[--i] = NULL;
			return -1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

int Tree::depth()
{
	depth(root);
}

int Tree::depth(tree_node * node)
{
	if(node== NULL)return 0;
	int l = depth(node->left);
	int r = depth(node->right);
	return l>r?l+1:r+1;
}

void Tree::display(int i)
{
	switch(i)
	{
		case 0:
			pretraverse(root);
			break;
		case 1:
			middletraverse(root);
			break;
		case 2:
			posttraverse(root);
			break;
	}
}

void Tree::pretraverse(tree_node * node)
{
	if(node==NULL)return;
	cout<<node->value<<endl;
	pretraverse(node->left);
	pretraverse(node->right);
}

void Tree::posttraverse(tree_node * node)
{
	if(node==NULL)return;
	pretraverse(node->left);
	pretraverse(node->right);
	cout<<node->value<<endl;
}

void Tree::middletraverse(tree_node * node)
{
	if(node==NULL)return;
	pretraverse(node->left);
	cout<<node->value<<endl;
	pretraverse(node->right);
}

void Tree::insertBtree(int a)
{
	++size;
	insertBtree(root,a);
}

void Tree::insertBtree(tree_node *& node,int a )
{
	if(node == NULL){
		tree_node * newNode = new tree_node;
		newNode->value = a;
		newNode->parent = newNode->left = newNode->right = NULL;
		node = newNode;
		return;
	}
	if(node->value >= a)
	{
		insertBtree(node->left,a);
	}
	else if(node->value < a)
	{
		insertBtree(node->right,a);
	}
}

int Tree::ldepth(tree_node * node)
{
	if(node == NULL)return 0;
	if(node->left)
	{
		return ldepth(node->left) + 1;
	}
	else
	{
		return 0;
	}

}

int Tree::ldepth_loop(tree_node * node)
{
	int a = 0;
	while(node->left)
	{
		++a;
		node = node->left;
	}
	return a;
}

int Tree::findPair()
{
	return	findPair(root);
}

int Tree::findPair(tree_node * node)
{
	if(node == NULL)return 0;
	int pair = countElemNum(node->left);
	pair += findPair(node->left);
	pair += findPair(node->right);
	return pair;
}

void Tree::insert(int a)
{
	++size;
	tree_node * newNode = new tree_node;
	newNode->value = a;
	newNode->parent = newNode->left = newNode->right = NULL;
	if(root == NULL)
	{
		cur = root = newNode;
	}
	else
	{
		if(cur->left == NULL)
		{
			cur->left = newNode;
		}
		else if(cur->right == NULL)
		{	
			cur->right = newNode;
		}
		else
		{
			cur = cur->left;
			cur->left = newNode;
		}
	}	
}




int main()
{
	srand(time(NULL));
	int MAX = 10;
	int randn = 0;
	Tree tree;
int a[]={6,3,1,4,9,7,10,11};
	for(int i = 0;i< 8; i++){
		tree.insertBtree(a[i]);
	}
cout<<"node count = "<<tree.countElemNum()<<endl;
tree.buildLinkedList();
/*cout<<"isbst = "<<tree.isBST()<<endl;
cout<<"pair = "<<tree.findPair()<<endl;*/
/*	for(int j = 0;j<1;j++){
		Tree tree;
		for(int i = 0; i < MAX;i++)
		{
			randn = rand()%(2*MAX+1) - MAX; 
			tree.insertBtree(randn);
		}	
		cout<<"isbst = "<<tree.isBST()<<endl;
		cout<<"findx = "<<tree.findX(1)<<endl;	
		tree.display(0);
		tree.findMaxPath();
		cout<<tree.findMaxLength()<<endl;
	}
*/
	/*	cout<<"depth = "<<tree.depth()<<endl;
	 *	cout<<"pair = "<<tree.findPair()<<endl;
	 *
	 *	cout<<"common father = "<<tree.findfather(12,56)<<endl;
	 *	cout<<"common father = "<<tree.findparent(12,56)<<endl;*/

}

