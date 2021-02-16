#include <filesystem>
#include <fstream>
#include <string>
using namespace std;

void create(const char* str)
{
  using namespace std::filesystem;
  path p(str);
  create_directory(p.filename().replace_extension());
  current_path(p.filename().replace_extension());
}
int main(int argc, char* argv[])
{
  string s(argv[1]);
  ifstream in(s + ".in"), ans(s + ".ans");
  create(argv[1]);
  unsigned int t;
  in >> t;
  for (unsigned int i = 0; i < t; ++i) {
    ofstream outin(to_string(i) + ".in"), outans(to_string(i) + ".ans");
    unsigned int n, m, ret;
    in >> n >> m;
    outin << 1 << endl;
    outin << n << " " << m << endl;
    ans >> ret;
    outans << ret << endl;
    for (unsigned int j = 0; j < n; ++j) {
      float a, b;
      in >> a >> b;
      outin << a << " " << b << endl;
    }
    outin.close();
    outans.close();
  }
  return 0;
}