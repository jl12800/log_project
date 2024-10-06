#include "../logs/utill.hpp"

int main()
{
    std::cout << jl12800::utill::DATE::now() << std::endl;
    std::string pathname="./aaa/bbb/c.txt";
    jl12800::utill::FILE::creat_directory(jl12800::utill::FILE::filepath(pathname));
    
    return 0;
}