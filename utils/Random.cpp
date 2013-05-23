#include "Random.h"

#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

CRandom:: CRandom()
{
	long lRealseed		= 0;
	struct timeval tv;
	
	// get times
	gettimeofday(&tv, NULL);
	lRealseed = time(NULL) + (tv.tv_usec % 100);
	
	// random seed
	srand((unsigned int) lRealseed);
}

CRandom:: ~CRandom()
{
}

int CRandom:: RandomInt(int min, int max)
{
	int mmin	= min;
	int mmax	= max;

	// parameter checking
	if(mmax == mmin)
		return 0;
	if(mmax < mmin)
	{
		// swap
		mmax = mmax + mmin;
		mmin = mmax - mmin;
		mmax = mmax - mmin;
	}	

	// generate random number
	return mmin + rand()%(mmax - mmin);
}
