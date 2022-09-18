#include "math-game.hpp"

namespace lb {
    const std::map<std::string, Math_game::Game> Math_game::_games = {
        {"doomsday", {&lb::Doomsday::generate, &lb::Doomsday::solve, &lb::Doomsday::stringify, &lb::Doomsday::state_to_xml, &lb::Doomsday::xml_to_state}},
        {"arithmetic", {&lb::Equation::generate, &lb::Equation::solve, &lb::Equation::stringify, &lb::Equation::state_to_xml, &lb::Equation::xml_to_state}}
    };

    void Math_game::set_state(const Settings& state){
        _current = state;
    }

    const Math_game::Settings& Math_game::get_state(){
        return _current;
    }

    int Math_game::load_xml(){
        _xml_data = lb::File::readFile(_current.folder+"/state.xml");
        if(_xml_data.size()==0){
            return 1;
        }
        _xml_doc.parse<0>(_xml_data.data());
        return 0;
    }

    int Math_game::store_xml(){
        std::string xml_doc_str; rapidxml::print(std::back_inserter(xml_doc_str), _xml_doc, 0);
        int fd = open((_current.folder+"/state.xml").c_str(), O_WRONLY|O_TRUNC);
        if(fd<0){
            return 1;
        }
        write(fd, xml_doc_str.c_str(), xml_doc_str.size());
        close(fd);
        return 0;
    }

    int Math_game::init(){
        int fd = open((_current.folder+"/state.xml").c_str(), O_CREAT|O_RDWR|O_TRUNC, 0666);
        if(fd<0){
            return 1;
        }
        close(fd);
        _xml_doc.append_node(_xml_doc.allocate_node(rapidxml::node_element, "packages"));
        return 0;
    }

    int Math_game::add_package(const std::string& package_name, int package_pos){
        rapidxml::xml_node<char>* package = find_node_by_attribute(_xml_doc.first_node()->first_node(), "name", package_name);
        if(package!=nullptr){
            return 1;
        }
        package = _xml_doc.allocate_node(rapidxml::node_element, "package");
        package->append_attribute(
            _xml_doc.allocate_attribute("name", _xml_doc.allocate_string(package_name.c_str()))
        );
        _xml_doc.first_node()->insert_node(find_node_by_index(_xml_doc.first_node()->first_node(), package_pos), package);
        return 0;
    }

    int Math_game::remove_package(const std::string& package_name){
        rapidxml::xml_node<char>* package = find_node_by_attribute(_xml_doc.first_node()->first_node(), "name", package_name);
        if(package==nullptr){
            return 1;
        }
        _xml_doc.first_node()->remove_node(package);
        return 0;
    }
    
    int Math_game::add_game(const std::string& package_name, const std::string& game_name, int game_pos){
        rapidxml::xml_node<char>* package = find_node_by_attribute(_xml_doc.first_node()->first_node(), "name", package_name);
        if(package==nullptr){
            return 1;
        }
        rapidxml::xml_node<char>* game = find_node_by_attribute(package->first_node(), "name", game_name);
        if(game!=nullptr){
            return 2;
        }
        game = _xml_doc.allocate_node(rapidxml::node_element, "game");
        game->append_attribute(
            _xml_doc.allocate_attribute("name", _xml_doc.allocate_string(game_name.c_str()))
        );

        package->insert_node(find_node_by_index(package->first_node(), game_pos), game);
        return 0;
    }

    int Math_game::remove_game(const std::string& package_name, const std::string& game_name){
        rapidxml::xml_node<char>* package = find_node_by_attribute(_xml_doc.first_node()->first_node(), "name", package_name);
        if(package==nullptr){
            return 1;
        }
        rapidxml::xml_node<char>* game = find_node_by_attribute(package->first_node(), "name", game_name);
        if(game==nullptr){
            return 2;
        }
        package->remove_node(game);
        return 0;
    }

    int Math_game::add_mod(const std::string& package_name, const std::string& game_name, const std::string& mod_name, int mod_pos){
        rapidxml::xml_node<char>* package = find_node_by_attribute(_xml_doc.first_node()->first_node(), "name", package_name);
        if(package==nullptr){
            return 1;
        }
        rapidxml::xml_node<char>* game = find_node_by_attribute(package->first_node(), "name", game_name);
        if(game==nullptr){
            return 2;
        }
        rapidxml::xml_node<char>* mod = _xml_doc.allocate_node(rapidxml::node_element, "mod");
        mod->append_attribute(
            _xml_doc.allocate_attribute("name", _xml_doc.allocate_string(mod_name.c_str()))
        );

        game->insert_node(find_node_by_index(game->first_node(), mod_pos), mod);
        return 0;
    }

