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
	cv::Mat testFile = cv::imread("resources/test-3.png");
	if (cardsImage.empty()) {
		std::cout << "Could not open or find the image to test" << std::endl;
		system("pause");
		return -1;
	}
	PokerVision p;
	p.showCards = true;
	p.showKeypoints = false;
	p.pipOnly = false;
	p.nbPointsToMatch = 250;

	cv::Mat testMat = testFile.clone();
	p.brightnessContrast(testMat, 1.3, -15);
	//p.increaseReadability(testMat);
	cv::Ptr<cv::ORB> imageOrb = cv::ORB::create(50000, 1.2, 8, 1, 2);
	Image testImage(testMat, imageOrb);
	testImage.name = "Test image";

	p.setCardsDataset(cardsImage,4,13);//setup de l'engine
	p.findCards(testImage, true);

	std::cout << p.cards.size() << std::endl;
	//cv::cvtColor(testImage.mat, testImage.mat, cv::COLOR_GRAY2BGR);
	testImage.showImage(false);
	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}