#include <algorithm>
#include <fstream>
#include <vector>
using namespace std;

vector<unsigned int> val;
int main(int, char* argv[])
{
  ifstream in(argv[1]);
  ofstream ans(argv[2]);
  unsigned int n, k, p, lst = 0;
  in >> n >> k >> p;
  for (unsigned int i = 0; i < n; ++i) {
    unsigned int op;
    in >> op;
    if (op == 1) {
      unsigned int x;
      in >> x;
      if (p) x ^= lst;
      val.insert(upper_bound(val.begin(), val.end(), x), x);
    }
    else {
      ans << (lst = val[val.size() - k]) << "\n";
      val.erase(val.end() - k);
    }
  }
  return 0;
}