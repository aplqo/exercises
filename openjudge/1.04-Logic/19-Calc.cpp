#include <iostream>
using std::cin;
using std::cout;

int main()
{
  int a, b, result = 0;
  char op;
  cin >> a >> b >> op;
  switch (op) {
    case '+':
      result = a + b;
      break;
    case '-':
      result = a - b;
      break;
    case '*':
      result = a * b;
      break;
    case '/':
      if (b == 0) {
        cout << "Divided by zero!";
        return 0;
      }
      result = a / b;
      break;
    default:
      cout << "Invalid operator!";
      return 0;
  }
  cout << result;
  return 0;
}
