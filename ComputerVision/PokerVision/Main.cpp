// Main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>
#include <string>
#include <math.h>

#include "CardValue.h"
#include "Card.h"
#include "PokerVision.h"

int main(int argc, char** argv)
{
	std::cout << "RONAN " << std::endl;
	std::string windowName = "card";
	bool BnWInputs = true;

	// Chargement des images
	cv::Mat cardsImage = cv::imread("resources/cartes.jpg");
	if (cardsImage.empty()){
		std::cout << "Could not open or find the card image" << std::endl;
		system("pause");
		return -1;
	}
	cv::Mat testFile = cv::imread("resources/test-2.png");
	if (cardsImage.empty()) {
		std::cout << "Could not open or find the image to test" << std::endl;
		system("pause");
		return -1;
	}
	bool pipOnly = false;
	int nbPointsToMatch = 100;
	float angleTolerance = 5;
	PokerVision p(pipOnly, nbPointsToMatch, angleTolerance);

	Image testImage(testFile);
	testImage.name = "Test image";
	p.brightnessContrast(testImage.mat, 1.3, -15);
	p.increaseReadability(testImage.mat);
	cv::Ptr<cv::ORB> imageOrb = cv::ORB::create(50000, 1.2, 8, 1, 0, 2,cv::ORB::FAST_SCORE);
	testImage.detectAndCompute(imageOrb);

	p.setCardsDataset(cardsImage,4,13);//setup de l'engine
	p.findCards(testImage,false);
	
	bool showProcessedImages = false;
	bool showCards = true;
	bool showPoints = false;
	bool showText = true;
	bool showROI = true;
	p.showResult(testImage, showProcessedImages, showCards, showPoints, showText, showROI, false);

	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}