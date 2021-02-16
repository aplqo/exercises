#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
using namespace std;
typedef int num;
typedef uniform_int_distribution<num> source;
const int maxl = 1e9 + 1, maxn = 1000010;

random_device rd;
ofstream in, ans;
struct range {
  num l;
  num r;
} dat[maxn], res;
int k, n;

num getNum(num l, num r)
{
  source rnd(l, r);
  return rnd(rd);
}
void getRangeL(num l, num r, range dat[], int n)
{
  source src(l, r);
  for (int i = 0; i < n; ++i)
    dat[i].l = src(rd);
}
void getRangeR(num l, num r, range dat[], int n)
{
  source src(l, r);
  for (int i = 0; i < n; ++i) {
    dat[i].r = src(rd);
    if (res.r <= res.l) --i;
  }
}
int main(int argc, char* argv[])
{
  in.open(argv[1]);
  ans.open(argv[2]);
  if (argc < 3)
    cin >> n >> k;
  else {
    n = atoi(argv[3]);
    k = atoi(argv[4]);
  }

  res.l = getNum(0, maxn);
  res.r = getNum(res.l + 1, maxn);

  getRangeL(0, res.l, dat, k);
  getRangeR(res.r, maxn, dat, k);
  {
    num ln = getNum(0, n - k);
    getRangeL(0, res.r, dat + k, ln);
    getRangeR(0, res.r, dat + k, ln);
    getRangeL(res.l + 1, maxn, dat + k + ln, n - k - ln);
    getRangeR(res.l + 1, maxn, dat + k + ln, n - k - ln);
  }
  shuffle(dat, dat + n, rd);

  in << n << " " << k << endl;
  for (int i = 0; i < n; ++i)
    in << dat[i].l << " " << dat[i].r << endl;
  in.close();
  ans << (res.r - res.l) << endl;
  ans.close();

  return 0;
}