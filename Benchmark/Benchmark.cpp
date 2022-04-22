// Benchmark.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <fstream>
#include <iostream>
#include <filesystem>
#include <Windows.h>
#include <sstream>
#include <string>
#include <codecvt>
#include <locale>
#include <cmath>
#include "nlohmann/json.hpp"
#include "ImageData.h"
using json = nlohmann::json;

float threshold = 400;

bool sortPoints(PointData position1, PointData position2) { return (position1.x < position2.x); }

/// <summary>
/// Compare two list of points of cards
/// </summary>
/// <param name="points1">position of the first cards</param>
/// <param name="points2">position of the second cards</param>
/// <returns>true if the points are close enough, false otherwise</returns>
float comparePoints(std::vector<PointData>& card1, std::vector<PointData>& card2) {


	//sort the position values for simpler comparison
	std::sort(card1.begin(), card1.end(), sortPoints);
	std::sort(card2.begin(), card2.end(), sortPoints);

	float sumdist = 0;
	for (int i = 0; i < card1.size(); i++) {
		//Check if the value are further than the value of threshold
		float x1 = card1[i].x;
		float x2 = card2[i].x;
		float y1 = card1[i].y;
		float y2 = card2[i].y;
		sumdist += sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
		/*if (std::abs(x1 - x2) > threshold || std::abs(y1 - y2) > threshold) {
			return false;
		}*/
	}
	sumdist /= 4;
	return sumdist;
}
bool sortByGroup(CardsData& a, CardsData& b) {
	return (a.group < b.group);
}
int main()
{
	std::string benchmarkCsv;
	std::string path = "resources/";
	std::string filename;

	//Iteration on all configurations corresponding to selected image (Antoine files)
	for (int i = 1; i < 4; i++) {
		benchmarkCsv = ";Correct;Total;Wrong;Percentage;ROIDifference;Execution;Search;Groupe;Overlap\n";
		std::string filenameJson("../output/config" + std::to_string(i) + "_benchmarked.json");
		std::string filenameCsv("../output/config" + std::to_string(i) + "_benchmarked.csv");
		std::ofstream outfileJson;
		outfileJson.open(filenameJson);
		std::ofstream outfileCsv;
		outfileCsv.open(filenameCsv);
		//Iteration on all files in resources (Ronan files)
		for (const auto& entry : std::filesystem::directory_iterator(path)) {
			if (entry.path().extension().u8string() == ".json") {
				filenameJson = entry.path().stem().u8string();
			}
			else {
				continue;
			}
			benchmarkCsv += filenameJson + ";";

			std::ifstream user_file("resources/" + filenameJson + ".json", std::ifstream::binary);

			std::string outputName = "measures_config" + std::to_string(i) + "_" + filenameJson;
			std::ifstream config_file("../output/" + outputName + ".json", std::ifstream::binary);

			if (user_file.fail()) {
				std::cout << "User File does not exist :(";
				continue;
			}
			if (config_file.fail()) {
				std::cout << "Config File does not exist :(";
				continue;
			}


			json userJson = json::parse(user_file); // Good data
			json resultJson = json::parse(config_file); // Algo data
			ImageData userData;
			ImageData resultData;

			userData = (userJson);
			std::sort(userData.cards.begin(), userData.cards.end(), sortByGroup);

			resultData = (resultJson);
			std::sort(resultData.cards.begin(), resultData.cards.end(), sortByGroup);


			bool foundCard = false;
			int goodCardCount = 0;
			int goodGroupCount = 0;
			std::vector<int> userGroup;
			std::vector<int> resultGroup;
			int badCardCount = 0;
			float distance = 0;

			for (auto& userCard : userData.cards)
			{
				for (auto& resultCard : resultData.cards)
				{
					if (resultCard.rank == userCard.rank
						&& resultCard.suit == userCard.suit) {

						distance = comparePoints(resultCard.points, userCard.points);
						if (distance < threshold) {
							goodCardCount++;
							foundCard = true;
							break;
						}
					}
				}

				if (!foundCard)
					badCardCount++;

				foundCard = false;
			}
			//Creation of benchmark json
			json benchmarkJson;
			//Number of good cards detected
			benchmarkJson["Goodmatch"] = goodCardCount;
			benchmarkCsv += std::to_string(goodCardCount) + ";";
			//Number of cards in the base image
			benchmarkJson["totalcards"] = userData.cards.size();
			benchmarkCsv += std::to_string(userData.cards.size()) + ";";

			//Error card found
			benchmarkJson["Badmatch"] = badCardCount;
			benchmarkCsv += std::to_string(badCardCount) + ";";

			//Percentage of cards found according to the total of cards in the image
			benchmarkJson["percentage"] = (float)goodCardCount / (float)userData.cards.size();
			benchmarkCsv += std::to_string((float)goodCardCount / (float)userData.cards.size()) + ";";

			benchmarkJson["ROImargin"] = distance;
			benchmarkCsv += std::to_string(distance) + ";";


			//Antoine values
			benchmarkJson["executiontime"] = resultJson["totalTimespan"];
			benchmarkCsv += std::to_string((float)resultJson["totalTimespan"]) + ";";

			benchmarkJson["cardsearchtime"] = resultJson["findCardsTimespan"];
			benchmarkCsv += std::to_string((float)resultJson["findCardsTimespan"]) + ";";

			benchmarkJson["grouptime"] = resultJson["groupingTimespan"];
			benchmarkCsv += std::to_string((float)resultJson["groupingTimespan"]) + ";";

			benchmarkJson["overlaptime"] = resultJson["removeOverlapTimespan"];
			benchmarkCsv += std::to_string((float)resultJson["removeOverlapTimespan"]) + "\n";

			if (!outfileJson.is_open() ) {
				std::cerr << "failed to open " << filenameJson << '\n';
			}
			else {
				outfileJson.write(benchmarkJson.dump(4).c_str(), benchmarkJson.dump(4).size());
				
				std::cerr << "Done Writing!" << std::endl;
			}

		}
		if (!outfileCsv.is_open()) {
			std::cerr << "failed to open " << filenameCsv << '\n';
		}
		else {
			outfileCsv.write(benchmarkCsv.c_str(), benchmarkCsv.size());

			std::cerr << "Done Writing!" << std::endl;
		}
	}




	//Crash with opencv in PokerVision.exe

	//std::wstring cmdArgslistSetChannel = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(config);
	//cmdArgslistSetChannel += L" " + std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(imagePath);

	/*HANDLE hProcess;
	HANDLE hThread;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	DWORD dwProcessId = 0;
	DWORD dwThreadId = 0;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	BOOL bCreateProcess = NULL;
	try {
		bCreateProcess = CreateProcess(
			L"D:\\ESGI\\5A\\VPO-PokerVision\\ComputerVision\\x64\\Debug\\PokerVision.exe",
			&cmdArgslistSetChannel[0],
			NULL,
			NULL,
			FALSE,
			0,
			NULL,
			NULL,
			&si,
			&pi);

		WaitForSingleObject(pi.hProcess, INFINITE);

		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
	catch (std::exception e){}*/
}





// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
