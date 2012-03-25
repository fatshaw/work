#include <iostream>
#include <windows.h>
using namespace std;

int main()
{
	
	int busy = 10;
	int idle = busy;
	unsigned long starttime = GetTickCount();
	for(;;){
		starttime = GetTickCount();
		while(GetTickCount() - starttime < busy);
		Sleep(idle);
	}
	return 0;
}