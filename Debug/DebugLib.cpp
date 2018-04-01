#include "DebugLib.h"

int PrintInfo(const char* msg)
{
    std::cerr << YELLOW;
    std::cerr << msg;
    std::cerr << DEFAULT;
}

int PrintErr(const char* msg)
{
    std::cerr << RED;
    std::cerr << msg;
    std::cerr << DEFAULT;
}
