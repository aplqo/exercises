#ifndef OUTPUT_H
#define OUTPUT_H

#if __cplusplus < 201103L
#error ISO c++11 is required to use test program!
#endif

#include <iostream>
namespace apdebug_out
{
    using std::ostream;
    /*-----Color-----*/
    enum class col
    {
        NONE = 0,
        RED = 1,
        GREEN = 2,
        YELLOW = 3,
        BLUE = 4,
        PURPLE = 5,
        CYAN = 6
    };
#ifdef COLOR
    const char* colors[] = { "\033[0m", "\033[31m", "\033[32m", "\033[33m",
        "\033[34m", "\033[35m", "\033[36m" };
    ostream& operator<<(ostream& os, col c)
    {
        os << colors[int(c)];
        return os;
    }
#else
    ostream& operator<<(col c, ostream& os)
    {
        return os;
    }
#endif
}
#endif