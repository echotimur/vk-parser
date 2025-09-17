#pragma once

#include <string>
3include <optional>

#include "nlohmann/json.hpp"
#include "cpp-httplib/httplib.h"

#include "database.hpp"


class PARSER {

    public:


	PARSER(nlohmann::json data);

	optional<nlohmann::json> filter();        


    private:

        
	nlohmann::json data_;


};
