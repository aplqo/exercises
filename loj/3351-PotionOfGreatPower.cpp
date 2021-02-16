#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <iterator>
#include <set>
#include <vector>
using namespace std;
const unsigned int maxn = 1e5, maxd = 500, maxu = 2e5;
const unsigned int blockSize = 50;
constexpr int inf = INT_MAX / 2, ninf = INT_MIN / 2;

int h[maxn + 10];
unsigned int n;
struct HLess {
  inline bool operator()(const unsigned int l, const unsigned int r) const
  {
    return h[l] != h[r] ? h[l] < h[r] : l < r;
  }
};
template <class T>
struct DynArray {
  inline DynArray() : size(0), ptr(nullptr){};
  inline ~DynArray()
  {
    delete[] ptr;
    ptr = nullptr;
  }
  inline void allocate(const unsigned int s)
  {
    size = s;
    ptr = new T[s];
  }
  DynArray(DynArray&& r) = delete;
  DynArray(const DynArray&) = delete;
  DynArray& operator=(const DynArray&) = delete;
  DynArray& operator=(DynArray&&) = delete;

  unsigned int size;
  T* ptr;
};
template <class T, unsigned int siz>
struct Array {
  template <class U>
  inline void push_back(const U val)
  {
    *(ptr++) = T(val);
  }
  inline void pop_back() { --ptr; }
  inline void clear() { ptr = val; }
  inline unsigned int size() const { return ptr - val; }

  T val[siz + 1], *ptr = val;
};
typedef Array<int, maxn + 10> VertexList;

class Neighbor {
 public:
  inline void addChange(const unsigned int time, const unsigned int v);
  void build();
  void get(const unsigned int time, VertexList& buf) const;

 private:
  struct Operate {
    enum { Insert, Remove, Unknown } type;
    unsigned int time, val;
    inline bool operator<(const Operate& r) const
    {
      if (h[val] != h[r.val])
        return h[val] < h[r.val];
      else if (val != r.val)
        return val < r.val;
      else
        return type == Insert ? r.type != Insert : false;
    }
  };
  struct Checkpoint {
    unsigned int time;
    DynArray<unsigned int> neigh;
  };
  vector<Operate> oper;
  DynArray<Checkpoint> chk;
} gra[maxn + 10];

template <class T>
inline bool tLess(const unsigned int tim, const T& v)
{
  return tim < v.time;
}
inline void Neighbor::addChange(const unsigned int time, const unsigned int v)
{
  oper.push_back(Operate{Operate::Unknown, time, v});
}
void Neighbor::build()
{
  static unsigned int vis[maxn + 10], sta;
  chk.allocate((oper.size() + blockSize - 1) / blockSize + 1);
  chk.ptr[0].time = 0;
  unsigned int cnt = 0;
  set<unsigned int, HLess> st;
  Checkpoint* cur = chk.ptr + 1;
  ++sta;
  for (auto it = oper.begin(); it != oper.end(); ++it) {
    if (cnt >= blockSize) {
      cur->neigh.allocate(st.size());
      copy(st.cbegin(), st.cend(), cur->neigh.ptr);
      ++cur;
      cnt = 0;
    }
    if (vis[it->val] == sta) {
      st.erase(it->val);
      vis[it->val] = 0;
      it->type = Operate::Remove;
    }
    else {
      st.insert(it->val);
      vis[it->val] = sta;
      it->type = Operate::Insert;
    }
    cur->time = it->time;
    ++cnt;
  }
  if (cnt) {
    cur->neigh.allocate(st.size());
    copy(st.cbegin(), st.cend(), cur->neigh.ptr);
  }
}
void Neighbor::get(const unsigned int time, VertexList& buf) const
{
  static Operate tmp[maxn + 10];
  static HLess cmp;
  const Checkpoint* const ck =
      upper_bound(chk.ptr, chk.ptr + chk.size, time, tLess<Checkpoint>) - 1;
  const auto& clist = ck->neigh;
  Operate* ed;
  {
    const auto oit =
        upper_bound(oper.cbegin(), oper.cend(), ck->time, tLess<Operate>);
    const auto oed =
        upper_bound(oper.cbegin(), oper.cend(), time, tLess<Operate>);
    ed = copy(oit, oed, tmp);
    sort(tmp, ed);
  }
  const Operate* optr = tmp;
  const unsigned int* vptr = clist.ptr;
  while (optr < ed || vptr < clist.ptr + clist.size) {
    if (optr == ed ||
        (vptr != clist.ptr + clist.size && !cmp(optr->val, *vptr)))
      buf.push_back(h[*(vptr++)]);
    else {
      if (optr->type == Operate::Insert)
        buf.push_back(h[optr->val]);
      else
        buf.pop_back();
      ++optr;
    }
  }
}

void init(int N, int, int H[])
{
  n = N;
  copy(H, H + n, h);
}
void curseChanges(int u, int a[], int b[])
{
  for (unsigned int i = 1; i <= u; ++i) {
    const unsigned int v1 = a[i - 1], v2 = b[i - 1];
    gra[v1].addChange(i, v2);
    gra[v2].addChange(i, v1);
  }
  for (Neighbor* i = gra; i < gra + n; ++i)
    i->build();
}

int question(int x, int y, int v)
{
  static VertexList vl[2];
  int ret = 1e9;
  vl[0].clear();
  gra[x].get(v, vl[0]);
  vl[1].clear();
  gra[y].get(v, vl[1]);
  const int* it[2] = {vl[0].val, vl[1].val};
  int lst[2] = {ninf, ninf};
  const unsigned int sz = vl[0].size() + vl[1].size();
  for (unsigned int i = 0; i < sz; ++i) {
    const unsigned int typ =
        it[0] == vl[0].ptr || (it[1] != vl[1].ptr && *it[0] > *it[1]);
    lst[typ] = *(it[typ]++);
    ret = min(ret, lst[typ] - lst[!typ]);
    if (it[!typ] != vl[!typ].ptr) ret = min(ret, *it[!typ] - lst[typ]);
  }
  return ret;
}

int main(int, char* argv[])
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int d, u, q;
  cin >> n >> d >> u >> q;
  copy_n(istream_iterator<int>(cin), n, h);
  {
    static int a[maxu + 10], b[maxu + 10];
    for (unsigned int i = 0; i < u; ++i)
      cin >> a[i] >> b[i];
    curseChanges(u, a, b);
  }
  for (unsigned int i = 0; i < q; ++i) {
    unsigned int x, y, v;
    cin >> x >> y >> v;
    cout << question(x, y, v) << endl;
  }
  return 0;
}
