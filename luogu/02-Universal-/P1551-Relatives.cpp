#include <iostream>
using namespace std;

int fa[5000];

int GetFather(int x) { return fa[x] == x ? x : fa[x] = GetFather(fa[x]); }
int main()
{
  int n, m, p;
  cin >> n >> m >> p;
  for (int i = 0; i <= n; i++)
    fa[i] = i;
  for (int i = 0; i < m; i++) {
    int a, b;
    cin >> a >> b;
    fa[GetFather(a)] = GetFather(b);
  }
  for (int i = 0; i < p; i++) {
    int a, b;
    cin >> a >> b;
    if (GetFather(a) == GetFather(b))
      cout << "Yes" << endl;
    else
      cout << "No" << endl;
  }
  return 0;
}