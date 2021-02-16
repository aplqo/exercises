#include <iostream>
#include <string>
using namespace std;
int main()
{
  string ans, orig;
  char typ = '0';
  auto pos = 0;
  cin >> orig;
  for (size_t i = 0; i < orig.size(); i++) {
    switch (orig[i]) {
      case '.':
      case '/':
        typ = orig[i];
        ans.erase(0, ans.find_first_not_of('0'));
        ans.push_back(orig[i]);
        ans.push_back(orig[++i]);
        pos = ans.size() - 1;
        break;
      case '%':
        typ = '%';
        ans.push_back(orig[i]);
        break;
      default:
        ans.insert(pos, 1, orig[i]);
    }
  }
  if (typ == '.')
    ans.erase(ans.find_last_not_of('0') + 1, ans.size());
  else
    ans.erase(pos, ans.find_first_not_of('0', pos) - pos);
  if (orig[0] == '0' && (typ != '0' || orig.size() == 1)) cout << '0';
  cout << ans;
  if (*(ans.end() - 1) == '.') cout << '0';
  return 0;
}
