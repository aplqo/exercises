#include <fstream>
#include <iostream>
using namespace std;
constexpr unsigned int modx = 39989, mody = 1e9;

inline unsigned int decodex(const unsigned long long x, const unsigned int lans)
{
  return (x + lans + modx - 1) % modx + 1;
}
inline unsigned int decodey(const unsigned long long y, const unsigned int lans)
{
  return (y + lans + mody - 1) % mody + 1;
}
int main(int argc, char* argv[])
{
  ifstream fin(argv[1]), fans(argv[2]);
  ofstream fout;
  if (argc > 3) fout.open(argv[3]);
  ostream& out = argc > 3 ? fout : cout;
  unsigned int n, lst = 0;
  fin >> n;
  out << n << "\n";
  for (unsigned int i = 0; i < n; ++i) {
    unsigned int op;
    fin >> op;
    out << op << " ";
    if (op == 0) {
      unsigned int x;
      fin >> x;
      out << decodex(x, lst) << "\n";
      fans >> lst;
    }
    else {
      unsigned int x1, y1, x2, y2;
      fin >> x1 >> y1 >> x2 >> y2;
      out << decodex(x1, lst) << " " << decodey(y1, lst) << " "
          << decodex(x2, lst) << " " << decodey(y2, lst) << "\n";
    }
  }
  return 0;
}