    int Math_game::remove_mod(const std::string& package_name, const std::string& game_name, const std::string& mod_name){
        rapidxml::xml_node<char>* package = find_node_by_attribute(_xml_doc.first_node()->first_node(), "name", package_name);
        if(package==nullptr){
            return 1;
        }
        rapidxml::xml_node<char>* game = find_node_by_attribute(package->first_node(), "name", game_name);
        if(game==nullptr){
            return 2;
        }
        rapidxml::xml_node<char>* mod = find_node_by_attribute(game->first_node(), "name", mod_name);
        if(mod==nullptr){
            return 3;
        }
        game->remove_node(mod);
        return 0;
    }
    
    int Math_game::add_setting(const std::string& package_name, const std::string& game_name, const std::string& mod_name, const std::pair<std::string, std::string>& setting){
        rapidxml::xml_node<char>* package = find_node_by_attribute(_xml_doc.first_node()->first_node(), "name", package_name);
        if(package==nullptr){
            return 1;
        }
        rapidxml::xml_node<char>* game = find_node_by_attribute(package->first_node(), "name", game_name);
        if(game==nullptr){
            return 2;
        }
        rapidxml::xml_node<char>* mod = find_node_by_attribute(game->first_node(), "name", mod_name);
        if(mod==nullptr){
            return 3;
        }
        rapidxml::xml_attribute<char>* attrib = mod->first_attribute(setting.first.c_str());
        
        if(attrib!=nullptr && attrib->name()=="name"){
            return 4;
        }

        mod->append_attribute(
            _xml_doc.allocate_attribute(_xml_doc.allocate_string(setting.first.c_str()), _xml_doc.allocate_string(setting.second.c_str()))
        );
        return 0;
    }

    int Math_game::add_settings(const std::string& package_name, const std::string& game_name, const std::string& mod_name, const std::vector<std::pair<std::string, std::string>>& settings){
        rapidxml::xml_node<char>* package = find_node_by_attribute(_xml_doc.first_node()->first_node(), "name", package_name);
        if(package==nullptr){
            return 1;
        }
        rapidxml::xml_node<char>* game = find_node_by_attribute(package->first_node(), "name", game_name);
        if(game==nullptr){
            return 2;
        }
        rapidxml::xml_node<char>* mod = find_node_by_attribute(game->first_node(), "name", mod_name);
        if(mod==nullptr){
            return 3;
        }

        for(const auto& setting : settings){
            rapidxml::xml_attribute<char>* attrib = mod->first_attribute(setting.first.c_str());
            if(attrib!=nullptr && attrib->name()=="name"){
                return 4;
            }
            mod->append_attribute(
                _xml_doc.allocate_attribute(_xml_doc.allocate_string(setting.first.c_str()), _xml_doc.allocate_string(setting.second.c_str()))
            );
        }
        return 0;
    }

    int Math_game::remove_setting(const std::string& package_name, const std::string& game_name, const std::string& mod_name, const std::string& setting){
        rapidxml::xml_node<char>* package = find_node_by_attribute(_xml_doc.first_node()->first_node(), "name", package_name);
        if(package==nullptr){
            return 1;
        }
        rapidxml::xml_node<char>* game = find_node_by_attribute(package->first_node(), "name", game_name);
        if(game==nullptr){
            return 2;
        }
        rapidxml::xml_node<char>* mod = find_node_by_attribute(game->first_node(), "name", mod_name);
        if(mod==nullptr){
            return 3;
        }
        rapidxml::xml_attribute<char>* attrib = mod->first_attribute(setting.c_str());
        if(attrib==nullptr ||  attrib->name()=="name"){
            return 4;
        }
        mod->remove_attribute(attrib);
        return 0;
    }

