#include "PokerVision.h"

PokerVision::PokerVision(bool pip, int nbMatch, float angleMargin): pipOnly(pip), nbPointsToMatch(nbMatch)
{
	cardOrb = cv::ORB::create(2000, 1.2, 8, 1, 0, 2, cv::ORB::FAST_SCORE);
	bfm = cv::BFMatcher::create();
	minAngle = 90 - angleMargin;
	maxAngle = 90 + angleMargin;
}

void PokerVision::setCardsDataset(const cv::Mat& cardsFile, int width, int height)
{
	cv::Mat tmp = cardsFile.clone();
	brightnessContrast(tmp, 1.3, -15);
	increaseReadability(tmp);
	//
	
	cardWidth = floor(cardsFile.cols / width);
	cardHeight = floor(cardsFile.rows / height);
	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {

			//on calcule la zone à lire pour récupérer notre image
			float minX = x * cardWidth + borderOffset;
			float minY = y * cardHeight + borderOffset;
			float maxX = (x + 1) * cardWidth - borderOffset;
			float maxY = (y + 1) * cardHeight - borderOffset;
			
			if(pipOnly){
				float pipRoiCoeffx = 1.3f / 8.0f;
				float pipRoiCoeffy = 3.3f / 11.0f;
				maxX = minX + (maxX - minX) * pipRoiCoeffx;
				maxY = minY + (maxY - minY) * pipRoiCoeffy;
			}

			cv::Rect cardROI(cv::Point2f(minX, minY), cv::Point2f(maxX, maxY));
			CardValue value(CardRank(14 - y), CardSuit(x));
			Card card(tmp(cardROI), value, cardOrb);
			cv::waitKey(0);
			cards.push_back(card);
		}
	}
}

void PokerVision::findCards(Image& img, bool enableLogs)
{
	int cardsFound = 0;
	for (Card card : cards) {
		bfm = cv::BFMatcher::create();
		if (bfm != nullptr) {
			card.knnMatch(bfm, img, 0.75);
		}
		if (enableLogs) {
			std::cout << std::endl << card.cardValue.ToShortString() << " : " << card.matchesGood.size() << " matches";
		}

		if (card.matchesGood.size() > nbPointsToMatch) {

			if (enableLogs) {
				std::cout << " -> VALID";
			}
			//Begin: Clipping
			std::vector<cv::Point2f> obj, scene;
			for (auto i : card.matchesGood) {
				obj.push_back(card.keypoints[i.queryIdx].pt);
				scene.push_back(img.keypoints[i.trainIdx].pt);
			}
			card.homographyMatrix = cv::findHomography(obj, scene, cv::RANSAC);

			cv::perspectiveTransform(card.corners, card.gameCorners, card.homographyMatrix);

			float a1 = std::abs(card.getAngleBetween(card.gameCorners[0], card.gameCorners[1], card.gameCorners[2]));
			float a2 = std::abs(card.getAngleBetween(card.gameCorners[1], card.gameCorners[2], card.gameCorners[3]));
			float a3 = std::abs(card.getAngleBetween(card.gameCorners[2], card.gameCorners[3], card.gameCorners[0]));
			float a4 = std::abs(card.getAngleBetween(card.gameCorners[3], card.gameCorners[0], card.gameCorners[1]));

			if (angleIsValid(a1) && angleIsValid(a1) && angleIsValid(a1) && angleIsValid(a1))
			{
				foundCards.push_back(card);
			}
			cardsFound++;
		}
	}
}

void PokerVision::increaseReadability(cv::Mat& img)
{
	cv::Mat tmp;
	cv::cvtColor(img, tmp, cv::COLOR_BGR2GRAY);
	cv::threshold(tmp, tmp, 160, 255, cv::THRESH_BINARY);
	//cv::adaptiveThreshold(tmp, tmp, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 5, 12);
	img = tmp;
}

void PokerVision::brightnessContrast(cv::Mat& img, double contrast, int brightness)
{
	cv::Mat tmp = cv::Mat::zeros(img.size(), img.type());
	for (int y = 0; y < img.rows; y++) {
		for (int x = 0; x < img.cols; x++) {
			for (int c = 0; c < img.channels(); c++) {
				tmp.at<cv::Vec3b>(y, x)[c] = cv::saturate_cast<uchar>(contrast * img.at<cv::Vec3b>(y, x)[c] + brightness);
			}
		}
	}
	img = tmp;
}

void PokerVision::showResult(const Image img, bool showCards, bool showPoints, bool showText, bool showROI, bool showBarrycenters)
{
	cv::Mat showImage = img.mat.clone();
	cv::cvtColor(showImage, showImage,cv::COLOR_GRAY2BGR);


	cv::Scalar roiColor(0, 0, 255);
	int roiThickness = 2;

	std::string desc = "Detected hand: (";
	int cardCount = 0;

	if (showPoints) {
		for (int i = 0; i < img.keypoints.size(); ++i) {
			cv::KeyPoint k = img.keypoints[i];
			cv::circle(showImage, k.pt, 1, cv::Scalar(0, 255, 0), 1);
		}
	}

	for (Card card : foundCards) {

		if (cardCount > 0) desc += ", ";
		cardCount++;
		desc += card.cardValue.ToShortString();

		if (showROI) {
			//Dessine sur le flux vidéo
			cv::line(showImage, card.gameCorners[0], card.gameCorners[1], roiColor, roiThickness);
			cv::line(showImage, card.gameCorners[1], card.gameCorners[2], roiColor, roiThickness);
			cv::line(showImage, card.gameCorners[2], card.gameCorners[3], roiColor, roiThickness);
			cv::line(showImage, card.gameCorners[3], card.gameCorners[0], roiColor, roiThickness);
			}
		if (showCards) {
			card.showImage(showPoints);
		}
	}
	desc += ")";
	std::cout << desc << std::endl;

	if (showText) {
		int font = cv::FONT_HERSHEY_SIMPLEX;
		cv::Point2f bottomLeftCornerOfText(25, 25);
		float fontScale = 0.7;
		cv::Scalar fontColor(0, 0, 255);
		int thickness = 1;
		int lineType = 2;
		cv::putText(showImage, desc, bottomLeftCornerOfText, font, fontScale, fontColor, thickness, lineType);
	}

	cv::imshow("Result", showImage);

}

bool PokerVision::angleIsValid(float angle)
{
	return (angle > minAngle && angle < maxAngle);
}
