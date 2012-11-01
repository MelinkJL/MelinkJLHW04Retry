#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "melinkjlStarbucksRetry.h"
#include <iostream>
#include <fstream>

using namespace ci;
using namespace ci::app;
using namespace std;

class MelinkJLHW04RetryApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	int numberOfEntries;
	Entry* readIntoArray();
};

void MelinkJLHW04RetryApp::setup()
{
	numberOfEntries = 0;
	Entry* entryArray = readIntoArray();
	melinkjlStarbucks* sortedArray = new melinkjlStarbucks;
	(*sortedArray).build(entryArray, numberOfEntries);
	delete [] entryArray;
	Entry* testPoint = (*sortedArray).getNearest(.74326, .61053);
	cout << (*testPoint).x;
}

Entry* MelinkJLHW04RetryApp::readIntoArray()
{
	string name;
	double xPos, yPos;
	ifstream input("../../../resources/Starbucks_2006.csv");
	if (input.is_open())
	{
		// Determines the size of the array to be instantiated.
		while (input.good())
		{
			getline(input, name, '\r');
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
			getline(input, name, '\r');
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

void MelinkJLHW04RetryApp::mouseDown( MouseEvent event )
{
}

void MelinkJLHW04RetryApp::update()
{
}

void MelinkJLHW04RetryApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_BASIC( MelinkJLHW04RetryApp, RendererGl )