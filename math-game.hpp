#pragma once

#include <iostream>
#include <map>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_print.hpp>
#include <fstream>

#include <fcntl.h>
#include <unistd.h>

#include "equation.hpp"
#include "doomsday.hpp"
#include "read.hpp"

namespace lb {
    class Math_game{
        public:
        struct Settings{
            std::string folder;
        };

        void set_state(const Settings& state);
        const Settings& get_state();

        int load_xml();
        int store_xml();

        int init();
        int add_package(const std::string& package_name, int package_pos);
        int remove_package(const std::string& package_name);
        int add_game(const std::string& package_name, const std::string& game_name, int game_pos);
        int remove_game(const std::string& package_name, const std::string& game_name);
        int add_mod(const std::string& package_name, const std::string& game_name, const std::string& mod_name, int mod_pos);
        int remove_mod(const std::string& package_name, const std::string& game_name, const std::string& mod_name);
        int add_setting(const std::string& package_name, const std::string& game_name, const std::string& mod_name, const std::pair<std::string, std::string>& setting);
        int add_settings(const std::string& package_name, const std::string& game_name, const std::string& mod_name, const std::vector<std::pair<std::string, std::string>>& settings);
        int remove_setting(const std::string& package_name, const std::string& game_name, const std::string& mod_name, const std::string& setting);
        int remove_settings(const std::string& package_name, const std::string& game_name, const std::string& mod_name, const std::vector<std::string>& settings);

        static std::string stringify_error(int error);

        std::string stringify_all();
        std::string stringify_package(const std::string& package_name);
        std::string stringify_game(const std::string& package_name, const std::string& game_name);
        std::string stringify_mod(const std::string& package_name, const std::string& game_name, const std::string& mod_name);
        std::string stringify_setting(const std::string& package_name, const std::string& game_name, const std::string& mod_name, const std::string& setting_name);

        std::vector<std::pair<std::string, std::string>> play_all();
        std::vector<std::pair<std::string, std::string>> play_package(const std::string& package);
        std::vector<std::pair<std::string, std::string>> play_game(const std::string& package_name, const std::string& game_name);
        std::vector<std::pair<std::string, std::string>> play_mod(const std::string& package_name, const std::string& game_name, const std::string& mod_name);

        private:
        struct Game{
            void(*generate)(void*);
            std::string(*solve)(void*);
            std::string(*stringify)(void*);
            void(*state_to_xml)(rapidxml::xml_document<char>&, rapidxml::xml_node<char>*, void*);
            void(*xml_to_state)(rapidxml::xml_node<char>*, void*);
        };
        Settings _current;
        rapidxml::xml_document<char> _xml_doc;
        std::string _xml_data;
        void* game_memory[512];
        static const std::map<std::string, Game> _games;

        rapidxml::xml_node<char>* find_node_by_attribute(rapidxml::xml_node<char>* first, const std::string& name, const std::string& value);
        rapidxml::xml_node<char>* find_node_by_index(rapidxml::xml_node<char>* first, int pos);

        std::string stringify_recursive(rapidxml::xml_node<char>* node, std::string indent="");
        void play_recursive(std::vector<std::pair<std::string, std::string>>& out, std::string& package_name, std::string& game_name, rapidxml::xml_node<char>* node);
    };
}