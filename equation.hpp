#pragma once

#include <vector>
#include <string>
#include <cmath>
#include <cstring>
#include <rapidxml/rapidxml.hpp>
#include "random.hpp"

namespace lb {
    struct Term{
        int base;
        int exponent;
        char operation;
    };
    namespace Equation{
        struct Settings{
            std::vector<Term> _equation;
            int min_terms, max_terms;
            int min_base, max_base;
            int min_exp, max_exp;
            std::string operations;
        };

        void generate(void* memory);
        std::string solve(void* memory);
        std::string stringify(void* memory);

        void state_to_xml(rapidxml::xml_document<char>& doc, rapidxml::xml_node<char>* node, void* memory);
        void xml_to_state(rapidxml::xml_node<char>* node, void* memory);
    }
}
