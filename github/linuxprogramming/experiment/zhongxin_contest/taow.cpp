/*
2010 - 5- 12
动态规划版本
*/

/*
* test.cpp
*
*  Created on: 2010-4-28
*      Author: fatshaw
*/

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <set>
#include <stack>
#include <algorithm>
using namespace std;

struct p{
	int x,y,w;
	p(int x, int y ,int w):x(x),y(y),w(w){}
	p(const p & p1 ){
		x = p1.x;
		y = p1.y;
		w = p1.w;
	}
};

struct cmp{
	bool operator()(const p & p1, const p & p2)const{
		return p1.w < p2.w;
	}
};

int ** weight = 0;
int ** value = 0;
int ** dir = 0;
vector<p>obj;
int r = 0,c = 0;
int cal(int i, int j){
	dir[i][j]  = -1;
	if( i - 1 >=0 ){
		if(weight[i-1][j] > weight[i][j]){
			if(value[i][j] < value[i-1][j] + 1){
				value[i][j] = value[ i -1 ][j] + 1;
				dir[i][j] = 0;
			}
		}

	}
	if( i + 1 < r){
		if(weight[i+1][j] > weight[i][j]){
			if(value[i][j] < value[i+1][j] + 1){
				value[i][j] = value[ i +1 ][j] + 1;
				dir[i][j] = 1;
			}
		}
	}
	if(j  -1 >=0){
		if(weight[i][j-1] > weight[i][j]){
			if(value[i][j] < value[i][j-1] + 1){
				value[i][j] = value[ i ][j-1] + 1;
				dir[i][j] = 2;
			}
		}
	}
	if(j + 1 <c ){
		if(weight[i][j+1] > weight[i][j]){
			if(value[i][j] < value[i][j+1] + 1){
				value[i][j] = value[ i  ][j+1] + 1;
				dir[i][j] = 3;
			}
		}
	}
	return value[i][j];
}


int main() {
	cout<<"input : ";
	char input[100];
	//char * input = "d:/test";
	cin>>input;
	fstream fin(input);
#define cin fin
	int max = 0;
	int temp = 0;
	if (cin) {
		cin >> r >> c;
		weight = new int*[r];
		value = new int*[r];
		dir = new int*[r];
		for(int i = 0 ;i<r; i++){
			weight[i] = new int[c];
			value[i] = new int[c];
			dir[i] =new int[c];
			memset(value[i],0,sizeof(int)*c);
		}
		for (int i = 0; i <r; ++i) {
			for (int j = 0; j <c; ++j) {
				cin>>weight[i][j];
				obj.push_back(p(i,j,weight[i][j]));
			}
		}
		sort(obj.begin(),obj.end(),cmp());
		temp = 0;
		for(int i = r * c -1; i >= 0 ;--i){
			temp = cal(obj.at(i).x,obj.at(i).y);
			if(temp > max)
				max = temp;
		}
		//cout<<max + 1<<endl;
		//process(cross, 0, 0,0);
	}else{
		cout<<"file is wrong"<<endl;
		system("pause");
		return 0;
	}
	int i =0, j =0;
	int direction = dir[0][0];
	cout<<weight[i][j]<<" ";
	while(direction != -1){
		switch(direction){
			case 0:
				i  = i -1;
				direction = dir[i][j];
				break;
			case 1:
				i = i+1;
				direction = dir[i][j];
				break;
			case 2:
				j = j -1;
				direction = dir[i][j];
				break;
			case 3:
				j = j +1;
				direction = dir[i][j];
				break;
		}
		cout<<weight[i][j]<<" ";
	}
	cout<<endl;
	for(int i = 0; i < r; ++i){
		delete[](weight[i]);
		delete[](value[i]);
		delete[](dir[i]);
	}
	delete[]weight;
	delete[]value;
	delete[]dir;
	system("pause");
	return 0;
}
