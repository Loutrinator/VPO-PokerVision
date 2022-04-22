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
	/*if (argc == 2) {
		photoName = argv[1];
	}
	else if (argc == 3) {

		configName = argv[1];
		photoName = argv[2];
	}*/

	clock_t findStart, overlapStart, groupStart, groupEnd;

	for (int configNumber = 1; configNumber <= 3; configNumber++) {
		std::string configName = "config" + std::to_string(configNumber);
		for (const auto& entry : std::filesystem::directory_iterator("resources/")) {
			if (entry.path().extension().u8string() == ".jpg" && entry.path().u8string() != "resources/cartes.jpg") {
				std::string photoPath = entry.path().u8string();// "table-easy1.jpg";
				std::string photoName = entry.path().stem().u8string();// "table-easy1.jpg";
				std::cout << photoName << std::endl;
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
				cv::Mat testFile = cv::imread(photoPath);
				if (testFile.empty() && config.enableLogs) {
					std::cout << "Could not open or find the image to test" << std::endl;
					system("pause");
					return -1;
				}
				PokerVision p(config);

				Image testImage(testFile);
				testImage.name = "Test image";

				if (config.correctWhiteBalance)
					p.divide(testImage.mat, cv::Vec3b(187, 218, 234));

				//testImage.convertToHsv();

				cv::Ptr<cv::ORB> imageOrb = cv::ORB::create(config.imageOrbMaxPointCount, 1.2, 8, 1, 0, 2, cv::ORB::FAST_SCORE);
				testImage.detectAndCompute(imageOrb);

				p.setCardsDataset(cardsImage, 4, 13);//setup de l'engine

				findStart = clock();
				p.findCards(testImage);
				overlapStart = clock();

				if (p.foundCards.size() > 0) {
					p.removeOverlapingImages(config.overlapDistThreshold);
					groupStart = clock();
					p.groupCards(config.groupingDistance);
					measures["removeOverlapTimespan"] = (float)(groupStart - overlapStart) / 1000;
					measures["groupingTimespan"] = (float)(groupEnd - groupStart) / 1000;
				}
				else {
					measures["removeOverlapTimespan"] = 0;
					measures["groupingTimespan"] = 0;
				}
				groupEnd = clock();

				//MEASURES
				measures["findCardsTimespan"] = (float)(overlapStart - findStart) / 1000;
				measures["totalTimespan"] = (float)(groupEnd - findStart) / 1000;


				//SHOW IMAGE
				bool showProcessedImages = true;
				bool showCards = true;
				bool showPoints = false;
				bool showText = true;
				bool showROI = true;
				bool showBarrycenter = false;
				if (config.showResult) {
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

			}
		}
	}
	

	return 0;
}