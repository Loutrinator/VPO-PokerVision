// Main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <math.h>
#include <chrono>

#include "CardValue.h"
#include "Card.h"
#include "PokerVision.h"

#include "nlohmann/json.hpp"
#include "Config.h"
using json = nlohmann::json;


int main(int argc, char** argv)
{

	clock_t findStart, overlapStart, groupStart, groupEnd;

	std::string configName = "config1";
	std::string photoName = "table-easy1";
	if (argc == 2) {
		photoName = argv[1];
	}
	else if (argc == 3) {

		configName = argv[1];
		photoName = argv[2];
	}

	Config config(configName);
	json measures;

	std::string windowName = "card";
	bool BnWInputs = true;

	// Chargement des images
	cv::Mat cardsImage = cv::imread("resources/cartes.jpg");
	if (cardsImage.empty() && config.enableLogs) {
		std::cout << "Could not open or find the card image" << std::endl;
		system("pause");
		return -1;
	}
	else {
		std::cout << "Cards file read." << std::endl;
	}
	cv::Mat testFile = cv::imread("resources/" + photoName + ".jpg");
	if (testFile.empty() && config.enableLogs) {
		std::cout << "Could not open or find the image to test" << std::endl;
		system("pause");
		return -1;
	}
	else {
		std::cout << "Game file read." << std::endl;
	}
	PokerVision p(config);

	Image testImage(testFile);
	//p.showImage(testImage.mat, "original", 1000);
	testImage.name = "Test image";

	//p.removeHueAndValueRange(testImage.mat, 14, 28, 120, 255); //Suppression de la couleur du bois
	//p.showImage(testImage.mat, "Bois retiré", 800);
	p.removeHueAndValueRange(testImage.mat, 95, 145, 16, 145); //Suppression de la couleur du tapis
	p.borderMasking(testImage.mat, 150, 3);

	if (config.correctWhiteBalance)
		p.divide(testImage.mat, cv::Vec3b(187, 218, 234));
	//p.showImage(testImage.mat, "platean nettoyé", 800);

	cv::Mat processedTestImage;
	cv::cvtColor(testImage.mat, processedTestImage, cv::COLOR_BGR2GRAY);
	cv::threshold(processedTestImage, processedTestImage, 190, 255, cv::THRESH_BINARY);

	p.removeUndesiredLines(testImage.mat);

	//cv::adaptiveThreshold(processedTestImage, processedTestImage, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 11, 2);

	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(processedTestImage, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);


	//p.showImage(contourImage,"contours",1000);
	//cv::waitKey(0);

	cv::Mat ROIImage = processedTestImage.clone();
	cv::cvtColor(ROIImage, ROIImage, cv::COLOR_GRAY2BGR);
	cv::drawContours(ROIImage, contours, -1, cv::Scalar(255, 0, 0), 5, cv::LINE_AA);

	std::vector<cv::Rect> validROIs;
	for (int i = 0; i < contours.size(); i++) {
		cv::Rect ROI = cv::boundingRect(contours[i]);
		if (ROI.width > 200 && ROI.width < 1000 && ROI.height > 200 && ROI.height < 1000) {
			validROIs.push_back(ROI);
		}
		//p.showImage(ROIImage, "ROIImage", 1000);
		//cv::waitKey(0);
	}
	std::vector<cv::Mat> croppedImages;
	std::vector<cv::Rect> croppedImagesROI;
	for (int i = 0; i < validROIs.size(); i++) {
		std::cout << "roi " << i << std::endl;
		cv::Rect ROI = validROIs[i];
		cv::rectangle(ROIImage, ROI, cv::Scalar(0, 0, 255), 5);
		cv::Mat cropped = testImage.mat(ROI);
		p.showImage(cropped, "cropped " + std::to_string(i), 500);
		croppedImages.push_back(cropped);
		croppedImagesROI.push_back(ROI);
	}
	p.showImage(ROIImage, "ROIImage", 800);
	//cv::waitKey(0);


	cv::Ptr<cv::ORB> imageOrb = cv::ORB::create(config.imageOrbMaxPointCount, 1.2, 8, 1, 0, 2, cv::ORB::FAST_SCORE);
	std::cout << "Setting card dataset" << std::endl;
	p.setCardsDataset(cardsImage, 4, 13);//setup de l'engine
	std::cout << "Card dataset created" << std::endl;

	testImage.detectAndCompute(imageOrb);
	findStart = clock();
	p.findCards(testImage);
	std::cout << "Found cards : " << p.foundCards.size() << std::endl;
	overlapStart = clock();
	p.removeOverlapingImages(config.overlapDistThreshold);
	groupStart = clock();
	p.groupCards(config.groupingDistance);
	groupEnd = clock();


	//MEASURES
	measures["findCardsTimespan"] = (float)(overlapStart - findStart)/1000;
	measures["removeOverlapTimespan"] = (float)(groupStart - overlapStart)/1000;
	measures["groupingTimespan"] = (float)(groupEnd - groupStart)/1000;
	measures["totalTimespan"] = (float)(groupEnd - findStart)/1000;


	//SHOW IMAGE
	bool showProcessedImages = true;
	bool showCards = true;
	bool showPoints = false;
	bool showText = true;
	bool showROI = true;
	bool showBarrycenter = false;
	if (config.showResult) {
		std::cout << "showResult " << std::endl;
		p.showResult(testImage, showProcessedImages, showCards, showPoints, showText, showROI, showBarrycenter);
		cv::waitKey(0);
		cv::destroyAllWindows();
	}


	std::vector<json> cardsJsonList;
	for (int i = 0; i < p.cardGroups.size(); i++) {
		CardGroup& group = p.cardGroups[i];
		for (int j = 0; j < group.cards.size(); j++) {
			json cardJson;
			cardJson["group"] = i;
			Card& card = group.cards[j];
			cardJson["suit"] = card.cardValue.suit;//coeur pique etc
			cardJson["rank"] = card.cardValue.rank;//As 7 Roi etc

			std::vector<json> points;
			for (int k = 0; k < card.gameCorners.size(); k++) {
				json corner;
				corner["x"] = card.gameCorners[k].x;
				corner["y"] = card.gameCorners[k].y;
				points.push_back(corner);
			}
			cardJson["points"] = points;
			cardsJsonList.push_back(cardJson);
		}
	}
	measures["cards"] = cardsJsonList;



	std::string sMeasures = measures.dump(4);
	std::string outputName = "measures_" + configName + "_" + photoName + ".json";
	std::ofstream measureFile;

	measureFile.open("../../output/" + outputName, std::ofstream::out | std::ofstream::trunc);
	if (!measureFile.is_open()) {
		std::cerr << "Unable to open " + outputName << std::endl;
	}
	else {
		measureFile.write(sMeasures.data(), sMeasures.size());
		std::cout << "Writing complete!" << std::endl;
	}
	return 0;
}