#include <algorithm>
#include <filesystem>
#include <fstream>

void changeDir(const char* pat)
{
  namespace fs = std::filesystem;
  const fs::path p(pat);
  if (!fs::exists(p)) fs::create_directory(p);
  fs::current_path(p);
}
int main(int, char* argv[])
{
  std::ifstream fin(argv[1]), fans(argv[2]);
  changeDir(argv[4]);
  unsigned int t;
  fin >> t;
  fin.ignore(5, '\n');
  for (unsigned int i = 0; i < t; ++i) {
    std::ofstream cin(argv[3] + std::to_string(i) + ".in"),
        cans(argv[3] + std::to_string(i) + ".ans");
    cin << "1\n";
    fin.get(*cin.rdbuf(), '\n');
    fin.ignore(5, '\n');
    cin.put('\n');
    fans.get(*cans.rdbuf(), '\n');
    fans.ignore(5, '\n');
    cans.put('\n');
  }
  return 0;
}