#include <cstring>
#include <fstream>
#include <random>
using namespace std;

mt19937_64 rnd;
uniform_int_distribution<int> gen(-1e4, 1e4);
ofstream out;

int main(int argc, char* argv[])
{
  rnd.seed(random_device()());
  unsigned int n = atoi(argv[1]),
               m = uniform_int_distribution<unsigned int>(0, n)(rnd);
  out.open(argv[2]);
  out << n << " " << m << endl;
  for (unsigned int i = 0; i < n; ++i)
    out << gen(rnd) << " ";
  out << endl;
  out.close();
  return 0;
}