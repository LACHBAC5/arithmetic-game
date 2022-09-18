#include "doomsday.hpp"

namespace lb{
    void Doomsday::generate(void* memory){
        Settings* state = static_cast<Settings*>(memory);
        state->_year = Rand::genInt(state->min_year, state->max_year);
        state->_month = Rand::genInt(state->min_month, state->max_month);
        state->_day = Rand::genInt(state->min_day, state->max_day);
    }

    std::string Doomsday::solve(void* memory){
        Settings* state = static_cast<Settings*>(memory);
        int century_Doomsday = (1+5*(state->_year%4)+4*(state->_year%100)+6*(state->_year%400))%7;
        if(state->_year%4==0 && state->_month <= 2){
            century_Doomsday--;
        }

        int delta_days = (state->_day-Doomsdays[state->_month-1]+century_Doomsday)%7;
        if(delta_days<0){
            delta_days+=7;
        }
        return week[delta_days];
    }

    std::string Doomsday::stringify(void* memory){
        Settings* state = static_cast<Settings*>(memory);
        return std::to_string(state->_year)+"/"+std::to_string(state->_month)+"/"+std::to_string(state->_day);
    }

    void Doomsday::state_to_xml(rapidxml::xml_document<char>& doc, rapidxml::xml_node<char>* node, void* memory){
        Settings* state = static_cast<Settings*>(memory);
        node->append_attribute(doc.allocate_attribute("min_year", doc.allocate_string(std::to_string(state->min_year).c_str())));
        node->append_attribute(doc.allocate_attribute("max_year", doc.allocate_string(std::to_string(state->max_year).c_str())));
        node->append_attribute(doc.allocate_attribute("min_month", doc.allocate_string(std::to_string(state->min_month).c_str())));
        node->append_attribute(doc.allocate_attribute("max_month", doc.allocate_string(std::to_string(state->max_month).c_str())));
        node->append_attribute(doc.allocate_attribute("min_day", doc.allocate_string(std::to_string(state->min_day).c_str())));
        node->append_attribute(doc.allocate_attribute("max_day", doc.allocate_string(std::to_string(state->max_day).c_str())));
    }
    void Doomsday::xml_to_state(rapidxml::xml_node<char>* node, void* memory){
        Settings* state = static_cast<Settings*>(memory);
        state->min_year=std::stoi(node->first_attribute("min_year")->value());
        state->max_year=std::stoi(node->first_attribute("max_year")->value());
        state->min_month=std::stoi(node->first_attribute("min_month")->value());
        state->max_month=std::stoi(node->first_attribute("max_month")->value());
        state->min_day=std::stoi(node->first_attribute("min_day")->value());
        state->max_day=std::stoi(node->first_attribute("max_day")->value());
    }
}
