#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <utility>
using namespace std;
using std::filesystem::create_directory;
using std::filesystem::path;

struct query {
  unsigned int l, r;
};
istream& operator>>(istream& is, query& q)
{
  is >> q.l >> q.r;
  return is;
}
ostream& operator<<(ostream& os, const query v)
{
  os << v.l << " " << v.r;
  return os;
}

template <class T>
inline void copyVal(ifstream& src, size_t n, ofstream& dest, const char* delm)
{
  copy_n(istream_iterator<T>(src), n, ostream_iterator<T>(dest, delm));
}
unsigned int copyInput(ifstream& src, const path out)
{
  ofstream oin(out);
  unsigned int n, q;
  src >> n;
  oin << 1 << endl;
  oin << n << endl;
  copyVal<unsigned int>(src, n, oin, " ");
  src >> q;
  oin << endl << q << endl;
  copyVal<query>(src, q, oin, "\n");
  return q;
}
void copyAnswer(ifstream& src, const unsigned int q, const path out)
{
  ofstream fans(out);
  copyVal<unsigned long long>(src, q, fans, "\n");
}

int main(int argc, char* argv[])
{
  ifstream in(argv[1]);
  const bool haveAns = strcmp(argv[2], "*");
  ifstream fans(argv[2]);
  const path d(argv[3]);
  create_directory(d);
  unsigned int t;
  in >> t;
  for (unsigned int i = 0; i < t; ++i) {
    const static auto repExt = [](const path d, unsigned int i,
                                  const char* ext) {
      return (d / to_string(i)).replace_extension(ext);
    };
    const unsigned int q = copyInput(in, repExt(d, i, ".in"));
    if (haveAns) copyAnswer(fans, q, repExt(d, i, ".ans"));
  }
  return 0;
}