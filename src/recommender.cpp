#include <string>
#include <optional>

#include "nlohmann/json.hpp"
#include "cpp-httplib/httplib.h"

#include "recommender.hpp"


RECOMMENDER::RECOMMENDER (nlohmann::json data) : data_(data) {}


std::optional<nlohmann::json> RECOMMENDER::filter () {

    // std::cout << data_.dump();
    // return std::nullopt;

    if (data_.contains("execute_errors")) return std::nullopt;
    // else if (data_.contains("city")) return data_;
    else if (!data_["response"]["user_info"][0]["city"].empty() && data_["response"]["user_info"][0]["city"]["title"] == "Moscow") return data_;
    // else return data_;

    return std::nullopt;


}
