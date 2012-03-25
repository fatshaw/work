#include <iostream>

using namespace std;

struct list_node
{
	list_node * next;
	int value;
	list_node * rnd;
};

class List
{
	private:
		list_node * head;
		list_node * cur;
		int cnt;
	public:
		List():head(NULL),cnt(0){}
		void insert(int a);
		list_node*reverse2(list_node*curnode);
		void reverse2();
		void reverse();
		void display();
		void merge(List & list);
		List intersector(List & list);
		void deleteByPointer(int a);
		bool checkCycle();
		list_node * getJoinPointer();
		list_node * lastKelement(int k);
		void insertCycle(int cnt);
		list_node * findCycleEntry();
		bool isIntersecting(const List & list);
		list_node * intersectNode(const List & list);
		void append(list_node * node){cur->next = node;cur = node;cnt++;}
		list_node* findNodeByValue(int a);
		void insertByPointer(int a, int b);
		void createRndList();
		List copyRndList();
		void printRnd();
		list_node * middleElement();
		void recursive_merge(const List &list);
		list_node* recursive_merge(list_node * a,list_node * b);
};

list_node * List::intersectNode(const List & list)
{
	if(!isIntersecting(list))return NULL;
	int a = cnt;
	int b = list.cnt;
	list_node * p;
	list_node * q;
	if(a<b){p=list.head;q = head;}
	else {p = head;	q=list.head;}
	a = abs(cnt - list.cnt);
	while(p && a)
	{
		p = p->next;
		a--;
	}
	while(p&&q)
	{
		if(q==p)break;
		p=p->next;
		q=q->next;
	}
	if(p && q && p == q)return p;
	return NULL;
}	

list_node * List::middleElement()
{
	list_node * p = head;
	list_node * q = head->next;
	while(q){
		p = p->next;
		if(q)q=q->next;
		if(q)q=q->next;
	}
}

void List::printRnd()
{
	list_node * p = head;
	while(p){
		printf("%d",p->value);
		if(p->rnd)printf(",%d",p->rnd->value);
		printf("\n");
		p=p->next;
	}
}

List List::copyRndList()
{
	list_node * newhead = NULL;
	list_node * newcur = NULL;
	list_node * cur = head;
	while(cur)
	{
		if(newhead == NULL){
			newhead = new list_node;
			newhead->value = cur->value;
			newhead->next = cur->next;
			cur->next = newhead;
		}
		else{
			newcur = new list_node;	
			newcur->value = cur->value;
			newcur->next = cur->next;
			cur->next = newcur;
		}
		cur=cur->next->next;
	}
	cur = head;
	while(cur){
		if(cur->rnd)
			cur->next->rnd=cur->rnd->next;
		else
			cur->next->rnd = NULL;
		cur = cur->next->next;
	}
	cur = head;
	list_node * dst = cur->next;
	while(cur)
	{
		list_node * temp = dst->next;
		cur->next = temp;
		if(temp)dst->next = temp->next;
		cur = cur->next;
		dst=dst->next;
	}
	List newList;
	newList.head = newhead;
	return newList;
}

void List::createRndList()
{
	srand(time(NULL));
	list_node * node = head;
	int rnd = 0;
	while(node)
	{
		rnd = rand()%(cnt+1);
		list_node * p = head;
		while(rnd){
			p = p->next;
			rnd--;
		}
		node->rnd = p;
		node = node->next;
	}
}

void createIntersectingList(List & list1,List & list2)
{
	for(int i= 1;i<5;i+=2){
		list1.insert(i);
	}

	for(int i = 2;i<6;i+=2){
		list2.insert(i);
	}

	for(int i = 7;i<15;i++){
		list_node * node = new list_node;
		node->value = i;
		node->next = NULL;
		list1.append(node);
		list2.append(node);	
	}
	/*list1.insertCycle(3);*/
}

bool List::isIntersecting(const List & list)
{
	bool flag = false;
	if(this->checkCycle())	
	{
		list_node * p = getJoinPointer();
		list_node * q = list.head;
		while(q){
			if(q == p){
				flag = true;
				break;
			}
			q=q->next;
		}
		flag = true;
	}
	else
	{
		list_node * p = head;
		list_node * q = list.head;
		while(p->next)p=p->next;
		while(q->next)q=q->next;
		if(p  == q)flag = true;
		else flag =false;
	}
	return flag;
}

/*
 *     x denotes distance between head and cycle entry
 *     z denotes distance between cycle entry and join pointer
 *     y denotes length of cycle 
 *     then we have x+z+1=y
 *	   so z = y-1-x. one pionter starts from the node after join pointer, and 
 *     another pointer starts from head, the join pointer where they meet is cycle entry
 * */
list_node * List::findCycleEntry()
{
	if(checkCycle()==false)return NULL;
	list_node * joinPointer = getJoinPointer();
	list_node * p = head;
	list_node * q = joinPointer->next;
	while(p!=q)
	{
		p=p->next;
		q=q->next;
	}
	return p;
}

list_node * List::lastKelement(int k){
	int t = k;
	list_node * p = head;
	while(p&&t){
		p=p->next;
		t--;
	}
	if(p == NULL && t >0)return NULL;
	list_node * q=head;
	while(q && p){
		p=p->next;
		q=q->next;
	}
	return q;
}

