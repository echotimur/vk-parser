#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <iostream>
#include <string>
#include <fstream>
#include <memory>
#include <unordered_map>

#include "cpp-httplib/httplib.h"
#include "nlohmann/json.hpp"
#include "termcolor/termcolor.hpp"

#include "parser.hpp"
#include "database.hpp"


int main() {

    

    remove("../data/users.txt");

    std::fstream conf("../config/apikey.json");
    nlohmann::json apiKey = nlohmann::json::parse(conf);
    conf.close();
    if (apiKey["vk_apikey"].empty() || !apiKey["vk_apikey"].is_string()) std::cerr << termcolor::red << "ERROR: " << termcolor::reset << "VK API key not readed...\n";

    std::fstream groups("../data/groups.json", std::ios::in);
    nlohmann::json group_id = nlohmann::json::parse(groups);
    groups.close();
    if (group_id["groups"][0].empty() || !group_id["groups"][0].is_string()) std::cerr << termcolor::red << "ERROR: " << termcolor::reset << "Can not read the group ID, check the groups.json file...";


    std::unordered_map<std::string, int> group_list;

    for (const auto& id : group_id["groups"]) {

        std::unique_ptr<PARSER> parse { std::make_unique<PARSER> (apiKey["vk_apikey"], id) };

        group_list[id] = parse->getCountOfUsers();

    }

    std::vector<int> user_id;

    for (const auto& [id, user_count] : group_list) {
    
        std::unique_ptr<PARSER> parse { std::make_unique<PARSER> (apiKey["vk_apikey"], id, user_count) };

        parse->getGroupUsers(user_id);

        // std::cout << id << " | " << user_count << std::endl;

    }

    DATABASE* users_db = new DATABASE();

    users_db->users_id_add(user_id);

    delete users_db;

    try{

        std::unique_ptr<DATABASE> get_id { std::make_unique<DATABASE>() };

        std::vector<int> id;

        get_id->get_users_id(id);
        
        std::unique_ptr<PARSER> getInfo { std::make_unique<PARSER>(apiKey["vk_apikey"]) };

        nlohmann::json user_info_resp;

        std::string fila_name;

        for (const int& i : id) {

            if (getInfo->getUserInfo(i, user_info_resp) == false) {

                fila_name = "../data/db_users/" + std::to_string(static_cast<int>(user_info_resp["response"]["user_info"][0]["id"])) + ".json";

                std::fstream a(fila_name, std::ios::app);

                a << user_info_resp << std::endl;

                a.close();

            }

        }


    } catch (std::exception& e) {
        std::cerr << termcolor::yellow << "EXCEPTION: " << termcolor::reset << e.what();
    }


    // std::unique_ptr<PARSER> ada { std::make_unique<PARSER>(apiKey["vk_apikey"]) };
    // std::cout << ada->getUserInfo(1054179009);

    return 0;
}