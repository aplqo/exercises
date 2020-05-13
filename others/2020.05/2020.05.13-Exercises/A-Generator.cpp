#include "debug_tools/generator.h"
#include <climits>
#include <fstream>
#include <random>
using namespace std;
using namespace apdebug::generator;
const unsigned int maxp = (1u << 31) - 1;

mt19937_64 rnd;

bool isPrime(const unsigned int p) {
  if (p < 3)
    return p == 2;
  for (unsigned int i = 2; i * i <= p; ++i)
    if (p % i == 0)
      return false;
  return true;
}
unsigned int genPrime(const unsigned int mp) {
  uniform_int_distribution<unsigned int> dis(1, mp);
  unsigned int ret = dis(rnd);
  while (!isPrime(ret))
    ret = dis(rnd);
  return ret;
}

int main(int argc, char *argv[]) {
  ofstream in(argv[2]);
  rnd.seed(random_device()());
  const unsigned int mp = MayBe(stoi(argv[1]) * 1u, maxp);
  in << genPrime(mp) << " " << genPrime(mp) << endl;
  return 0;
}