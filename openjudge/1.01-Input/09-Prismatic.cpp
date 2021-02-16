#include <iostream>
using std::cin;
using std::cout;
using std::endl;

constexpr unsigned int width = 5;
constexpr unsigned int graph[] = {1, 3, 5, 3, 1};

void put(const char c, const unsigned int num)
{
  auto putNum = [](const char c, const unsigned int num) {
    for (unsigned int i = 0; i < num; i++) {
      cout << c;
    }
  };
  unsigned int blank = (width - num) / 2;
  putNum(' ', blank);
  putNum(c, num);
  putNum(' ', blank);
  cout << endl;
}
int main()
{
  char c;
  cin >> c;
  for (const auto i : graph) {
    put(c, i);
  }
  cout.flush();
  return 0;
}
