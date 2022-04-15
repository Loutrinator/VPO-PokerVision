// Benchmark.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>
#include <fstream>
#include <Windows.h>
#include <sstream>
#include <string>
#include <codecvt>
#include <locale>
#include "nlohmann/json.hpp"
using json = nlohmann::json;

int main()
{
	std::string config;
	std::string imagePath;
	std::cout << "Choose config 1,2 or 3" << std::endl;
	std::cin >> config;
	std::cout << "Choose image to load" << std::endl;
	std::cin >> imagePath;
	
	std::ifstream user_file( "resources/" + imagePath + ".json", std::ifstream::binary);
	if (user_file.fail() ){
		std::cout << "User File does not exist :(";
		return 0;
	}
	
	std::wstring cmdArgslistSetChannel = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(config);
	cmdArgslistSetChannel += L" " + std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(imagePath);

	HANDLE hProcess;
	HANDLE hThread;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	DWORD dwProcessId = 0;
	DWORD dwThreadId = 0;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	BOOL bCreateProcess = NULL;
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
	
	std::string outputName = "measures_" + config + "_" + imagePath + ".json";
	std::ifstream config_file("../output/" + outputName, std::ifstream::binary);
	
	json userJson = json::parse(user_file);
	json resultJson = json::parse(config_file);

	int goodCardCount = 0;

	for (int i = 0; i < userJson["cards"].size(); i++) {
		for (int j = 0; j < resultJson["cards"].size(); j++) {
			if (userJson["cards"][i]["rank"] == resultJson["cards"][j]["rank"]
				&& userJson["cards"][i]["suit"] == resultJson["cards"][j]["suit"]
				/* && userJson["cards"][i]["group"] == resultJson["cards"][j]["group"]*/) {
				//json userPoints = userJson["cards"][i]["points"];
				//json resultPoints = resultJson["cards"][j]["points"];

				goodCardCount++;
				break;
			}
		}
	}

	std::cout << goodCardCount << std::endl;
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
