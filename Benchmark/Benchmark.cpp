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
using json = nlohmann::json;

float threshold = 400;

/// <summary>
/// Compare two list of points of cards
/// </summary>
/// <param name="point1">position of the first cards</param>
/// <param name="point2">position of the second cards</param>
/// <returns>true if the points are close enough, false otherwise</returns>
bool comparePoints(json point1, json point2) {
	
	//Stock the position values of each card in array
	std::vector<float> card1 = { point1[0]["x"], point1[0]["y"], point1[1]["x"],point1[1]["y"],
								point1[2]["x"], point1[2]["y"], point1[3]["x"], point1[3]["y"]};
	std::vector<float> card2 = { point2[0]["x"], point2[0]["y"], point2[1]["x"],point2[1]["y"],
								point2[2]["x"], point2[2]["y"], point2[3]["x"], point2[3]["y"] };
	
	//sort the position values for simpler comparison
	std::sort(card1.begin(), card1.end());
	std::sort(card2.begin(), card2.end());

	
	for (int i = 0; i < card1.size(); i++) {
		//Check if the value are further than the value of threshold
		if (std::abs(card1[i] - card2[i]) > threshold) {
			return false;
		}
	}
	return true;
}

int main()
{
	std::string path = "resources/";
	std::string filename;

	//Iteration on all files in resources (Ronan files)
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (entry.path().extension().u8string() == ".json") {
			filename = entry.path().stem().u8string();
		}
		else {
			continue;
		}
		std::ifstream user_file("resources/" + filename + ".json", std::ifstream::binary);

		//Iteration on all configurations corresponding to selected image (Antoine files)
		for (int i = 1; i < 4; i++) {
			std::string outputName = "measures_config" + std::to_string(i) + "_" + filename;
			std::ifstream config_file("../output/" + outputName + ".json", std::ifstream::binary);

			if (user_file.fail()) {
				std::cout << "User File does not exist :(";
				return 0;
			}

			json userJson = json::parse(user_file);
			json resultJson = json::parse(config_file);

			bool foundCard = false;
			int goodCardCount = 0;
			int badCardCount = 0;
			//Iterate on cards of Antoine files
			for (int x = 0; x < resultJson["cards"].size(); x++) {
				//Iterate on cards of Ronan files
				for (int y = 0; y < userJson["cards"].size(); y++) {
					//Compare rank, suit and position of two cards 
					if (resultJson["cards"][x]["rank"] == userJson["cards"][y]["rank"]
						&& resultJson["cards"][x]["suit"] == userJson["cards"][y]["suit"]
						
						&& comparePoints(resultJson["cards"][x]["points"], userJson["cards"][y]["points"])
						/* && userJson["cards"][i]["group"] == resultJson["cards"][j]["group"]*/) {
						
						//If cards are corresponding, increment good counter
						goodCardCount++;
						foundCard = true;
						break;
					}
				}
				//If the card is not found the result is bad, increment bad counter
				if (!foundCard)
					badCardCount++;
				
				foundCard = false;
			}

			//Creation of benchmark json
			json benchmark;
			//Number of good cards detected
			benchmark["Goodmatch"] = goodCardCount;
			//Number of cards in the base image
			benchmark["totalcards"] = userJson["cards"].size();
			//Error card found
			benchmark["Badmatch"] = badCardCount;
			//Percentage of cards found according to the total of cards in the image
			benchmark["percentage"] = (float)goodCardCount / (float)userJson["cards"].size();

			//Antoine values
			benchmark["executiontime"] = resultJson["totalTimespan"];
			benchmark["cardsearchtime"] = resultJson["findCardsTimespan"];
			benchmark["grouptime"] = resultJson["groupingTimespan"];
			benchmark["overlaptime"] = resultJson["removeOverlapTimespan"];

			std::string filename("../output/" + outputName + "_benchmarked.json");
			std::ofstream outfile;

			outfile.open(filename, std::ofstream::out | std::ofstream::trunc);

			if (!outfile.is_open()) {
				std::cerr << "failed to open " << filename << '\n';
			}
			else {
				outfile.write(benchmark.dump(4).c_str(), benchmark.dump(4).size());
				std::cerr << "Done Writing!" << std::endl;
			}
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
