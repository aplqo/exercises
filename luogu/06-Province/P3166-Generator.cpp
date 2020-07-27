#include "debug_tools/generator.h"
#include <cstring>
#include <fstream>
using namespace std;
using namespace apdebug::generator;
const int maxv = 1000;

int main(int, char* argv[])
{
    Init();
    const unsigned int mv = MayBe(atoi(argv[1]), maxv);
    ofstream(argv[2]) << GetRandom(1u, mv) << " " << GetRandom(1u, mv) << endl;
    return 0;
}