    int Math_game::remove_settings(const std::string& package_name, const std::string& game_name, const std::string& mod_name, const std::vector<std::string>& settings){
        rapidxml::xml_node<char>* package = find_node_by_attribute(_xml_doc.first_node()->first_node(), "name", package_name);
        if(package==nullptr){
            return 1;
        }
        rapidxml::xml_node<char>* game = find_node_by_attribute(package->first_node(), "name", game_name);
        if(game==nullptr){
            return 2;
        }
        rapidxml::xml_node<char>* mod = find_node_by_attribute(game->first_node(), "name", mod_name);
        if(mod==nullptr){
            return 3;
        }
        for(const auto setting : settings){
            rapidxml::xml_attribute<char>* attrib = mod->first_attribute(setting.c_str());
            if(attrib==nullptr ||  attrib->name()=="name"){
                return 4;
            }
            mod->remove_attribute(attrib);
        }
        return 0;
    }

    std::string Math_game::stringify_error(int error){
        std::stringstream out;
        switch(error){
            case 0:
                out << "success";
                break;
            case 1:
                out << "package_not_found";
                break;
            case 2:
                out << "game_not_found";
                break;
            case 3:
                out << "mod_not_found";
                break;
            case 4:
                out << "setting_not_found";
                break;
        }
        return out.str();
    }

    std::string Math_game::stringify_all(){
        std::stringstream out;
        out << stringify_recursive(_xml_doc.first_node()->first_node());
        return out.str();
    }

    std::string Math_game::stringify_package(const std::string& package_name){
        std::stringstream out;
        rapidxml::xml_node<char>* package = find_node_by_attribute(_xml_doc.first_node()->first_node(), "name", package_name);
        if(package==nullptr){
            return "none...";
        }
        out << stringify_recursive(package->first_node());
        return out.str();
    }

    std::string Math_game::stringify_game(const std::string& package_name, const std::string& game_name){
        std::stringstream out;
        rapidxml::xml_node<char>* package = find_node_by_attribute(_xml_doc.first_node()->first_node(), "name", package_name);
        if(package==nullptr){
            return "none...";
        }
        rapidxml::xml_node<char>* game = find_node_by_attribute(package->first_node(), "name", game_name);
        if(game==nullptr){
            return "none...";
        }
        out << stringify_recursive(game->first_node());
        return out.str();
    }

    std::string Math_game::stringify_mod(const std::string& package_name, const std::string& game_name, const std::string& mod_name){
        std::stringstream out;
        rapidxml::xml_node<char>* package = find_node_by_attribute(_xml_doc.first_node()->first_node(), "name", package_name);
        if(package==nullptr){
            return "none...";
        }
        rapidxml::xml_node<char>* game = find_node_by_attribute(package->first_node(), "name", game_name);
        if(game==nullptr){
            return "none...";
        }
        rapidxml::xml_node<char>* mod = find_node_by_attribute(game->first_node(), "name", mod_name);
        if(mod==nullptr){
            return "none...";
        }
        rapidxml::xml_attribute<char>* attrib = mod->first_attribute();
        while(attrib!=nullptr){
            if(strcmp(attrib->name(),"name")!=0){
                out << attrib->name() << "=" << attrib->value() << '\n';
            }
            attrib=attrib->next_attribute();
        }
        return out.str();
    }

    std::string Math_game::stringify_setting(const std::string& package_name, const std::string& game_name, const std::string& mod_name, const std::string& setting_name){
        std::stringstream out;

        rapidxml::xml_node<char>* package = find_node_by_attribute(_xml_doc.first_node()->first_node(), "name", package_name);
        if(package==nullptr){
            return "none...";
        }
        rapidxml::xml_node<char>* game = find_node_by_attribute(package->first_node(), "name", game_name);
        if(game==nullptr){
            return "none...";
        }
        rapidxml::xml_node<char>* mod = find_node_by_attribute(game->first_node(), "name", mod_name);
        if(mod==nullptr){
            return "none...";
        }
        rapidxml::xml_attribute<char>* attrib = mod->first_attribute(setting_name.c_str());
        if(attrib==nullptr){
            return "none...";
        }
        out << attrib->value();

        return out.str();
    }

    std::vector<std::pair<std::string, std::string>> Math_game::play_all(){
        std::vector<std::pair<std::string, std::string>> out;
        std::string n_game_name="";
        std::string n_package_name="";
        play_recursive(out, n_package_name, n_game_name, _xml_doc.first_node()->first_node());
        return out;
    }

