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

#include "nlohmann/json.hpp"
#include "Config.h"
using json = nlohmann::json;


int main(int argc, char** argv)
{

	std::string configName = "config1";
	std::string photoName = "photo1";
	if (argc == 2) {
		photoName = argv[1];
	}
	else if (argc == 3) {

		configName = argv[1];
		photoName = argv[2];
	}

	Config config(configName);

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
	PokerVision p(config);

	Image testImage(testFile);
	testImage.name = "Test image";

	p.divide(testImage.mat, cv::Vec3b(187,218,234));

	//testImage.convertToHsv();



	cv::Ptr<cv::ORB> imageOrb = cv::ORB::create(config.imageOrbMaxPointCount, 1.2, 8, 1, 0, 2,cv::ORB::FAST_SCORE);
	testImage.detectAndCompute(imageOrb);

	p.setCardsDataset(cardsImage,4,13);//setup de l'engine
	p.findCards(testImage, true);
	p.removeOverlapingImages(config.overlapDistThreshold);
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