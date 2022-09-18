#include <iostream>
#include <vector>
#include "math-game.hpp"
#include <chrono>

std::string stringify_path(const std::vector<std::string>& path){
    std::stringstream out;
    for(int i = 0; i < path.size(); i++){
        out << '/' << path[i];
    }
    return out.str();
}

int main(int argc, char **argv){
    if(argc<3){
        std::cout << "invalid input params...\n";
        return 1;
    }

    lb::Math_game math_game; math_game.set_state({argv[2]});
    std::vector<std::string> path;

    if(strcmp(argv[1], "run")==0&&math_game.load_xml()==0){
        auto question = math_game.play_all(); std::string input;
        for(const auto& [query, answer] : question){
            //std::cout << query << ' ' << answer << '\n';

            std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
            std::cout << '\n' << query <<"\nYour answer = "; std::cin>>input;
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

            int pos=0;
            for(int i = 0; i < 3; i++){
                pos=query.find(' ', pos+1);
            }

            const std::string data_file = math_game.get_state().folder+'/'+query.substr(0, pos)+".txt";
            const std::string data_query = query.substr(pos+1, query.size()-pos);
            const std::string data_time = std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count()/1000.0);

            int fd = open(data_file.c_str(), O_CREAT|O_WRONLY|O_APPEND, 0666);
            std::string msg = "<data query=\""+data_query+"\" answer=\""+answer+"\" input=\""+input+"\" time=\""+data_time+"\"/>\n";
            write(fd, msg.c_str(), msg.size());
            close(fd);

            if(answer!=input){
                std::cout << "wrong answer! Result=" << answer << '\n';
            }
            std::cout<<"Type Enter to continue... ";
            std::cin.ignore(); std::getline(std::cin, input);
            if(input=="quit"){
                break;
            }
        }
    }
    else if(strcmp(argv[1], "state")==0&&math_game.load_xml()==0){
        bool quit=false;
        std::string input;
        while(!quit){
            std::cout << stringify_path(path) << " input& "; std::cin>>input;
            if(input=="quit"){
                quit=true;
            }
            else if(input=="cd"){
                std::cin>>input;
                int start=0, middle=0;
                while(middle!=std::string::npos){
                    middle=input.find('/', start);
                    std::string folder=input.substr(start, middle-start);
                    if(folder==".."){
                        path.pop_back();
                    }
                    else
                    {
                        path.push_back(input.substr(start, middle-start));
                    }
                    start = middle+1;
                }
            }
            else if(input=="add"){
                std::cin>>input;
                if(input=="package"){
                    std::string package_name, pos;
                    std::cin>>package_name; std::cin>>pos;
                    std::cout << math_game.stringify_error(math_game.add_package(package_name, std::stoi(pos))) << '\n';
                }
                else if(input=="game"){
                    std::string game_name, pos;
                    std::cin>>game_name; std::cin>>pos;
                    std::cout << math_game.stringify_error(math_game.add_game(path[0], game_name, std::stoi(pos))) << '\n';
                }
                else if(input=="mod"){
                    std::string mod_name, pos;
                    std::cin>>mod_name; std::cin>>pos;
                    std::cout << math_game.stringify_error(math_game.add_mod(path[0], path[1], mod_name, std::stoi(pos))) << '\n';
                }
                else if(input=="setting"){
                    std::string name, value;
                    std::cin>>input;
                    while(input!="done"){
                        int equal_pos = input.find('=');
                        name=input.substr(0, equal_pos);
                        value=input.substr(equal_pos+1, input.size()-equal_pos);
                        std::cout << math_game.stringify_error(math_game.add_setting(
                            path[0],
                            path[1],
                            path[2],
                            std::make_pair(name, value)
                        )) << '\n';
                        std::cin>>input;
                    }
                }
            }
            else if(input=="remove"){
                std::cin>>input;
                if(input=="package"){
                    std::string package_name;
                    std::cin>>package_name;
                    std::cout << math_game.stringify_error(math_game.remove_package(package_name)) << '\n';
                }
                else if(input=="game"){
                    std::string game_name;
                    std::cin>>game_name;
                    std::cout << math_game.stringify_error(math_game.remove_game(path[0], game_name)) << '\n';
                }
                else if(input=="mod"){
                    std::string mod_name;
                    std::cin>>mod_name;
                    std::cout << math_game.stringify_error(math_game.remove_mod(path[0], path[1], mod_name)) << '\n';
                }
                else if(input=="setting"){
                    std::cin>>input;
                    while(input!="done"){
                        std::cout << math_game.stringify_error(math_game.remove_setting(
                            path[0],
                            path[1],
                            path[2],
                            input
                        )) << '\n';
                        std::cin>>input;
                    }
                }
            }
            else if(input=="print"){
                std::cin>>input;
                if(input=="all"){
                    std::cout << math_game.stringify_all() << '\n';
                }
                else if(input=="package"){
                    std::string package_name;
                    std::cin>>package_name;
                    std::cout << math_game.stringify_package(package_name) << '\n';
                }
                else if(input=="game"){
                    std::string game_name;
                    std::cin>>game_name;
                    std::cout << math_game.stringify_game(path[0], game_name) << '\n';
                }
                else if(input=="mod"){
                    std::string mod_name;
                    std::cin>>mod_name;
                    std::cout << math_game.stringify_mod(path[0], path[1], mod_name) << '\n';
                }
                else if(input=="setting"){
                    std::cin>>input;
                    while(input!="done"){
                        std::cout << math_game.stringify_setting(
                            path[0],
                            path[1],
                            path[2],
                            input
                        ) << '\n';
                        std::cin>>input;
                    }
                }
            }
            else if(input=="test"){
                std::cin>>input;
                if(input=="all"){
                    auto result = math_game.play_all();
                    for(const auto& [name, value] : result){
                        std::cout << name << ' ' << value << '\n';
                    }
                }
                else if(input=="package"){
                    std::string package_name;
                    std::cin>>package_name;
                    auto result = math_game.play_package(package_name);
                    for(const auto& [name, value] : result){
                        std::cout << name << ' ' << value << '\n';
                    }
                }
                else if(input=="game"){
                    std::string game_name;
                    std::cin>>game_name;
                    auto result = math_game.play_game(path[0], game_name);
                    for(const auto& [name, value] : result){
                        std::cout << name << ' ' << value << '\n';
                    }
                }
                else if(input=="mod"){
                    std::string mod_name;
                    std::cin>>mod_name;
                    auto result = math_game.play_mod(path[0], path[1], mod_name);
                    for(const auto& [name, value] : result){
                        std::cout << name << ' ' << value << '\n';
                    }
                }
            }
            else if(input=="save"){
                math_game.store_xml();
            }
        }
    }
    else if(strcmp(argv[1], "init")==0){
        math_game.init();
        math_game.store_xml();
    }
    else if(strcmp(argv[1], "default")==0){
        
        math_game.init();
        math_game.add_package("Solve", 0);
        math_game.add_game("Solve", "arithmetic", 0);
        math_game.add_mod("Solve", "arithmetic", "easy", 0);
        math_game.add_settings("Solve", "arithmetic", "easy", {
            {"min_terms", "2"},  {"max_terms", "2"},
            {"min_base", "0"},   {"max_base", "10"},
            {"min_exp", "1"},    {"max_exp", "1"},
            {"operations", "+-"}
        });
        math_game.add_mod("Solve", "arithmetic", "medium", 1);
        math_game.add_settings("Solve", "arithmetic", "medium", {
            {"min_terms", "2"},   {"max_terms", "5"},
            {"min_base", "-200"}, {"max_base", "+200"},
            {"min_exp", "1"},     {"max_exp", "1"},
            {"operations", "+-"}
        });
        math_game.add_package("Try", 1);
        math_game.add_game("Try", "doomsday", 0);
        math_game.add_mod("Try", "doomsday", "hard", 0);
        math_game.add_settings("Try", "doomsday", "hard", {
            {"min_year", "1950"}, {"max_year", "2050"},
            {"min_month", "1"}, {"max_month", "12"},
            {"min_day", "1"}, {"max_day", "30"}
        });
        math_game.store_xml();
    }
    else
    {
        std::cout << "invalid input params...\n";
        return 2;
    }
}