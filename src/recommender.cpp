#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <optional>


RECOMENDER::RECOMENDER (nlohmann::json data) : data_(data) {}


nlohmann::json RECOMENDER::filtration () {

	if (data_.contains("execute_error")) return nullopt;
	
	if (!(data_.contains("city") || data_.contains("") ||))

}	
