#include <cstdio>
using std::printf;
using std::scanf;
int main()
{
  long long n;
  scanf("%d", &n);
  while (n != 1) {
    if (n % 2) {
      printf("%ld*3+1=", n);
      n = n * 3 + 1;
    }
    else {
      printf("%ld/2=", n);
      n /= 2;
    }
    printf("%ld\n", n);
  }
  printf("End");
  return 0;
}
