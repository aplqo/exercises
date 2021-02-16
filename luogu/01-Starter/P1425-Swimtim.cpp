#include <iostream>
using std::cin;
using std::cout;

namespace mycode {

class time {
 public:
  unsigned int hour;
  unsigned int min;
};
time duration(const time t1, const time t2)
{
  time result;
  result.hour = t2.hour - t1.hour;
  if (t2.min < t1.min) {
    result.min = t2.min + 60 - t1.min;
    result.hour--;
  }
  else {
    result.min = t2.min - t1.min;
  }
  return result;
}
}  // namespace mycode
int main()
{
  mycode::time t1, t2, result;
  cin >> t1.hour >> t1.min;
  cin >> t2.hour >> t2.min;
  result = mycode::duration(t1, t2);
  cout << result.hour << " " << result.min;
  cout.flush();
  return 0;
}
