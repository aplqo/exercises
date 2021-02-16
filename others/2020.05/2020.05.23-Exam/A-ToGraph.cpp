#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <tuple>
#include <utility>
using namespace std;
const unsigned int maxn = 100;

enum typ { Unknown = 0, Left = 1, Right = 2 };
enum dir { In = 0 << 1, Out = 1 << 1 };
using point = pair<unsigned int, typ>;
using dpoint = tuple<unsigned int, typ, dir>;
using edge = pair<dpoint, dpoint>;
set<edge> vec;
pair<typ, typ> mat[maxn + 10][maxn + 10];

ostream& operator<<(ostream& os, dpoint s)
{
  os << get<0>(s);
  os << (get<1>(s) == Left ? "L" : "R");
  os << (get<2>(s) == In ? "I" : "O");
  os << " (" << (get<0>(s) * 4 + get<1>(s) + get<2>(s) - 1) << ")";
  return os;
}
void insEdge(const point u, const point v, const dir ud = Out,
             const dir vd = In)
{
  dpoint u1 = make_tuple(u.first, u.second, ud);
  dpoint u2 = make_tuple(v.first, v.second, vd);
  edge&& e = make_pair(u1, u2);
  if (vec.find(e) == vec.end()) vec.insert(e);
}
void addEdge(point l, point r)
{
  static const auto upd = [](typ& t, typ v) {
    if (v != Unknown) t = v;
  };
  if (l.first > r.first) swap(l, r);
  auto& c = mat[l.first][r.first];
  upd(c.first, l.second);
  upd(c.second, r.second);
  if (c.first != Unknown && c.second != Unknown) {
    point u = make_pair(l.first, c.first), v = make_pair(r.first, c.second);
    insEdge(u, v);
    insEdge(v, u);
  }
}
void readEdge(const unsigned int s)
{
  const static auto read = []() -> unsigned int {
    unsigned int i;
    cin >> i;
    return i - 1;
  };
  unsigned int n1, n2;
  cin >> n1 >> n2;
  insEdge(make_pair(s, Left), make_pair(s, Right), In, Out);
  insEdge(make_pair(s, Right), make_pair(s, Left), In, Out);
  for (unsigned int i = 0; i < n1; ++i)
    addEdge(make_pair(s, Left), make_pair(read(), Unknown));
  for (unsigned int j = 0; j < n2; ++j)
    addEdge(make_pair(s, Right), make_pair(read(), Unknown));
}
int main(int argc, char* argv[])
{
  unsigned int n;
  cin >> n;
  for (unsigned int i = 0; i < n; ++i) {
    unsigned int s, l;
    cin >> s >> l;
    readEdge(s - 1);
  }
  ostringstream self;
  for (auto i : vec)
    if (get<0>(i.first) == get<0>(i.second))
      self << i.first << " -> " << i.second << endl;
    else
      cout << i.first << " -> " << i.second << endl;
  cout << endl << "Self edges: " << endl;
  cout << self.str();
  return 0;
}