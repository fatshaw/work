#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace  std;

#define WIDTH 10

int main()
{	
	srand((unsigned int)time(0));
	int row = 0;
	int col = 0;
	int board[WIDTH+2][WIDTH+2];
	memset(board,0,sizeof(int)*(WIDTH+2)*(WIDTH+2));
	for(int i = 1; i<=WIDTH ;++i){
		do{
			row = rand()%WIDTH+1;	
			col = rand()%WIDTH+1;
		}
		while(board[row][col] == -1);
		board[row][col] = -1;
		if(board[row - 1][col] != -1)++board[row - 1][col];
		if(board[row - 1][col-1]!=-1)++board[row - 1][col-1];
		if(board[row - 1][col+1]!=-1)++board[row-1][col+1];
		if(board[row + 1][col]!=-1)++board[row + 1][col];
		if(board[row + 1][col-1]!=-1)++board[row + 1][col-1];
		if(board[row + 1][col+1]!=-1)++board[row + 1][col+1];
		if(board[row][col+1]!=-1)++board[row][col+1];
		if(board[row][col-1]!=-1)++board[row][col-1];
	}

	for(int i = 1; i<=WIDTH;++i){
		for(int j = 1; j<=WIDTH;++j){
			cout<<setw(3)<<board[i][j];
		}
		cout<<endl;
	}
 	system("pause");
}