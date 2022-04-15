#pragma once
#include <string>
class Config
{
public:
	Config(std::string configName);

	//Enables the std::cout logs
	bool enableLogs = false;//true

	//Shows the resulting image as an OpenCV window
	bool showResult = false;//true

	//Enables white balance correction
	bool correctWhiteBalance = false;//true

	//Enable cards brightness and contrast adjustments
	bool cardBrightnessContrast = false;//true

	//Changes the detection to check only for the pip of the cards
	bool pipOnly = false;//false

	//The amount of points required to consider a match between a card and the image valid 
	int nbPointsToMatch = 0;//10

	//The +- angle allowed for the ROI of the cards
	int angleTolerance = 0;//5

	//The amount of points asked to the ORB to find on the poker game image
	int imageOrbMaxPointCount = 0;//50000

	//The amount of points asked to the ORB to find on each card
	int cardOrbMaxPointCount = 0;//2000

	//The distance between cards to consider them to be two cards detected on top of each other (to remove missmatches)
	int overlapDistThreshold = 0;//150

	//The distance between cards to consider them to be in the same group
	int groupingDistance = 0;//300
};

