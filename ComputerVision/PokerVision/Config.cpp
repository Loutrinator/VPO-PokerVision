#include "Config.h"
#include <iostream>
//#include <vector>
#include <fstream>
#include <iomanip>


#include "nlohmann/json.hpp"
using json = nlohmann::json;

Config::Config(std::string configName)
{
	std::ifstream config_file("resources/config.json", std::ifstream::binary);
	json j = json::parse(config_file);

}
