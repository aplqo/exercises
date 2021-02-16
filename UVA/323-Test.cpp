#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>

#include "debug_tools/tester.h"
using namespace std;
using namespace apdebug::tester;
const int maxn = 200;

int p[maxn + 10], d[maxn + 10];
unsigned int sel[maxn + 10], op, od, ansp, ansd;
unsigned int n, m, id = 1;

enum {
  Accept = 0,
  Diff = 1,    // sum mismatch
  Select = 2,  // select people mismatch
  Output = 3,  // output too short
  Order = 4,   // wrong order
  Format = 5
};

void PrintTest(const unsigned int id) { cout << "Test " << id << ": "; }
void ReadData(ifstream& in)
{
  in >> n >> m;
  if (!n && !m) exit(Accept);
  for (unsigned int i = 1; i <= n; ++i)
    in >> p[i] >> d[i];
}
void ReadLine(ifstream& f, char* buf, const unsigned int size)
{
  f.getline(buf, size, '\n');
  if (!f.good()) throw exceptions::ReadFail();
}
void ReadOut(ifstream& out)
{
  char buf[100];
  unsigned int t1, t2;
  out.ignore(numeric_limits<streamsize>::max(), 'J');
  out.seekg(-1, ios::cur);
  ReadLine(out, buf, 50);
  sscanf(buf, "Jury #%d", &t1);
  if (t1 != id) {
    PrintTest(id);
    cout << "Wrong jury number in"
         << " line 1." << endl;
    exit(Format);
  }
  ReadLine(out, buf, 100);
  sscanf(buf,
         "Best jury has value %d for prosecution and value %d for defence:",
         &op, &od);
  for (unsigned int i = 0; i < m; ++i)
    sel[i] = ReadOutput<unsigned int>(out);
}
void ReadAns(ifstream& ans)
{
  char buf[100];
  ans.ignore(numeric_limits<streamsize>::max(), '#');
  ans.ignore(3, '\n');
  ans.getline(buf, 100, '\n');
  sscanf(buf,
         "Best jury has value %d for prosecution and value %d for defence:",
         &ansp, &ansd);
}

void TestSum(const unsigned int id)
{
  if (ansp != op) {
    PrintTest(id);
    cout << "Wrong sum of P[i]. Read=" << op << " Expect=" << ansp << endl;
    exit(Diff);
  }
  if (ansd != od) {
    PrintTest(id);
    cout << "Wrong sum of D[i]. Read=" << od << " Expect=" << ansd << endl;
    exit(Diff);
  }
}
void TestOrder(const unsigned int id)
{
  for (unsigned int i = 1; i < m; ++i)
    if (sel[i] <= sel[i - 1]) {
      PrintTest(id);
      cout << "Wrong order " << sel[i] << " should print before " << sel[i - 1]
           << endl;
      exit(Order);
    }
}
void TestSel(const unsigned int id)
{
  unsigned int ap = 0, ad = 0;
  for (unsigned int i = 0; i < m; ++i) {
    ap += p[sel[i]];
    ad += d[sel[i]];
  }
  if (ap != op) {
    PrintTest(id);
    cout << "Sum of P[i] mismatch. Read=" << op << " Actually=" << ap << endl;
    exit(Select);
  }
  if (ad != od) {
    PrintTest(id);
    cout << "Sum of D[i] mismatch. Read=" << od << " Actually=" << ad << endl;
    exit(Select);
  }
}
int main(int argc, char* argv[])
{
  ifstream in(argv[1]), out(argv[2]), ans(argv[3]);
  while (true) {
    ReadData(in);
    try {
      ReadOut(out);
    }
    catch (exceptions::ReadFail) {
      PrintTest(id);
      cout << "Output too short!" << endl;
      return Output;
    }
    ReadAns(ans);
    TestSum(id);
    TestSel(id);
    TestOrder(id);
    ++id;
  }
  return Accept;
}