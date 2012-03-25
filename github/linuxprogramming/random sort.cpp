#include <iostream>
#include<fstream>
#include<math.h>
#define MaxNum 21
using namespace std; 

int num = 0; //用于排列个数的计数
template <class T>
/**
* 允许重复的排列生成算法，p用于存储生成的排列
* list的长度为n，p的长度为m，递归的位置跟踪k
*/
void repPerm(FILE *file,T list[], int n, T p[], int m, int k){    
	if (k == m) {    //此时程序运行到最内层，可以输出此排列
		for (int i = 0; i < m; i ++){
			fprintf(file,"%c ",p[i]);
		}
		fprintf(file,"\n");
		num ++;
	}else{
		for (int i = 0; i < n; i ++){ //逐层确定元素，递归赋值
			p[k] = list[i]; //递归到的层进行赋值
			repPerm(file,list, n, p, m, k + 1);
		}
	}
} 
int main(){
	FILE *pFile;
	int m;
	int n;
    char list[MaxNum];     
    char p[MaxNum];
	pFile=fopen("a.txt","w");
	cout<<"输入你要排列的几位数(最大为10。例如6，就将输入的6位数进行按一定的位数进行全排列)：";
	cin>>n;
	cout<<"输入你要输入排列位数：(最大为21.例如7，就会出现7位数的全排列)";
	cin>>m;
	cout<<endl<<"(0到9的"<<n<<"个数的"<<m<<"位的全排列)排列结果已经存储在result.txt文件中\n";
	for(int i=0;i<n;i++){
		list[i]=(char)(rand()%10+48);
	}
    repPerm(pFile,list, n, p, m, 0);	
	fprintf(pFile,"total is %d \n",num);
	fclose(pFile);
    return 0; 

}