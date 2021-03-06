#include <unordered_set>

#include "testlib.h"
const int maxn = 500;
const long long maxv = 1e15;
const int dx[] = {0, 0, 1, -1}, dy[] = {1, -1, 0, 0};

long long output[maxn + 10][maxn + 10];

inline long long getMod(const int x1, const int y1, const int x2, const int y2)
{
  return std::max(output[x1][y1], output[x2][y2]) %
         std::min(output[x1][y1], output[x2][y2]);
}
void checkNumber(const int n)
{
  std::unordered_set<long long> s;
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j)
      ouf.ensuref(s.insert(output[i][j]).second,
                  "output[%d][%d]=%lld used twice.", i, j, output[i][j]);
}
long long checkAnswer(const int n)
{
  const long long m = getMod(0, 0, 0, 1);
  ouf.ensuref(m > 0, "m can't be zero");
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j)
      for (int k = 0; k < 4; ++k) {
        const int nx = i + dx[k], ny = j + dy[k];
        if (0 <= nx && 0 <= ny && nx < n && ny < n) {
          const long long v = getMod(i, j, nx, ny);
          ouf.ensuref(v == m,
                      "mod of output[%d][%d] output[%d][%d] = %lld != %lld", i,
                      j, nx, ny, v, m);
        }
      }
  return m;
}

int main(int argc, char* argv[])
{
  registerTestlibCmd(argc, argv);
  const int n = inf.readLong();
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j)
      output[i][j] = ouf.readLong(1ll, maxv);
  checkNumber(n);
  quitf(_ok, "Accepted. m=%lld", checkAnswer(n));
  return 0;
}