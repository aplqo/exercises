#include <cstdio>
using namespace std;
int main()
{
  constexpr float prices[] = {28.9, 32.7, 45.6, 78, 35, 86.2, 27.8, 43, 56, 65};
  float ans = 0;
  int t;
  for (int i = 0; i < 10; i++) {
    scanf("%d", &t);
    ans += prices[i] * t;
  }
  printf("%.1f", ans);
  return 0;
}
