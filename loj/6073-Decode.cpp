#include <fstream>
#include <limits>

inline void ignore(std::istream& is, const char c)
{
  is.ignore(std::numeric_limits<std::streamsize>::max(), c);
}
inline void copyLine(std::ifstream& in, std::ofstream& out)
{
  in.get(*out.rdbuf(), '\n');
  ignore(in, '\n');
  out.put('\n');
}
void copyTree(const int n, std::ifstream& in, std::ofstream& out)
{
  for (int i = 1; i < n; ++i)
    copyLine(in, out);
}

int main(int, char* argv[])
{
  std::ifstream in(argv[1]), ans(argv[2]);
  std::ofstream out(argv[3]);
  ignore(in, '\n');
  out << "1\n";
  int n, q;
  in >> n >> q;
  ignore(in, '\n');
  out << n << " " << q << "\n";
  copyTree(n, in, out);
  copyLine(in, out);
  long long last = 0;
  for (int i = q; i; --i) {
    long long u, v, k;
    in >> u >> v >> k;
    out << (u ^ last) << " " << (v ^ last) << " " << (k ^ last) << "\n";
    ans >> last;
  }
  return 0;
}