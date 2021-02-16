#ifdef APDEBUG
#define COLOR
#define APINPROG
#include "debug_tools/time.cpp"
#endif
#include <iostream>
using namespace std;
typedef long long num;
const int maxn = 1e6 + 5;
#define lowbit(x) ((x) & -(x))

num tree[maxn];
int n;

void update(int x, num val)
{
  for (; x <= n; x += lowbit(x))
    tree[x] += val;
}
num query(int x)
{
  num ret = 0;
  for (; x; x -= lowbit(x))
    ret += tree[x];
  return ret;
}

int main()
{
  ios_base::sync_with_stdio(false);
  int q;
  cin >> n >> q;
  for (int i = 1; i <= n; ++i) {
    num v;
    cin >> v;
    update(i, v);
  }
  for (int i = 0; i < q; ++i) {
    char op;
    int x;
    cin >> op >> x;
    switch (op) {
      case '1':
        num v;
        cin >> v;
        update(x, v);
        break;
      case '2':
        int y;
        cin >> y;
        cout << query(y) - query(x - 1) << endl;
        break;
    }
  }
  return 0;
}