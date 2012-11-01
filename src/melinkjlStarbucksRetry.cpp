#include "melinjlStarbucksRetry.h"

void melinkjlStarbucksRetry::build(Entry* c, int n)
{
	lengthOfArray = n;
	entryArray = new Entry[n];
	for (int i = 0; i < n; i++)
	{
		entryArray[i] = c[i];
	}
//	int counter = 0;
//	int numNewArrayElements = 0;
//	int indexOfCurrentLowest;
//	sortedArray = new Entry[n];
//	while (counter < n)
//	{
//		for (i = 0; i < n-1; i++)
//		{
//			if (c[i].x > c[i+1].x)
//			{
//				indexOfCurrentLowest = i;
//			}
//		}
//		if (counter >= 1)
//		{
//			for (i = 0; i < n-1; i++)
//			{
//				if (c[indexOfCurrentLowest].x - c[i].x > .00001)
//				{
//					if (c[indexOfCurrentLowest].y - c[i].y > .00001)
//					{
//						sortedArray[counter] = c[indexOfCurrentLowest];
//						numNewArrayElements++;
//					}
//				}
//			}
//		}
//		counter++;
//	}
//	finalSortedArray = new Entry[numNewArrayElements + 1];
//	for (int i = 0; i <= numNewArrayElements-1; i++)
//	{
//		finalSortedArray[i] = (*sortedArray)[i];
//	}
//	int mostIsolatedStarbucksDist;
//	distArray = new double[numNewArrayElements - 1];
//	for (int i = 0; i < numNewArrayElements - 1; i++)
//	{
//		mostIsolatedStarbucksDist = 1;
//		for (int j = i + 1; j < numNewArrayElements; j++)
//		{
//			distBetweenTwoStarbucks = sqrt(((finalSortedArray[i].x - finalSortedArray[j].x) * 
//				(finalSortedArray[i].x - finalSortedArray[j].x)) + ((finalSortedArray[i].y - 
//				finalSortedArray)[j].y) * (finalSortedArray[i].y - finalSortedArray[j].y)))
//			if (distBetweenTwoStarbucks < mostIsolatedStarbucksDist)
//			{
//				mostIsolatedStarbucksDist = distBetweenTwoStarbucks;
//			}
//		}
//		distArray[i] = distBetweenTwoStarbucks;
//	}
//	double largestDistFromOneStarbucksToNextClosestStarbucks = distArray[0];
//	for (int i = 1, i <= numNewArrayElements - 2; i++)
//	{
//		if (largestDistFromOneStarbucksToNextClosestStarbucks < distArray[i])
//		{
//			largestDistFromOneStarbucksToNextClosestStarbucks = distArray[i];
//		}
//	}
//	finalSortedArray[numNewArrayElements] = largestDistFromOneStarbucksToNextClosestStarbucks;
}

Entry* melinkjlStarbucksRetry::getNearest(double x, double y)
{
	double shortestDist = 1;
	double distCur;
	int currentIndex = 0;
	for (int i = 0; i < lengthOfArray; i++)
	{
		distCur = sqrt((((entryArray[i].x - x) * (entryArray[i].x - x)) + ((entryArray[i].y - y) * (entryArray[i].y - y);
		if (distCur < shortestDist)
		{
			shortestDist = distCur;
			currentIndex = i;
		}
	}
	return entryArray[currentIndex];
}