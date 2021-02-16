#include <cstdio>
using namespace std;

int s[101];
int main()
{
  int n;
  scanf("%d", &n);
  for (int *i = s; i <= s + n; i++)
    scanf("%d", i);
  if (s[0] != 1 && s[0] != -1)
    printf("%dx^%d", s[0], n);
  else if (s[0] == -1)
    printf("-x^%d", n);
  else
    printf("x^%d", n);
  for (int i = 1; i < n; i++) {
    if (s[i] != 1 && s[i] != 0 && s[i] != -1)
      printf("%+dx", s[i]);
    else if (s[i] == 1)
      printf("+x");
    else if (s[i] == -1)
      printf("-x");
    if (i != n - 1 && s[i] != 0) printf("^%d", n - i);
  }
  if (s[n] != 0) printf("%+d", s[n]);
  return 0;
}
