/************************************************************************/
/*skiing £º µÝ¹é°æ±¾                                                                     */
/************************************************************************/
#include "header.h"
static int sssa;
int total = 0;
int maximum = 0;
set<pair<int, int> > road;
stack<int>s;
stack<int>maxstack;
extern int ** weight ;
extern int r, c;
bool process(int i, int j,int last) {
	bool flag =false;
	if (road.find(make_pair(i, j)) == road.end()) {
		if(weight[i][j] < last)return false;
		road.insert(make_pair(i, j));
		last = weight[i][j];
		total +=last;
		s.push(last);
		if(i + 1 < r){
			flag = process(i+1,j,last);
		}
		if(i - 1 >=0){
			flag = process(i-1,j,last);
		}
		if(j + 1< c){
			flag = process(i,j+1,last);
		}
		if(j - 1 >=0){
			flag = process(i,j-1,last);
		}
		road.erase(make_pair(i, j));
		if(flag == false){
			if(total > maximum)
			{
				maximum = total;maxstack = s;s.pop();total -=last;return true;
			}
		}
		total -=last;s.pop();
		return true;
	} else {
		return false;
	}
}

int recursion()
{
	char * input = "d:/test";
	fstream fin(input);
#define cin fin
	int max = 0;
	int temp = 0;
	if (cin) {
		cin >> r >> c;
		weight = new int*[r];
		for(int i = 0 ;i<r; i++){
			weight[i] = new int[c];
		}
		for (int i = 0; i <r; ++i) {
			for (int j = 0; j <c; ++j) {
				cin>>weight[i][j];
			}
		}
		process(0, 0,0);
	}else{
		cout<<"file is wrong"<<endl;
		return 0;
	}

	cout<<maxstack.size()<<endl;
	for(int i = 0; i < r; ++i){
		delete[](weight[i]);
	}
	delete[]weight;
	return 0;
}