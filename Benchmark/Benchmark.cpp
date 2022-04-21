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

bool comparePoints(json point1, json point2) {
	std::vector<float> card1 = { point1[0]["x"], point1[0]["y"], point1[1]["x"],point1[1]["y"],
								point1[2]["x"], point1[2]["y"], point1[3]["x"], point1[3]["y"]};
	std::vector<float> card2 = { point2[0]["x"], point2[0]["y"], point2[1]["x"],point2[1]["y"],
								point2[2]["x"], point2[2]["y"], point2[3]["x"], point2[3]["y"] };

	std::sort(card1.begin(), card1.end());
	std::sort(card2.begin(), card2.end());
	for (int i = 0; i < card1.size(); i++) {
		if (std::abs(card1[i] - card2[i]) > 400) {
			return false;
		}
	}
	return true;
}

int main()
{
	std::string path = "resources/";
	std::string filename;
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (entry.path().extension().u8string() == ".json") {
			filename = entry.path().stem().u8string();
		}
		else {
			continue;
		}
		for (int i = 1; i < 4; i++) {
			std::string outputName = "measures_config" + std::to_string(i) + "_" + filename;
			std::ifstream config_file("../output/" + outputName + ".json", std::ifstream::binary);

			std::ifstream user_file("resources/" + filename + ".json", std::ifstream::binary);
			if (user_file.fail()) {
				std::cout << "User File does not exist :(";
				return 0;
			}

			json userJson = json::parse(user_file);
			json resultJson = json::parse(config_file);

			bool foundCard = false;
			int goodCardCount = 0;
			int badCardCount = 0;
			for (int x = 0; x < resultJson["cards"].size(); x++) {
				for (int y = 0; y < userJson["cards"].size(); y++) {
					if (resultJson["cards"][x]["rank"] == userJson["cards"][y]["rank"]
						&& resultJson["cards"][x]["suit"] == userJson["cards"][y]["suit"]
						/* && userJson["cards"][i]["group"] == resultJson["cards"][j]["group"]*/
						&& comparePoints(resultJson["cards"][x]["points"], userJson["cards"][y]["points"])) {
						goodCardCount++;
						foundCard = true;
						break;
					}
				}
				if (!foundCard)
					badCardCount++;
				foundCard = false;
			}

			json benchmark;
			benchmark["Goodmatch"] = goodCardCount;
			benchmark["totalcards"] = userJson["cards"].size();

			benchmark["Badmatch"] = badCardCount;
			benchmark["percentage"] = (float)goodCardCount / (float)userJson["cards"].size();
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
