#include "stack.h"
int main()
{
	srand(time(NULL));
	double rnd;
#if 0 
	Stack s(N);
	try
	{
		for(int i = 0 ; i <N;++i){
			s.push(i);
		}
		
		for(int i = 0 ; i <N; ++i){
			printf("%d\n",s.pop());
		}
	}
	catch(exception & e)	
	{
	}
#endif
#if 1 
	doubleStack ds(N);
	double rands[N];
	try{
		for(int i = 0 ; i  < N; ++i)
		{
			rands[i] = drandom();
			if(rands[i]<0.5){
				ds.push(i,RIGHT);
			}
			else{
				ds.push(i,LEFT);
			}
		}
		for(int i = N-1 ; i>=0;--i)
		{
			if(rands[i]<0.5)
			{
				printf("right=%d\n",ds.pop(RIGHT));
			}
			else
			{
				printf("left = %d\n",ds.pop(LEFT));
			}
		}
	}
	catch(exception & e){
	}
#endif
	return 0;
}
