#include "../logs/level.hpp"

int main(){
    std::cout<<jl12800::LOGLEVEL::tostring(jl12800::LOGLEVEL::value::DEBUG)<<std::endl;
    std::cout<<jl12800::LOGLEVEL::tostring(jl12800::LOGLEVEL::value::INFO)<<std::endl;
    std::cout<<jl12800::LOGLEVEL::tostring(jl12800::LOGLEVEL::value::WARN)<<std::endl;
    std::cout<<jl12800::LOGLEVEL::tostring(jl12800::LOGLEVEL::value::ERROR)<<std::endl;
    std::cout<<jl12800::LOGLEVEL::tostring(jl12800::LOGLEVEL::value::FATAL)<<std::endl;


    return 0;
}