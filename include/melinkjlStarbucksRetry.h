#include "Starbucks.h"

class melinkjlStarbucksRetry : public Starbucks 
{
  public:
	virtual void build(Entry* c, int n);
	int lengthOfArray;
	Entry* entryArray;
//	Entry* sortedArray;
//	Entry* finalSortedArray;
//	double* distArray;
	virtual Entry* getNearest(double x, double y);
};