#pragma once

#include <vector>
#include <string>
#include <array>
#include <rapidxml/rapidxml.hpp>
#include "random.hpp"

namespace lb {
    namespace Doomsday {
        struct Settings{
            int _year, _month, _day;
            int min_year, max_year;
            int min_month, max_month;
            int min_day, max_day;
        };

        void generate(void* memory);
        std::string solve(void* memory);
        std::string stringify(void* memory);

        void state_to_xml(rapidxml::xml_document<char>& doc, rapidxml::xml_node<char>* node, void* memory);
        void xml_to_state(rapidxml::xml_node<char>* node, void* memory);

        static std::array<int, 12> Doomsdays = {
            3, 28, 14, 4, 9, 6, 11, 8, 5, 10, 7, 12
        };
        static std::array<std::string, 7> week = {
            "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"
        };
    };
}