#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cctype>
#include <cstring>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxl = 10000;

struct Point {
  int x, y;
  template <class T>
  T& operator[](T dat[3][3]) const
  {
    return dat[x][y];
  }
};
struct Step {
  int dx, dy;
};
constexpr Step colnxt{0, 1}, colprev{0, -1}, rownxt{1, 0}, rowprev{-1, 0};

unsigned int plane[6][3][3];
unsigned int *line[3][3][12], *ring[6][9];
char str[maxl + 10];

inline Point operator+(const Point a, const Step s)
{
  return Point{a.x + s.dx, a.y + s.dy};
}
void linePointer(unsigned int* ptr[], const unsigned int pla, Point p,
                 const Step ini)
{
  for (unsigned int i = 0; i < 3; ++i) {
    ptr[i] = &p[plane[pla]];
    p = p + ini;
  }
}
static void initAL(const unsigned int p)
{
  static constexpr unsigned int id[] = {5, 4, 2, 0};
  unsigned int** pl = line[0][p];
  for (unsigned int i : id) {
    linePointer(pl, i, Point{2, p}, rowprev);
    pl += 3;
  }
}
static void initDL(const int p)
{
  unsigned int** pl = line[1][p];
  linePointer(pl, 3, Point{2, p}, rowprev);
  pl += 3;
  linePointer(pl, 0, Point{2 - p, 2}, colprev);
  pl += 3;
  linePointer(pl, 1, Point{0, 2 - p}, rownxt);
  pl += 3;
  linePointer(pl, 4, Point{p, 0}, colnxt);
}
static void initHL(const int p)
{
  static constexpr unsigned int id[] = {1, 2, 3};
  unsigned int** pl = line[2][p];
  for (unsigned int i : id) {
    linePointer(pl, i, Point{p, 0}, colnxt);
    pl += 3;
  }
  linePointer(pl, 5, Point{2 - p, 2}, colprev);
}
static void initRing(const unsigned int p)
{
  unsigned int** pl = ring[p];
  linePointer(pl, p, Point{0, 0}, colnxt);
  pl += 2;
  linePointer(pl, p, Point{0, 2}, rownxt);
  pl += 2;
  linePointer(pl, p, Point{2, 2}, colprev);
  pl += 2;
  linePointer(pl, p, Point{2, 0}, rowprev);
}
void rotate(unsigned int* const* const ptr, const unsigned int len,
            const unsigned int offset)
{
  static unsigned int buf[30];
  for (unsigned int i = 0; i < len; ++i)
    buf[i] = *ptr[i];
  for (unsigned int i = len - 1; i >= offset; --i)
    *ptr[i] = *ptr[i - offset];
  for (unsigned int i = 0, bas = len - offset; i < offset; ++i, ++bas)
    *ptr[i] = buf[bas];
}

inline void printPtrs(unsigned int* const* const pl, const unsigned int len)
{
  for (unsigned int i = 0; i < len; ++i)
    clog << *pl[i] << " ";
  clog.put('\n');
}

inline void rotateLeft(unsigned int* const* const pl, const unsigned int len,
                       const unsigned int off)
{
  rotate(pl, len, len - off);
}
inline void roteteRight(unsigned int* const* const pl, const unsigned int len,
                        const unsigned int off)
{
  rotate(pl, len, off);
}

void readMatrix(unsigned int res[3][3])
{
  for (unsigned int i = 0; i < 3; ++i)
    copy_n(istream_iterator<unsigned int>(cin), 3, res[i]);
}
void readState()
{
  readMatrix(plane[0]);
  for (unsigned int i = 0; i < 3; ++i) {
    static constexpr unsigned int id[] = {1, 2, 3};
    for (unsigned int d : id)
      for (unsigned int j = 0; j < 3; ++j)
        cin >> plane[d][i][j];
  }
  readMatrix(plane[4]);
  readMatrix(plane[5]);
}
void writeMatrix(ostream& os, unsigned int res[3][3])
{
  for (unsigned int i = 0; i < 3; ++i) {
    copy(res[i], res[i] + 3, ostream_iterator<unsigned int>(os, " "));
    os.put('\n');
  }
}
void writeState(ostream& os)
{
  writeMatrix(os, plane[0]);
  for (unsigned int i = 0; i < 3; ++i) {
    static constexpr unsigned int id[] = {1, 2, 3};
    for (unsigned int d : id)
      for (unsigned int j = 0; j < 3; ++j)
        os << plane[d][i][j] << " ";
    os.put('\n');
  }
  writeMatrix(os, plane[4]);
  writeMatrix(os, plane[5]);
}
static void init()
{
  for (unsigned int i = 0; i < 3; ++i) {
    initAL(i);
    initDL(i);
    initHL(i);
  }
  for (unsigned int i = 0; i < 6; ++i)
    initRing(i);
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  readState();
  init();
  cin >> str;
  const size_t len = strlen(str);
  for (size_t i = 0; i < len; ++i) {
    static constexpr unsigned int rtl[] = {1, 2, 0}, rtr[] = {3, 5, 4};
    auto rotl = rotateLeft, rotr = roteteRight;
    if (isupper(str[i])) swap(rotl, rotr);
    str[i] = tolower(str[i]);
    const unsigned int typ = str[i] - 'a' - (str[i] > 'g'), tp = typ / 3;
    rotr(line[tp][typ % 3], 12, 3);
    if (typ % 3 == 0)
      rotl(ring[rtl[tp]], 8, 2);
    else if (typ % 3 == 2)
      rotr(ring[rtr[tp]], 8, 2);
  }
  writeState(cout);
  return 0;
}