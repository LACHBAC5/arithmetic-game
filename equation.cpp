#include "equation.hpp"

namespace lb{
    void Equation::generate(void* memory){
        Settings* state = static_cast<Settings*>(memory);

        // nullify the pointer position, wherever it is, so it won't be mistakenly freed after assignment.
        memset(memory, 0, sizeof(std::vector<Term>)); state->_equation={};
        //memcpy(memory, &lb::unmove(std::vector<Term>()), sizeof(std::vector<Term>)); // cursed vector construction :)

        state->_equation.clear();
        int terms = lb::Rand::genInt(state->min_terms, state->max_terms);
        for(int i = 0; i < terms; i++){
            state->_equation.push_back({
                lb::Rand::genInt(state->min_base, state->max_base),  
                lb::Rand::genInt(state->min_exp, state->max_exp),
                state->operations[lb::Rand::genInt(0, state->operations.size()-1)]
            });
        }
    }

    std::string Equation::solve(void* memory){
        Settings* state = static_cast<Settings*>(memory);
        double sum=pow(state->_equation[0].base, state->_equation[0].exponent);
        for(int i = 1; i < state->_equation.size(); i++){
            double scalar = pow(state->_equation[i].base, state->_equation[i].exponent);
            //std::cout << scalar << ' ' << sum << '\n';
            if(state->_equation[i-1].operation=='-'){
                sum-=scalar;
            }
            else if(state->_equation[i-1].operation=='+'){
                sum+=scalar;
            }
            else if(state->_equation[i-1].operation=='/'){
                sum/=scalar;
            }
            else if(state->_equation[i-1].operation=='*'){
                sum*=scalar;
            }
        }
        std::string out = std::to_string(sum);
        out = out.substr(0, out.find('.')+3);
        return out;
    }

    std::string Equation::stringify(void* memory){
        Settings* state = static_cast<Settings*>(memory);
        std::string out;
        if(state->_equation.size()==0){
            return out;
        }
        int i = 0;
        for(; i < state->_equation.size()-1; i++){
            out += std::to_string(state->_equation[i].base)+"^"+std::to_string(state->_equation[i].exponent)+" "+state->_equation[i].operation+" ";
        }
        out += std::to_string(state->_equation[i].base)+"^"+std::to_string(state->_equation[i].exponent);
        return out;
    }

    void Equation::state_to_xml(rapidxml::xml_document<char>& doc, rapidxml::xml_node<char>* node, void* memory){
        Settings* state = static_cast<Settings*>(memory);
        node->append_attribute(doc.allocate_attribute("min_terms", doc.allocate_string(std::to_string(state->min_terms).c_str())));
        node->append_attribute(doc.allocate_attribute("max_terms", doc.allocate_string(std::to_string(state->max_terms).c_str())));
        node->append_attribute(doc.allocate_attribute("min_base", doc.allocate_string(std::to_string(state->min_base).c_str())));
        node->append_attribute(doc.allocate_attribute("max_base", doc.allocate_string(std::to_string(state->max_base).c_str())));
        node->append_attribute(doc.allocate_attribute("min_exp", doc.allocate_string(std::to_string(state->min_exp).c_str())));
        node->append_attribute(doc.allocate_attribute("max_exp", doc.allocate_string(std::to_string(state->max_exp).c_str())));
        node->append_attribute(doc.allocate_attribute("operations", state->operations.c_str()));
    }

    void Equation::xml_to_state(rapidxml::xml_node<char>* node, void* memory){
        Settings* state = static_cast<Settings*>(memory);
        memset(memory, 0, sizeof(std::vector<Term>)); state->_equation={};
        memset(&state->operations, 0, sizeof(std::string));
        state->min_terms=std::stoi(node->first_attribute("min_terms")->value());
        state->max_terms=std::stoi(node->first_attribute("max_terms")->value());
        state->min_base=std::stoi(node->first_attribute("min_base")->value());
        state->max_base=std::stoi(node->first_attribute("max_base")->value());
        state->min_exp=std::stoi(node->first_attribute("min_exp")->value());
        state->max_exp=std::stoi(node->first_attribute("max_exp")->value());
        state->operations=node->first_attribute("operations")->value();

    }
}