#include "parser.hpp"

#include <string>
#include <cstdlib>
#include <vector>
#include <unordered_set>
#include <thread>
#include <chrono>


#include "nlohmann/json.hpp"
#include "cpp-httplib/httplib.h"
#include "termcolor/termcolor.hpp"


PARSER::PARSER (std::string api_key, std::string group_id) : 
    api_key_(api_key),
    group_id_(group_id),
    cli_("https://api.vk.com") {
}


PARSER::PARSER (std::string api_key, std::string group_id, float offset) : 
    api_key_(api_key),
    group_id_(group_id),
    offset_(offset),
    cli_("https://api.vk.com") {
}


PARSER::PARSER (std::string api_key) : 
    api_key_(api_key),
    cli_("https://api.vk.com") {
}


PARSER::~PARSER () {

    if (offset_ != 0)

        std::cout << termcolor::green << "Group " << group_id_ << " was parsed!" << termcolor::reset << std::endl;

}



int PARSER::getCountOfUsers () {

    int user_count = 0;

    try {

        auto res = cli_.Get("/method/groups.getMembers?group_id=" + group_id_ + "&count=1&access_token=" + api_key_ + "&v=5.131");

        nlohmann::json response = nlohmann::json::parse(res->body);

        user_count = response["response"]["count"].get<int>();

        if (res->status != 200) {

            std::cerr << termcolor::red << "ERROR: " << termcolor::reset << "Group " << termcolor::yellow << group_id_ << termcolor::reset << " have a private data (user count), or another problem with connection" << std::endl;

        }

        if (user_count > 1000) {

            std::cerr << termcolor::magenta << "WARNING:" << termcolor::reset;

            std::cerr << " Count of users in group " << group_id_ << " more than 1000 (" << termcolor::red << user_count << termcolor::reset << "). It means you need to write a cicle to get and processing more than 1000 user's ID. Please transfer in method .getGroupUsers int offset argument (" << termcolor::red << user_count << termcolor::reset << " / 1000)..." << std::endl;

        }


    } catch (const std::string& e) {

        std::cerr << termcolor::yellow << "EXCEPTION: " << termcolor::reset << e;

    }

    return user_count;

}


void PARSER::getGroupUsers (std::vector<int>& response_id) {

    try{
            
        nlohmann::json json_response;

        for (int i = 0; i < offset_/1000; i++) {

            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            auto res = cli_.Get("/method/groups.getMembers?group_id=" + group_id_ + "&count=1000&offset=" + std::to_string(i * 1000) + "&access_token=" + api_key_ + "&v=5.131");

            json_response = nlohmann::json::parse(res->body);

            for (int j = 0; j < 1000; j++) {

                if (!json_response["response"]["items"][j].is_null()) {

                    response_id.push_back(json_response["response"]["items"][j]);

                }

            }

        }
        
        std::unordered_set<int> seen;

        for (std::vector<int>::iterator it = response_id.begin(); it != response_id.end(); it++) {

            seen.insert(*it);

        }

        response_id.clear();

        for (int elem : seen) {
            response_id.push_back(elem);
        }

    } catch (std::exception& e) {
        std::cerr << termcolor::yellow << "EXCEPTION: " << termcolor::reset << e.what();
    }

}



bool PARSER::getUserInfo (int id, nlohmann::json& response) {
    
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    std::string request = "/method/execute?code=return\%20%7B\%0A\%20\%20\%20\%20\%22user_info\%22\%3A\%20API.users.get\%28%7B\%22user_ids\%22\%3A\%20\%22" + std::to_string(id) 
    + "\%22\%2C\%20\%22fields\%22\%3A\%20%22bdate\%2Ccity\%2Cphoto_max\%2Ccan_post\%2Ccan_see_all_posts\%2Ccan_see_audio\%2Cinterests\%2Cbooks\%2Ctv\%2Cquotes\%2Cabout\%2Cgames"
    + "\%2Cmovies\%2Cactivities\%2Cmusic\%2Csite\%2Cstatus\%2Cfollowers_count\%2Coccupation\%2Ccareer\%2Cmilitary\%2Chome_town\%2Cpersonal\%2Cuniversities\%2Cschools\%2Crelatives\%2Csex\%2Cphoto_50"
    + "\%2Cphoto_100\%2Cverified\%22\%7D\%29\%2C\%0A\%20\%20\%20\%20%22wall_posts\%22\%3A\%20API.wall.get\%28%7B\%22owner_id\%22\%3A\%20" + std::to_string(id) 
    + "\%2C\%20\%22count\%22\%3A\%2010\%7D\%29\%0A\%7D%3B&access_token=" + api_key_ + "&v=5.131";

    auto res = cli_.Get(request.c_str());

    if (res->status != 200) return true;

    response = nlohmann::json::parse(res->body);

    if (response.contains("execute_errors")) return true;

    return false;

}