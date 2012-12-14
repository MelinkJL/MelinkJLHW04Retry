#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "Resources.h"
#include "stdio.h"
#include "math.h"
#include "melinkjlStarbucksRetry.h"
#include <iostream>
#include <fstream>

using namespace ci;
using namespace ci::app;
using namespace std;

const int surfaceSize = 512;

class MelinkJLHW04RetryApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	int numberOfEntries;
	int numberOfEntriesCensus2000;
	int numberOfEntriesCensus2010;
	Entry* readIntoArray();
	Entry* census2000Data();
	Entry* census2010Data();
	void colorByPopulationDensity(Entry* census2000EntryArray, Entry* census2010EntryArray, melinkjlStarbucksRetry* linearSearchArray, int lengthCensus2000, int lengthCensus2010, int numberOfEntries, uint8_t* pixelArray);
	melinkjlStarbucksRetry* linearSearchArray;
	void prepareSettings(Settings* settings);
	Entry* mostRecentEntry;

  private:
	Surface* mySurface;
	uint8_t* pixelArray;
	void clear(uint8_t* pixelArray); 
	void drawCircle(uint8_t* pixelArray, int xCenter, int yCenter, int radius, Color8u blend);
	//gl::Texture* myTexture;
	gl::Texture* USMap;
};

int clickCounter = 0;

void MelinkJLHW04RetryApp::prepareSettings(Settings* settings)
{
	(*settings).setWindowSize(512, 512);
	(*settings).setResizable(false);
}

/*
 * Clears the given pixel array by turning the entire array black.
 * @param pixelArray: The pixel array to be modified.
 */
void MelinkJLHW04RetryApp::clear(uint8_t* pixelArray)
{
	Color8u blend = Color8u(0,0,0);
	for (int y = 0; y < surfaceSize; y++)
	{
		for (int x = 0; x < surfaceSize; x++)
		{
			int pixel = 3 * (x + y * surfaceSize);
			pixelArray[pixel] = blend.r;
			pixelArray[pixel + 1] = blend.g;
			pixelArray[pixel + 2] = blend.b;
		}
	}
}

/*
 * Draws a circle on the screen.
 * @param pixelArray: The pixel array to be modified.
 * @param xCenter: The x-coordinate of the center point of the circle.
 * @param yCenter: The y-coordinate of the center point of the circle.
 * @param radius: The radius of the circle.
 * @param blend: The color to be assigned to the circle.
 */
void MelinkJLHW04RetryApp::drawCircle(uint8_t* pixelArray, int xCenter, int yCenter, int radius, Color8u blend)
{
	double squareRoot = 0;
	for (int y = 0; y < surfaceSize; y++)
	{
		for (int x = 0; x < surfaceSize; x++)
		{
			if (pow((pow((double)yCenter - y, 2) + pow((double)xCenter - x, 2)), .5) <= radius)
			{
				int pixel = 3 * (x + y * surfaceSize);
				pixelArray[pixel] = blend.r;
				pixelArray[pixel + 1] = blend.g;
				pixelArray[pixel + 2] = blend.b;
			}
		}
	}
}

void MelinkJLHW04RetryApp::setup()
{
	mySurface = new Surface(surfaceSize, surfaceSize, false);
	//myTexture = new gl::Texture(*mySurface);
	pixelArray = (*mySurface).getData();
	clear(pixelArray);
	//USMap = new gl::Texture(loadImage(loadResource(RES_US_MAP)));

	numberOfEntries = 0;
	Entry* entryArray = readIntoArray();

	numberOfEntriesCensus2000 = 0;
	numberOfEntriesCensus2010 = 0;
	Entry* census2000EntryArray = census2000Data();
	Entry* census2010EntryArray = census2010Data();
	linearSearchArray = new melinkjlStarbucksRetry;
	(*linearSearchArray).build(entryArray, numberOfEntries);
	colorByPopulationDensity(census2000EntryArray, census2010EntryArray, linearSearchArray, numberOfEntriesCensus2000, numberOfEntriesCensus2010, numberOfEntries, pixelArray);

	for (int i = 0; i < numberOfEntries; i++)
	{
		drawCircle(pixelArray, (int)(surfaceSize * (entryArray[i].x)), 
			(int)(surfaceSize - (surfaceSize * (entryArray[i].y))), 1, Color8u(255, 255, 0));
	}

	delete [] entryArray;
	//Entry* testPoint = (*linearSearchArray).getNearest(.7432, .6105);
}

