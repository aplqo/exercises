#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "debug_tools/tester.h"
using namespace std;
using namespace apdebug::tester;
const unsigned int maxn = 5e4;

enum RStatus {
  Accept = 0,
  Empty = 1,
  Type = 2,
  Constraint = 3,
  Output = 4,
};
unsigned int n;
string res;

void testEmpty(ifstream& out, const char* fans)
{
  ifstream ans(fans);
  if (out.peek() == '-' || ans.peek() == '-') {
    const bool ov = out.peek() == '-', av = ans.peek() == '-';
    if (ov == av) exit(Accept);
    if (ov) {
      print(cout, "Fail: Read -1, expect not -1.");
      exit(Empty);
    }
    if (av) {
      print(cout, "Fail: Reat not -1, expect -1.");
      exit(Empty);
    }
  }
}
void testNeq(ifstream& in)
{
  static char neq[maxn + 100];
  in >> neq;
  for (unsigned int i = 0; i < n; ++i)
    if (toupper(neq[i]) == res[i]) {
      print(cout, "Fail: Select forbidden car on ", i, ". Type=", neq[i]);
      exit(Type);
    }
}
void testConstraint(ifstream& in)
{
  unsigned int m;
  in >> m;
  for (unsigned int i = 0; i < m; ++i) {
    char hf, ht;
    unsigned int f, t;
    in >> f >> hf >> t >> ht;
    --f;
    --t;
    if (res[f] == hf && res[t] != ht) {
      print(cout, "Fail: Constraint ", i, " (", f, ":", hf, "->", t, ":", ht,
            ") not satified.Read ", t, "=", res[t]);
      exit(Constraint);
    }
  }
}

int main(int, char* argv[])
{
  ifstream in(argv[1]), out(argv[2]);
  testEmpty(out, argv[3]);
  try {
    out >> res;
  }
  catch (exceptions::ReadFail&) {
    print(cout, "Fail: Invalid output");
    return Output;
  }
  unsigned int d;
  in >> n >> d;
  if (res.size() < n) {
    print(cout, "Fail: Output too short. Expect=", n, " Read=", res.size());
    return Output;
  }
  testNeq(in);
  testConstraint(in);
  return Accept;
}