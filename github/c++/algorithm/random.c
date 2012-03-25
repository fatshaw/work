#include <stdlib.h>
//generate double random number between 0 and 1, excluding 1. [0,1)
extern "C" double drandom()
{
	return double(rand())/ (double(RAND_MAX)+1);
}

//generate random number between low and high, excluding high. [low,high)
extern "C" int rrandom(int low,int high)
{
	return (rand() %(high - low) + low);
}
