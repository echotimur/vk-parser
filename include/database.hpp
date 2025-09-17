#pragma once

#include <string>
#include <fstream>

#include "nlohmann/json.hpp"


class DATABASE {

    public:

        // DATABASE(std::string user_id);

        DATABASE();

        DATABASE(nlohmann::json data_to_add);

        ~DATABASE();



        void users_id_add(std::vector<int>& users_to_add);

        void get_users_id(std::vector<int>& id);


    private:

        nlohmann::json data_to_add_;

        std::fstream users_list_;

        std::fstream users_info_;


};