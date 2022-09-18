#include "random.hpp"

namespace lb::Rand{
    int genInt(int min, int max){
        return rand()*clock()%(max-min+1)+min;
    }
}
