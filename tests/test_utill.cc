#include "../logs/utill.hpp"

int main()
{
    std::cout << jl12800::utill::date::now() << std::endl;
    std::string pathname="./aaa/bbb/c.txt";
    jl12800::utill::file::creat_directory(jl12800::utill::file::filepath(pathname));
    
    return 0;
}