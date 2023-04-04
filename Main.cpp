#include <iostream>

#include "STLmesh.h"

int main()
{
    convertSTL("cube.stl");
    std::cout << "Finished" << std::endl;
    return 0;
}