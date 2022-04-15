// Main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>
#include <string>
#include <math.h>

#include "CardValue.h"
#include "Card.h"


#include "DataQualifier.h"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>
#include "ImageData.h"
#include "CardsData.h"

#include <string>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

cv::Mat image, img0;
int ffillMode = 1;
int cardValue = 0, cardColor = 0;
std::vector<cv::Point> points;
std::vector<cv::Mat> undo_image;
int roiThickness = 4;
ImageData datas;

static void help(char** argv)
{
	std::cout << "\nThis program demonstrated the floodFill() function\n"
		"Call:\n"
		<< argv[0]
		<< " [image_name -- Default: fruits.jpg]\n" << std::endl;

	std::cout << "Hot keys: \n"
		"\tESC - quit the program\n"
		"\tc - switch color/grayscale mode\n"
		"\tm - switch mask mode\n"
		"\tr - restore the original image\n"
		"\ts - use null-range floodfill\n"
		"\tf - use gradient floodfill with fixed(absolute) range\n"
		"\tg - use gradient floodfill with floating(relative) range\n"
		"\t4 - use 4-connectivity mode\n"
		"\t8 - use 8-connectivity mode\n" << std::endl;
}


static void drawPoints(const std::vector<cv::Point> p, cv::Scalar roiColor = cv::Scalar(255, 0, 0)) {
	if (p.size() >= 2)
		cv::line(image, p[0], p[1], roiColor, roiThickness);
	if (p.size() >= 3)
		cv::line(image, p[1], p[2], roiColor, roiThickness);
	if (p.size() == 4) {
		cv::line(image, p[2], p[3], roiColor, roiThickness);
		cv::line(image, p[3], p[0], roiColor, roiThickness);
	}

	cv::imshow("image", image);
}

static void onMouse(int event, int x, int y, int, void*)
{
	if (event != cv::EVENT_LBUTTONDOWN)
		return;
	if (points.size() >= 4)
		return;

	cv::Point p = cv::Point(x, y);
	//Ajouter les points lors du clic
	//clic enter for show les carr�es
	points.push_back(p);
	drawPoints(points);
	std::cout << "(x : " << p.x << "; y :" << p.y << ";) pixels were repainted\n";
	std::cout << points.size() << std::endl;
}


int main(int argc, char** argv)
{
	std::string path = "resources";
	bool changeImage = false;

	cv::namedWindow("image", 0);
	cv::createTrackbar("card_number", "image", &cardValue, 12, 0);
	cv::createTrackbar("color_card", "image", &cardColor, 3, 0);

	for (const auto& entry : fs::directory_iterator(path)) {
		changeImage = false;
		std::cout << entry.path() << std::endl;
		img0 = cv::imread(entry.path().u8string());

		if (img0.empty())
		{
			break;
		}
		img0.copyTo(image);
		cv::setMouseCallback("image", onMouse, 0);
		while (!changeImage)
		{
			cv::imshow("image", image);

			for (auto& ps : datas.cards)
			{
				drawPoints(ps.pointsRaw, cv::Scalar(128, 128, 128));
			}
			char c = (char)cv::waitKey(0);

			std::cout << c << std::endl;
			if (c == 27)
			{
				std::cout << "Exiting ...\n";
				break;
			}
			switch (c)
			{
			case 'r': // Reload images
				std::cout << "Original image is restored\n";
				points.clear();
				datas = ImageData();
				img0.copyTo(image);
				break;

			case 'v': // Validate 4 points
				std::cout << "Point & card values validate\n";
				datas.addCard(cardValue, cardColor, NULL, points);
				points.clear();
				break;

			case 'g': // Generate JSON
				changeImage = true;
				datas = ImageData();
				//TODO : close & generate JSON
				break;

			case 's': // Skip
				changeImage = true;
				points.clear();
				datas = ImageData();
				std::cout << "Skip\n";
				break;
			}
		}
	}
	return 0;
}

//int main(int argc, char** argv)
//{
//	std::string windowName = "card";
//	bool BnWInputs = true;
//
//	// Chargement des images
//	cv::Mat cardsImage = cv::imread("resources/cartes.jpg");
//	if (cardsImage.empty()){
//		std::cout << "Could not open or find the card image" << std::endl;
//		system("pause");
//		return -1;
//	}
//	cv::Mat testFile = cv::imread("resources/table-easy1.jpg");
//	if (testFile.empty()) {
//		std::cout << "Could not open or find the image to test" << std::endl;
//		system("pause");
//		return -1;
//	}
//	bool pipOnly = false;
//	int nbPointsToMatch = 10;
//	float angleTolerance = 5;
//	DataQualifier p(pipOnly, nbPointsToMatch, angleTolerance);
//
//	Image testImage(testFile);
//	testImage.name = "Test image";
//
//	p.divide(testImage.mat, cv::Vec3b(187,218,234));
//
//	//testImage.convertToHsv();
//
//
//
//	cv::Ptr<cv::ORB> imageOrb = cv::ORB::create(50000, 1.2, 8, 1, 0, 2,cv::ORB::FAST_SCORE);
//	testImage.detectAndCompute(imageOrb);
//
//	p.setCardsDataset(cardsImage,4,13);//setup de l'engine
//	p.findCards(testImage, true);
//	p.removeOverlapingImages(150);
//	p.groupCards(300);
//	
//	bool showProcessedImages = true;
//	bool showCards = true;
//	bool showPoints = false;
//	bool showText = true;
//	bool showROI = true;
//	bool showBarrycenter = false;
//	p.showResult(testImage, showProcessedImages, showCards, showPoints, showText, showROI, showBarrycenter);
//
//	cv::waitKey(0);
//	cv::destroyAllWindows();
//	return 0;
//}