Entry* MelinkJLHW04RetryApp::readIntoArray()
{
	// Brandon Sonoda's "Double Pass Method" ©
	string name;
	double xPos, yPos;
	ifstream input("../resources/Starbucks_2006.csv");
	if (input.is_open())
	{
		// Determines the size of the array to be instantiated.
		while (input.good())
		{
			getline(input, name, ',');
			input >> xPos;
			input.get();
			input >> yPos;
			input.get();
			numberOfEntries++;
		}
		input.clear();
		input.seekg(0);

		Entry* storeEntries = new Entry[numberOfEntries];
		numberOfEntries = 0;

		// Fills the array with the data.
		while (input.good())
		{
			getline(input, name, ',');
			input >> xPos;
			input.get();
			input >> yPos;
			input.get();
			(*(storeEntries + numberOfEntries)).identifier = name;
			(*(storeEntries + numberOfEntries)).x = xPos;
			(*(storeEntries + numberOfEntries)).y = yPos;
			numberOfEntries++;
		}
		input.close();
		return storeEntries;
	}
	else
	{
		return NULL;
	}
}

Entry* MelinkJLHW04RetryApp::census2000Data()
{
	string population;
	double xPos, yPos;
	ifstream input("../resources/Census_2000_Altered.csv");
	if (input.is_open())
	{
		// Determines the size of the array to be instantiated.
		while (input.good())
		{
			getline(input, population, ',');
			input >> xPos;
			input.get();
			input >> yPos;
			input.get();
			numberOfEntriesCensus2000++;
		}
		input.clear();
		input.seekg(0);

		Entry* storeEntries = new Entry[numberOfEntriesCensus2000];
		numberOfEntriesCensus2000 = 0;

		// Fills the array with the data.
		while (input.good())
		{
			getline(input, population, ',');
			input >> xPos;
			input.get();
			input >> yPos;
			input.get();
			(*(storeEntries + numberOfEntriesCensus2000)).identifier = population;
			(*(storeEntries + numberOfEntriesCensus2000)).x = xPos;
			(*(storeEntries + numberOfEntriesCensus2000)).y = yPos;
			numberOfEntriesCensus2000++;
		}
		input.close();
		return storeEntries;
	}
	else
	{
		return NULL;
	}
}

Entry* MelinkJLHW04RetryApp::census2010Data()
{
	string population;
	double xPos, yPos;
	ifstream input("../resources/Census_2010_Altered.csv");
	if (input.is_open())
	{
		// Determines the size of the array to be instantiated.
		while (input.good())
		{
			getline(input, population, ',');
			input >> xPos;
			input.get();
			input >> yPos;
			input.get();
			numberOfEntriesCensus2010++;
		}
		input.clear();
		input.seekg(0);

		Entry* storeEntries = new Entry[numberOfEntriesCensus2010];
		numberOfEntriesCensus2010 = 0;

		// Fills the array with the data.
		while (input.good())
		{
			getline(input, population, ',');
			input >> xPos;
			input.get();
			input >> yPos;
			input.get();
			(*(storeEntries + numberOfEntriesCensus2010)).identifier = population;
			(*(storeEntries + numberOfEntriesCensus2010)).x = xPos;
			(*(storeEntries + numberOfEntriesCensus2010)).y = yPos;
			numberOfEntriesCensus2010++;
		}
		input.close();
		return storeEntries;
	}
	else
	{
		return NULL;
	}
}

