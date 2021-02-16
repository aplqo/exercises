#include <cstdlib>
#include <iostream>
#include <random>
#include <string>
using namespace std;

random_device rd;
int n;
string pre, sn;
int main(int argc, char* argv[])
{
  if (argc > 2)
    n = atoi(argv[2]);
  else
    n = 1000000;
  pre = argv[1];
  sn = to_string(n) + " ";

  uniform_int_distribution<int> src(1, n);
  for (int i = 1; i <= n; i = src(rd)) {
    string in = pre + ".in" + " ", out = pre + ".out" + " ",
           ans = pre + ".ans" + " ";
    string gen =
        string("5102-Generator.exe") + " " + in + ans + sn + to_string(i);
    system(gen.c_str());
    string cmd = string("5102-Prawnicy.exe") + " " + "-in " + in + "-out " +
                 out + " " + "-time 10000";
    int ret = system(cmd.c_str());
    if (ret) {
      cout << "Failed data: n=" << n << " k=" << i << endl;
      return 1;
    }
  }
  return 0;
}