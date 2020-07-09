#include "debug_tools/generator.h"
#include <cstring>
#include <fstream>
#include <random>
using namespace std;
using apdebug::generator::GetRandom;
using apdebug::generator::Init;
using apdebug::generator::MayBe;
const unsigned long long maxv = 1e15;

int main(int argc, char* argv[])
{
    Init();
    ofstream(argv[2]) << GetRandom(1ull, MayBe(stoull(argv[1]), maxv)) << endl;
    return 0;
}