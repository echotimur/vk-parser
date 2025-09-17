#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <optional>


class RECOMENDER {
	
	public:
	
		RECOMENDER(nlohmann::json data);
		
		std::optional<nlohmann::json> filtration();
	
	private:
	
		nlohmann::json data_;
	
};
