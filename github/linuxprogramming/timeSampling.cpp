#include <iostream>
#include <list>
#include <queue>
#include <time.h>
using namespace std;

#define FREQ  5

typedef deque<time_t>TimeQueue;

int initTimeQueue(TimeQueue & timeQueue)
{
	timeQueue.resize(FREQ,0);
}

int main(int argc,char ** argv)
{
	time_t now = time(0);
	time_t lasttime = time(0);
	time_t span = 0;
	uint64_t freq = 0 ;
	TimeQueue timeQueue;
	initTimeQueue(timeQueue);
	while(true)
	{
		now = time(0);
		span = now - lasttime;
		if(now != lasttime)
		{
			lasttime = now;
		}
		if(span >= FREQ)
		{
			freq = 0;
			timeQueue.assign(FREQ,0);
		}
		else if(span > 0 && span < FREQ)
		{
				for(int i = 0 ; i < span;++i)
				{
					freq -=timeQueue.back();
					timeQueue.pop_back();
				}
				
				for(int i = 0; i < span;++i)
				{
					timeQueue.push_front(0);
				}			
		}
		if(freq > FREQ)
		{
			cout<<"freq exceed max freqence"<<endl;
			sleep(1);
			continue;
		}
		
		++timeQueue.front();
		++freq;
	}
}