    std::vector<std::pair<std::string, std::string>> Math_game::play_package(const std::string& package_name){
        std::vector<std::pair<std::string, std::string>> out;
        rapidxml::xml_node<char>* package = find_node_by_attribute(_xml_doc.first_node()->first_node(), "name", package_name);
        if(package==nullptr){
            return out;
        }
        std::string n_game_name="";
        std::string n_package_name=package_name;
        play_recursive(out, n_package_name, n_game_name, package);
        return out;
    }

    std::vector<std::pair<std::string, std::string>> Math_game::play_game(const std::string& package_name, const std::string& game_name){
        std::vector<std::pair<std::string, std::string>> out;
        rapidxml::xml_node<char>* package = find_node_by_attribute(_xml_doc.first_node()->first_node(), "name", package_name);
        if(package==nullptr){
            return out;
        }
        rapidxml::xml_node<char>* game = find_node_by_attribute(package->first_node(), "name", game_name);
        if(game==nullptr){
            return out;
        }
        std::string n_game_name=game_name;
        std::string n_package_name=package_name;
        play_recursive(out, n_package_name, n_game_name, game);
        return out;
    }

    std::vector<std::pair<std::string, std::string>> Math_game::play_mod(const std::string& package_name, const std::string& game_name, const std::string& mod_name){
        std::vector<std::pair<std::string, std::string>> out;
        rapidxml::xml_node<char>* package = find_node_by_attribute(_xml_doc.first_node()->first_node(), "name", package_name);
        if(package==nullptr){
            return out;
        }
        rapidxml::xml_node<char>* game = find_node_by_attribute(package->first_node(), "name", game_name);
        if(game==nullptr){
            return out;
        }
        rapidxml::xml_node<char>* mod = find_node_by_attribute(game->first_node(), "name", mod_name);
        if(mod==nullptr){
            return out;
        }
        std::string n_game_name=game_name;
        std::string n_package_name=package_name;
        play_recursive(out, n_package_name, n_game_name, mod);
        return out;
    }
    
    rapidxml::xml_node<char>* Math_game::find_node_by_attribute(rapidxml::xml_node<char>* first, const std::string& name, const std::string& value){
        rapidxml::xml_node<char>* node = first;
        while(node!=nullptr){
            if(value==node->first_attribute(name.c_str())->value()){
                return node;
            }
            node=node->next_sibling();
        }
        return nullptr;
    }
    rapidxml::xml_node<char>* Math_game::find_node_by_index(rapidxml::xml_node<char>* first, int pos){
        rapidxml::xml_node<char>* node = first;
        while(pos>0&&node!=nullptr){
            node=node->next_sibling();
            pos--;
        }
        return node;
    }

    std::string Math_game::stringify_recursive(rapidxml::xml_node<char>* node, std::string indent){
        std::stringstream out;
        while(node!=nullptr){
            rapidxml::xml_attribute<char>* attrib = node->first_attribute("name");
            out << indent << node->name() << " \"" << attrib->value() << "\":\n";
            attrib=attrib->next_attribute();
            while(attrib!=nullptr){
                out << indent << " " << attrib->name() << "=" << attrib->value() << '\n';
                attrib=attrib->next_attribute();
            }
            out << stringify_recursive(node->first_node(), indent+" ");
            node=node->next_sibling();
        }
        return out.str();
    }

    void Math_game::play_recursive(std::vector<std::pair<std::string, std::string>>& out, std::string& package_name, std::string& game_name, rapidxml::xml_node<char>* node){
        while(node!=nullptr){
            if(strcmp(node->name(),"game")==0){
                game_name=node->first_attribute("name")->value();
            }
            else if(strcmp(node->name(),"mod")==0){
                _games.at(game_name).xml_to_state(node, game_memory);
                _games.at(game_name).generate(game_memory);
                out.push_back(std::make_pair(
                    package_name+' '+game_name+' '+node->first_attribute("name")->value()+' '+_games.at(game_name).stringify(game_memory),
                    _games.at(game_name).solve(game_memory)
                ));
            }
            else if(strcmp(node->name(),"package")==0){
                package_name=node->first_attribute("name")->value();
            }
            play_recursive(out, package_name, game_name, node->first_node());
            node=node->next_sibling();
        }
    }
}