void MelinkJLHW04RetryApp::colorByPopulationDensity(Entry* census2000Array, Entry* census2010Array, melinkjlStarbucksRetry* starbucksLocationsArray, int lengthCensus2000, int lengthCensus2010, int starbucksArraySize, uint8_t* pixelArray)
{
	int totalPopulationStarbucks2000, totalPopulationStarbucks2010, changeInPopulation, greatestPopulationChange = 0;
	//int* changeInPopulationArray = new int[starbucksArraySize];
	int changeInPopulationArray[7655] = {0};
	for (int i = 0; i < lengthCensus2010; i++)
	{
		stringstream(census2010Array[i].identifier) >> totalPopulationStarbucks2010;
		Entry* testPoint = (*linearSearchArray).getNearest(census2010Array[i].x, census2010Array[i].y);
		for (int j = 0; j < numberOfEntries; j++)
		{
			if ((*testPoint).identifier == (*starbucksLocationsArray).entryArray[j].identifier)
			{
				changeInPopulationArray[j] = changeInPopulationArray[j] + totalPopulationStarbucks2010;
				j = numberOfEntries;
			}
		}
	}
	for (int i = 0; i < lengthCensus2000; i++)
	{
		stringstream(census2000Array[i].identifier) >> totalPopulationStarbucks2000;
		Entry* testPoint = (*linearSearchArray).getNearest(census2000Array[i].x, census2000Array[i].y);
		for (int j = 0; j < numberOfEntries; j++)
		{
			if ((*testPoint).identifier == (*starbucksLocationsArray).entryArray[j].identifier)
			{
				changeInPopulationArray[j] = changeInPopulationArray[j] - totalPopulationStarbucks2000;
				j = numberOfEntries;
			}
		}
	}

	for (int i = 0; i < starbucksArraySize; i++)
	{
		if (changeInPopulationArray[i] > greatestPopulationChange)
		{
			greatestPopulationChange = changeInPopulationArray[i];
		}
	}

	for (int i = 0; i < starbucksArraySize; i++)
	{
		int brightness = (int)(((double)changeInPopulationArray[i] / (double)greatestPopulationChange) * 205);
		if (changeInPopulationArray[i] >= 0)
		{
			brightness = brightness * -1;
			drawCircle(pixelArray, (int)(surfaceSize * (*starbucksLocationsArray).entryArray[i].x), 
				(int)(surfaceSize - (surfaceSize * (*starbucksLocationsArray).entryArray[i].y)), 
				5, Color8u(0, (50 + brightness), 0));
		}
		else
		{
			drawCircle(pixelArray, (int)(surfaceSize * (*starbucksLocationsArray).entryArray[i].x), 
				(int)(surfaceSize - (surfaceSize * (*starbucksLocationsArray).entryArray[i].y)), 
				5, Color8u((50 + brightness), 0, 0));
		}
	}

//	int census2000Int, census2010Int, loopCounter = 0;
//	double changeInPopulation, meanX, meanY, greatestPopulationChange = 0;
//	for (int i = 0; i < (2 * lengthCensus2000); i++)
//	{
//		if (i < lengthCensus2000)
//		{
//			stringstream(census2000Array[i].identifier) >> census2000Int;
//			stringstream(census2010Array[i].identifier) >> census2010Int;
//	
//			changeInPopulation = census2010Int - census2000Int;
//			
//			if (changeInPopulation > greatestPopulationChange)
//			{
//				greatestPopulationChange = changeInPopulation;
//			}
//		}
//		else
//		{
//			stringstream(census2000Array[i % lengthCensus2000].identifier) >> census2000Int;
//			stringstream(census2010Array[i % lengthCensus2000].identifier) >> census2010Int;
//	
//			changeInPopulation = census2010Int - census2000Int;
//
//			meanX = (census2000Array[i % lengthCensus2000].x + census2010Array[i % lengthCensus2000].x) / 2;
//			meanY = (census2000Array[i % lengthCensus2000].y + census2010Array[i % lengthCensus2000].y) / 2;
//
//			if (changeInPopulation >= 0)
//			{
//				drawCircle(pixelArray, (int)(surfaceSize * meanX), (int)(surfaceSize - (surfaceSize * meanY)), 3, Color8u(100 + ((changeInPopulation / greatestPopulationChange) * 155), 0, 0));
//			}
//			else
//			{
//				drawCircle(pixelArray, (int)(surfaceSize * meanX), (int)(surfaceSize - (surfaceSize * meanY)), 3, Color8u(0, 100 + ((-1 * (changeInPopulation) / greatestPopulationChange) * 155), 0));
//			}
//		}
//	}
//	for	(int j = lengthCensus2000; j < lengthCensus2010 + (lengthCensus2010 - lengthCensus2000); j++)
//	{
//		if (j < lengthCensus2010)
//		{
//			stringstream(census2010Array[j].identifier) >> census2010Int;
//	
//			changeInPopulation = census2010Int;
//
//			if (changeInPopulation < greatestPopulationChange)
//			{
//				greatestPopulationChange = changeInPopulation;
//			}
//
//			loopCounter++;
//		}
//		else
//		{
//			stringstream(census2010Array[j - loopCounter].identifier) >> census2010Int;
//	
//			changeInPopulation = census2010Int;
//
//			meanX = census2010Array[j - loopCounter].x;
//			meanY = census2010Array[j - loopCounter].y;
//
//			drawCircle(pixelArray, (int)(surfaceSize * meanX), (int)(surfaceSize - (surfaceSize * meanY)), 3, Color8u(100 + ((changeInPopulation / greatestPopulationChange) * 155), 0, 0));
//		}
//	}
}

