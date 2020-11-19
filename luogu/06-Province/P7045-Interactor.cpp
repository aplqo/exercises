#include "debug_tools/tester.h"
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iterator>
using namespace std;
using namespace apdebug::tester;
const unsigned int maxn = 5e4, maxbuf = 1000;

ifstream input;
ofstream result, err;
FILE *proin, *proout;
unsigned int type[maxn + 10];

void release()
{
    fclose(proin);
    fclose(proout);
}

template <class... Args>
inline void print(const unsigned int id, Args... args)
{
    print(err, "Test ", id, ": ", forward<Args>(args)...);
}
template <class... Args>
inline void fail(const unsigned int id, Args... args)
{
    print(id, forward<Args>(args)...);
    exit(1);
}
void ignoreEnd(FILE* f)
{
    char ch = fgetc(f);
    while (!isgraph(ch))
        ch = fgetc(f);
    ungetc(ch, f);
}

void doInteract(const unsigned int id)
{
    unsigned int n, q, cnt = 0;
    input >> n >> q;
    copy_n(istream_iterator<unsigned int>(input), n, type);
    fprintf(proin, "%u %u\n", n, q);
    fflush(proin);
    while (true)
    {
        static char str[maxbuf + 10];
        int a, b;
        ignoreEnd(proout);
        fgets(str, maxbuf, proout);
        const unsigned int t = sscanf(str, "%d%d", &a, &b);
        if (t == 2)
        {
            if (q == cnt)
                fail(id, "Interact times exceed limit!");
            if (a < 0 || b < 0 || a >= n || b >= n)
                fail(id, "Ineract value out of range. Expect=[", 0, ",", n - 1, "]. Read a=", a, " b=", b);
            ++cnt;
            fputs(type[a] == type[b] ? "0\n" : "1\n", proin);
            fflush(proin);
        }
        else if (t == 1)
        {
            if (a == -1)
            {
                result << "N\n";
                break;
            }
            if (a != n)
                fail(id, "Invalid output! Expected n=", n, " read=\"", a, "\"");
            result << "Y\n";
            for (unsigned int i = 0; i < n; ++i)
            {
                unsigned int t;
                if (!fscanf(proout, "%u", &t))
                    fail(id, "Invalid output! Error occured while reading permutation. Read=\"", move(str), "\n");
                result << t << " ";
            }
            result.put('\n');
            break;
        }
        else
            fail(id, "Unkown output \"", move(str), "\"");
    }
    print(id, "Call function ", cnt, " times.");
}

int main(int argc, char* argv[])
{
    input.open(argv[1]);
    proin = fopen(argv[2], "w");
    proout = fopen(argv[3], "r");
    result.open(argv[4]);
    err.open(argv[5]);
    unsigned int t;
    input >> t;
    fprintf(proin, "%u\n", t);
    fflush(proin);
    for (unsigned int i = 0; i < t; ++i)
        doInteract(i);
    release();
    return 0;
}