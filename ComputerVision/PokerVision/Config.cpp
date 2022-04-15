#include "Config.h"
#include <iostream>
//#include <vector>
#include <fstream>
#include <iomanip>


#include "nlohmann/json.hpp"
using json = nlohmann::json;

Config::Config(std::string configName)
{
	std::ifstream config_file("resources/" + configName + ".json", std::ifstream::binary);
	json j = json::parse(config_file);
	enableLogs = j["enableLogs"];
	showResult = j["showResult"];
	pipOnly = j["pipOnly"];
	correctWhiteBalance = j["correctWhiteBalance"];
	cardBrightnessContrast = j["cardBrightnessContrast"];

	nbPointsToMatch = j["nbPointsToMatch"];
	angleTolerance = j["angleTolerance"];
	imageOrbMaxPointCount = j["imageOrbMaxPointCount"];
	cardOrbMaxPointCount = j["cardOrbMaxPointCount"];
	overlapDistThreshold = j["overlapDistThreshold"];
	groupingDistance = j["groupingDistance"];
}
