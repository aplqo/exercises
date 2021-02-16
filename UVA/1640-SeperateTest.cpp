#include <filesystem>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char* argv[])
{
  ifstream in(argv[1]), ans(argv[2]);
  const std::filesystem::path d(argv[3]);
  std::filesystem::create_directory(d);
  unsigned int cnt = 0;
  while (true) {
    unsigned long long a, b;
    in >> a >> b;
    if (!a && !b) break;
    const string s = to_string(cnt);
    {
      ofstream oin(d / (s + ".in"));
      oin << a << " " << b << endl;
      oin << "0 0" << endl;
    }
    {
      ofstream oans(d / (s + ".ans"));
      string t;
      getline(ans, t);
      oans << t << endl;
    }
    ++cnt;
  }
  return 0;
}