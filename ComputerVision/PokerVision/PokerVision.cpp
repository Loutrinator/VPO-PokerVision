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
			Card card(tmp(cardROI),value);

			brightnessContrast(card.mat, 1.3, -15);
			increaseReadability(card.mat);
			card.detectAndCompute(cardOrb);

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

		card.nbMatches = card.matchesGood.size();

		if (card.nbMatches > nbPointsToMatch) {

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
			card.center = getBarrycenter(card.gameCorners);
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

void PokerVision::removeOverlapingImages(float minDist)
{
	std::vector<std::vector<int>> groups = getGroupCardIdsByDistance(minDist);

	std::vector<Card> cleanedUpCards;
	for (int i = 0; i < groups.size(); ++i) {
		std::vector<int> group = groups[i];
		if (group.size() > 1) {

			int maxMatched = -15000;
			int maxMatchedId = 0;
			std::cout << "multiple matches in one place !" << std::endl;
			for (int j = 0; j < group.size(); ++j) {
				int cardId = group[j];
				std::cout << foundCards[cardId].cardValue.ToShortString() << " : " << foundCards[cardId].nbMatches << std::endl;
				if (maxMatched < foundCards[cardId].nbMatches) {
					maxMatchedId = cardId;
					maxMatched = foundCards[cardId].nbMatches;
				}
			}
			Card chosenCard = foundCards[maxMatchedId];
			std::cout << "Chosen card : " << chosenCard.cardValue.ToShortString() << " maxMatched : " << maxMatched << " card matches : " << chosenCard.nbMatches << std::endl;
			cleanedUpCards.push_back(chosenCard);

		}
		else {
			cleanedUpCards.push_back(foundCards[group[0]]);
		}
	}

	foundCards = cleanedUpCards;
	
}

void PokerVision::groupCards(float minDist)
{
	std::vector<std::vector<int>> groups = getGroupCardIdsByDistance(minDist);

	for (int i = 0; i < groups.size(); ++i) {
		std::vector<int> group = groups[i];
		CardGroup cg;
		for (int j = 0; j < group.size(); ++j) {
			int cardId = group[j];
			cg.cards.push_back(foundCards[cardId]);
		}
		float hue = (180 / groups.size()) * i;
		cv::Scalar bgrColor = HSV2BGR(hue, 255, 255);
		std::cout << "hue : " << hue << " color " << bgrColor[0] << ", " << bgrColor[1] << ", " << bgrColor[2] << std::endl;
		cg.setColors(bgrColor, cv::Scalar(255, 255, 0));
		cardGroups.push_back(cg);
	}
}

cv::Point2f PokerVision::getBarrycenter(std::vector<cv::Point2f> points)
{
	float x = 0;
	float y = 0;
	int count = 0;
	for (cv::Point2f point : points) {
		x += point.x;
		y += point.y;
		count++;
	}
	x = x / (float)count;
	y = y / (float)count;
	return cv::Point2f(x, y);
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

void PokerVision::showResult(const Image& img, bool showProcessedImage, bool showCards, bool showPoints, bool showText, bool showROI, bool showBarrycenters)
{
	//Settings pour le texte
	int font = cv::FONT_HERSHEY_SIMPLEX;
	cv::Point2f textOffset(0, 15);
	float fontScale = 0.4;
	cv::Scalar fontColor(255, 255, 255);
	int thickness = 1.5;
	int lineType = 2;


	cv::Mat showImage;
	
	if (showProcessedImage) {
		showImage = img.mat.clone();
		cv::cvtColor(showImage, showImage, cv::COLOR_GRAY2BGR);
	}
	else {
		showImage = img.originalMat.clone();
	}

	int roiThickness = 2;


	if (showPoints) {
		for (int i = 0; i < img.keypoints.size(); ++i) {
			cv::KeyPoint k = img.keypoints[i];
			cv::circle(showImage, k.pt, 1, cv::Scalar(0, 255, 0), 1);
		}
	}
	std::vector<CardGroup> cgs = cardGroups;
	for (CardGroup group : cardGroups) {
		float minROIx = 10000000;
		float minROIy = 10000000;
		float maxROIx = -10000000;
		float maxROIy = -10000000;

		std::string desc = "(";

		int cardCount = 0;
		for (Card card : group.cards) {
			if (cardCount > 0) desc += ", ";
			cardCount++;
			desc += card.cardValue.ToString();

			if (showROI) {
				//Dessine sur le flux vidéo
				cv::line(showImage, card.gameCorners[0], card.gameCorners[1], card.roiColor, roiThickness);
				cv::line(showImage, card.gameCorners[1], card.gameCorners[2], card.roiColor, roiThickness);
				cv::line(showImage, card.gameCorners[2], card.gameCorners[3], card.roiColor, roiThickness);
				cv::line(showImage, card.gameCorners[3], card.gameCorners[0], card.roiColor, roiThickness);
			}
			if (showBarrycenters) {
				cv::circle(showImage, card.center, 2, card.centerColor, 2);
			}

			if (showCards) {
				card.showImage(showPoints, !showProcessedImage);
			}

			//création du ROI du groupe pour afficher le texte en dessous
			for (cv::Point2f corner : card.gameCorners) {
				if (corner.x < minROIx) minROIx = corner.x;
				if (corner.y < minROIy) minROIy = corner.y;
				if (corner.x > maxROIx) maxROIx = corner.x;
				if (corner.y > maxROIy) maxROIy = corner.y;
			}
		}
		desc += ")";

		//Tracage de la description de la main du joueur
		if (showText) {
			cv::Point2f textPos((minROIx + maxROIx) / 2, maxROIy);
			textPos += textOffset;
			textPos += cv::Point2f(-(fontScale * 15 * desc.length())/2,0); //décalage proportionnel a la longueur du texte pour le centrer par rapport au groupe de cartes
			cv::putText(showImage, desc, textPos, font, fontScale, fontColor, thickness, lineType);
		}
	}
	

	cv::imshow("Result", showImage);

}

bool PokerVision::angleIsValid(float angle)
{
	return (angle > minAngle && angle < maxAngle);
}

cv::Scalar PokerVision::HSV2BGR(float h, float s, float v)
{
	cv::Mat hsv(1, 1, CV_8UC3, cv::Scalar(h, s, v));
	cv::Mat	bgr;
	cv::cvtColor(hsv, bgr, cv::COLOR_HSV2BGR);
	return cv::Scalar(bgr.data[0], bgr.data[1], bgr.data[2]);
}

cv::Scalar PokerVision::BGR2HSV(float b, float g, float r)
{
	cv::Mat bgr(1, 1, CV_8UC3, cv::Scalar(b, g, r));
	cv::Mat	hsv;
	cv::cvtColor(bgr, hsv, cv::COLOR_BGR2HSV);
	return cv::Scalar(hsv.data[0], hsv.data[1], hsv.data[2]);
}

std::vector<std::vector<int>> PokerVision::getGroupCardIdsByDistance(float dist)
{

	std::vector<cv::Point2f> sums;
	std::vector<std::vector<int>> groups;

	std::vector<int> group;
	group.push_back(0);
	groups.push_back(group);//ajout du premier groupe contenant la première carte
	sums.push_back(foundCards[0].center);

	for (int i = 1; i < foundCards.size(); ++i) {
		cv::Point2f center = foundCards[i].center;
		bool addedToGroup = false;
		for (int j = 0; j < groups.size(); ++j) {
			std::vector<int> g = groups[j];
			cv::Point2f moyPoint = cv::Point2f(sums[j].x / g.size(), sums[j].y / g.size());
			if (cv::norm(moyPoint - center) < dist) { // on est assez proche de la norme 
				groups[j].push_back(i);
				sums[j] += center;
				addedToGroup = true;
				break;
			}
		}
		if (!addedToGroup) {
			std::vector<int> g;
			g.push_back(i);
			groups.push_back(g);
			sums.push_back(foundCards[i].center);
		}
	}
	return groups;

}
