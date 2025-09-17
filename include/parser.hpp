#pragma once

#ifndef CPPHTTPLIB_OPENSSL_SUPPORT
#define CPPHTTPLIB_OPENSSL_SUPPORT
#endif

#include <string>

#include "nlohmann/json.hpp"
#include "cpp-httplib/httplib.h"

#include "database.hpp"


class PARSER {

    public:

        PARSER(std::string apiKey, std::string group_id);

        PARSER(std::string apiKey, std::string group_id, float offset);
        
        PARSER(std::string apiKey);

        ~PARSER();



        int getCountOfUsers();

        void getGroupUsers(std::vector<int>& response_id);

        bool getUserInfo(int id, nlohmann::json& response);

        std::string urlEncode(const std::string& value);


    private:

        std::string api_key_ = "";

        std::string group_id_ = "";

        float offset_ = 0;

        httplib::Client cli_;


};