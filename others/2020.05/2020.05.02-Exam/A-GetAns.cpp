#include <fstream>
using namespace std;
using Number = unsigned long long;

int main(int argc, char* argv[])
{
  ifstream in(argv[1]);
  ofstream ans(argv[2]);
  Number n, res = 0;
  in >> n;
  in.close();
  for (Number i = 1; i <= n; ++i)
    for (Number j = i; j > 0; --j)
      if (i % j == 0 && j % 2 == 1) {
        res += j;
        break;
      }
  ans << res << endl;
  return 0;
}