void MelinkJLHW04RetryApp::mouseDown( MouseEvent event )
{
	if (clickCounter == 0)
	{
		double xPosition, yPosition;
		double nearestX, nearestY;

		xPosition = event.getX();
		yPosition = event.getY();
		
		Entry* nearestPoint = (*linearSearchArray).getNearest((double)(xPosition / surfaceSize), (double)((surfaceSize - yPosition) / surfaceSize));
		mostRecentEntry = nearestPoint;
		nearestX = (*nearestPoint).x;
		nearestY = (*nearestPoint).y;
		drawCircle(pixelArray, (int)(surfaceSize * nearestX), (int)(surfaceSize - (surfaceSize * nearestY)), 1, Color8u(0, 255, 255));
		clickCounter++;
	}
	else
	{
		double xPosition, yPosition;
		double nearestX, nearestY;

		nearestX = (*mostRecentEntry).x;
		nearestY = (*mostRecentEntry).y;

		drawCircle(pixelArray, (int)(surfaceSize * nearestX), (int)(surfaceSize - (surfaceSize * nearestY)), 
			1, Color8u(255, 255, 0));
		
		xPosition = event.getX();
		yPosition = event.getY();
		
		Entry* nearestPoint = (*linearSearchArray).getNearest((double)(xPosition / surfaceSize), (double)((surfaceSize - yPosition) / surfaceSize));
		mostRecentEntry = nearestPoint;
		nearestX = (*nearestPoint).x;
		nearestY = (*nearestPoint).y;
		drawCircle(pixelArray, (int)(surfaceSize * nearestX), (int)(surfaceSize - (surfaceSize * nearestY)), 
			1, Color8u(0, 255, 255));
	}
}

void MelinkJLHW04RetryApp::update()
{
}

void MelinkJLHW04RetryApp::draw()
{
	//gl::Texture USMap( loadImage( loadResource( RES_US_MAP ) ) );
	gl::draw(*mySurface);
	//gl::draw(USMap);
}

CINDER_APP_BASIC( MelinkJLHW04RetryApp, RendererGl )