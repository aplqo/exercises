#include <cstdio>
using std::printf;
using std::scanf;
int main()
{
  float month = 0, t;
  for (int i = 0; i < 12; i++) {
    scanf("%f", &t);
    month += t;
  }
  month /= 12;
  printf("$%.2f", month);
  return 0;
}
