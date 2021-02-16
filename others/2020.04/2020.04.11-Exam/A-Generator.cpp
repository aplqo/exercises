#include <fstream>
#include <random>
#include <string>
using namespace std;
const int maxn = 1000;

random_device rnd;

unsigned int GetRandom(const unsigned int a, const unsigned int b)
{
  uniform_int_distribution<unsigned int> dis(a, b);
  return dis(rnd);
}

int main(int argc, char* argv[])
{
  ofstream out(argv[2]);
  unsigned int n = stoi(argv[1]);
  if (!n) n = maxn;
  out << GetRandom(1, n) << endl;
  out.close();
  return 0;
}