#include "database.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include <unordered_set>

#include "nlohmann/json.hpp"
#include "termcolor/termcolor.hpp"


DATABASE::DATABASE () :
    users_list_("../data/users.txt", std::ios::in | std::ios::app) {
}


DATABASE::DATABASE (nlohmann::json data_to_add) :
    data_to_add_(data_to_add),
    users_info_("../datausers.txt", std::ios::in | std::ios::app) {
}



DATABASE::~DATABASE () {

    users_list_.close();

}



void DATABASE::get_users_id (std::vector<int>& user_id) {

    users_list_.seekg(0, std::ios::beg);

    std::string id;
    while (getline(users_list_, id)) {

        user_id.push_back(std::stoi(id));

    }

}



void DATABASE::users_id_add (std::vector<int>& user_id) {

    for (std::vector<int>::iterator it = user_id.begin(); it != user_id.end(); it++) {

        users_list_ << *it << std::endl;

    }

}