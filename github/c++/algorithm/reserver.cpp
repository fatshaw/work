#include <iostream>
using namespace std;

void reverse(char * a, int n)
{
	int i = 0;
	int j = n-1;
	int t = 0;
	while(i<j)
	{
		t = a[i];
		a[i] = a[j];
		a[j] = t;
		++i;
		--j;
	}	
}



int main()
{
	char str[] = "abcdefghij";
	reverse(str,10);
	for(int i = 0 ; i< 10;i++)
	{
		cout<<"str["<<i<<"]"<<str[i]<<",";
	}
	cout<<endl;
}
