#include <algorithm>
#include <fstream>
using namespace std;
const unsigned int maxn = 1e4, maxp = 5000;

unsigned int mat[maxp + 1][maxp + 1];

void init()
{
  for (unsigned int i = 1; i <= maxp; ++i)
    for (unsigned int j = 1; j <= maxp; ++j)
      mat[i][j] += mat[i - 1][j] + mat[i][j - 1] - mat[i - 1][j - 1];
}
unsigned int sum(const unsigned int x1, const unsigned int y1,
                 const unsigned int x2, const unsigned int y2)
{
  return mat[x2][y2] + mat[x1 - 1][y1 - 1] - mat[x1 - 1][y2] - mat[x2][y1 - 1];
}
unsigned int findMax(const unsigned int w, const unsigned int h)
{
  unsigned int ret = 0;
  for (unsigned int i = 1; i <= maxp; ++i)
    for (unsigned int j = 1; j <= maxp; ++j)
      ret = max(ret, sum(i - min(w, i - 1), j - min(h, j - 1), i, j));
  return ret;
}
int main(int, char* argv[])
{
  ifstream in(argv[1]);
  ofstream ans(argv[2]);
  unsigned int n, w, h;
  in >> n >> w >> h;
  for (unsigned int i = 0; i < n; ++i) {
    unsigned int x, y, v;
    in >> x >> y >> v;
    mat[x][y] += v;
  }
  init();
  ans << findMax(w - 1, h - 1) << "\n";
  return 0;
}