#include <cstdio>
using std::printf;
using std::scanf;
int main()
{
  char x;
  scanf("%c", &x);
  if (x % 2) {
    printf("YES");
  }
  else {
    printf("NO");
  }
  return 0;
}