void List::insertCycle(int cnt){
	list_node * p = lastKelement(cnt);
	cur->next = p;
}

list_node * List::getJoinPointer()
{
	if(head == NULL || head->next == NULL)return NULL;
	list_node * one = head;
	list_node * two = head->next;
	while(one != two){
		one = one->next;
		if(two)two=two->next;
		else break;
		if(two)two=two->next;
		else break;
	}
	if(one == NULL || two == NULL)return NULL;
	return one;
}

bool List::checkCycle()
{
	return !(getJoinPointer() == NULL);
}

list_node * List::findNodeByValue(int a)
{
	list_node * node = head;
	while(node)
	{
		if(node->value == a)break;
		node = node->next;
	}
	return node;
}

void List::deleteByPointer(int a)
{
	list_node * node = findNodeByValue(a);	
	if(node)
	{
		if(node->next){
			node->value = node->next->value;	
			node->next = node->next->next;
		}
	}
}

/*	a is used to find node that new node is inserted before
 *	b is the value of new node
 *	*/
void List::insertByPointer(int a, int b)
{
	list_node * node = findNodeByValue(a);	
	if(node)
	{
		list_node * newnode = new list_node;
		newnode->next = node->next;
		node->next = newnode;
		newnode->value = node->value;		
		node->value = b;
	}
	cnt++;

}

List List::intersector(List & list)
{
	List l;
	list_node * h1= this->head;
	list_node * h2=list.head;
	while(h1&&h2)
	{
		while(h2&&h1->value > h2->value)h2=h2->next;
		if(h2 && h2->value == h1->value)l.insert(h2->value);
		h1=h1->next;
	}
	return l;
}

void List::merge(List & list)
{
	list_node * a = head;
	list_node * b = list.head;
	list_node * tempa= a;
	list_node * tempb = b;
	while(a&&b)
	{
		if(a->value <= b->value)
		{
			while(a&&b&&a->value <= b->value)
			{
				tempa = a;
				a = a->next;
			}
			tempa->next=b;
		}
		else
		{
			while(a&&b&&a->value > b->value)
			{
				tempb = b;
				b = b->next;
			}	
			tempb->next=a;
		}
	}
}

void List::recursive_merge(const List &list)
{
	recursive_merge(this->head,list.head);
}

list_node* List::recursive_merge(list_node * a,list_node * b)
{
	if(a == NULL)return b;
	if(b == NULL)return a;
	if(a->value <= b->value){
		a->next=recursive_merge(a->next,b);
		return a;
	}
	if(a->value > b->value)
	{
		b->next=recursive_merge(a,b->next);
		return b;
	}
}

void List::insert(int a)
{
	list_node * newNode = new list_node;
	newNode->next = NULL;
	newNode->value = a;		
	if(head == NULL)
	{
		cur = head = newNode;
	}
	else
	{
		cur->next= newNode;
		cur = newNode; 
	}
	cnt++;
}

void List::reverse()
{
	list_node * p = head;
	list_node * q = p->next;
	list_node * r = NULL;
	while(q){
		r = q->next;
		q->next = p;
		p = q;
		q = r;
	}
	head->next  = NULL;
	head = p;

}

list_node * List::reverse2(list_node * node){
	list_node * ln = NULL;
	if(node->next)
	{
		ln = reverse2(node->next);
	}
	else
	{
		head = node;
		return node;
	}
	ln->next = node;
	cur = node;
	node->next = NULL;
	return node;
}

void List::reverse2()
{
	reverse2(head);
}

void List::display()
{
	list_node * temp = head;
	while(temp)
	{
		cout<<"node = "<<temp->value<<",";
		temp = temp->next;
	}
	cout<<endl;
}

int main()
{
	List list;
	for(int i =3;i<10;i+=3){
		list.insert(i);
	}
	for(int i =4;i<10;i+=2){
		list.insert(i);
	}

	/*merge and recursive merge unit test*/
	/*list1.merge(list2);
	list1.recursive_merge(list2);
	list1.display();*/

	/*reverse unit test*/
	list.reverse2();	

	/*delete point unit test*/
	/*list.deleteByPointer(3);*/

	/*insertByPointer unit test*/
	/*list.insertByPointer(3,1);*/
	list.display();

	/*createRndList unit test*/
	/*list.createRndList();
	 * list.printRnd();
	 * printf("\n");
	 * List copyList = list.copyRndList();
	 * copyList.printRnd();
	 * */

	/*  cycle unit test*/
	list.insertCycle(3);
	printf("%d\n",list.checkCycle());
	list_node * p =list.findCycleEntry();
	printf("%d\n",p->value);

	/*lastkelement unit test*/
	/*list_node *  p =list.lastKelement(3);
	printf("%d\n",p->value);*/

	/*	Intersecting unit test*/
/*	List list1,list2;	
	createIntersectingList(list1,list2);
	printf("%d",list1.isIntersecting(list2));
	list_node * p = list1.intersectNode(list2);
	if(p)printf("%d\n",p->value);*/
	return 0;
}



