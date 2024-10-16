#include "../logs/level.hpp"

int main(){
    std::cout<<jl12800::LOG_LEVEL::tostring(jl12800::LOG_LEVEL::value::DEBUG)<<std::endl;
    std::cout<<jl12800::LOG_LEVEL::tostring(jl12800::LOG_LEVEL::value::INFO)<<std::endl;
    std::cout<<jl12800::LOG_LEVEL::tostring(jl12800::LOG_LEVEL::value::WARN)<<std::endl;
    std::cout<<jl12800::LOG_LEVEL::tostring(jl12800::LOG_LEVEL::value::ERROR)<<std::endl;
    std::cout<<jl12800::LOG_LEVEL::tostring(jl12800::LOG_LEVEL::value::FATAL)<<std::endl;


    return 0;
}