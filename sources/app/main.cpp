
#include "CEngine.hpp"

#include <iostream>


int main()
{
    try
    {
        CEngine engine;
        engine.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
    }
    return 0;
}
