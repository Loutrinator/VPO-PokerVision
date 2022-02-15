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
	std::string windowName = "card";
	bool BnWInputs = true;

	// Chargement des images
	cv::Mat cardsImage = cv::imread("resources/cartes.jpg");
	if (cardsImage.empty()){
		std::cout << "Could not open or find the card image" << std::endl;
		system("pause");
		return -1;
	}
	cv::Mat testFile = cv::imread("resources/table-easy1.jpg");
	if (testFile.empty()) {
		std::cout << "Could not open or find the image to test" << std::endl;
		system("pause");
		return -1;
	}

	cv::Mat tRes, tResNb, cres;
	std::vector<std::vector<cv::Point> > contours;

	cv::cvtColor(testFile, tResNb, cv::COLOR_BGR2GRAY);
	cv::threshold( tResNb, tRes, 128,255, cv::THRESH_BINARY);
	cv::findContours(tRes, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	cv::Mat copy = testFile.clone();

	cv::drawContours(copy, contours, -1, cv::Scalar( 255,0,0 ),5);
	PokerVision::showImage(copy, "cres", 720);


	bool pipOnly = false;
	int nbPointsToMatch = 10;
	float angleTolerance = 5;
	PokerVision p(pipOnly, nbPointsToMatch, angleTolerance);

	Image testImage(testFile);
	testImage.name = "Test image";

	p.divide(testImage.mat, cv::Vec3b(187,218,234));

	//testImage.convertToHsv();

	cv::Ptr<cv::ORB> imageOrb = cv::ORB::create(50000, 1.2, 8, 1, 0, 2,cv::ORB::FAST_SCORE);
	testImage.detectAndCompute(imageOrb);
	
	p.setCardsDataset(cardsImage,4,13);//setup de l'engine
	p.findCards(testImage, true);
	p.removeOverlapingImages(150);
	p.groupCards(300);
	
	bool showProcessedImages = true;
	bool showCards = true;
	bool showPoints = false;
	bool showText = true;
	bool showROI = true;
	bool showBarrycenter = false;
	p.showResult(testImage, showProcessedImages, showCards, showPoints, showText, showROI, showBarrycenter);

	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}