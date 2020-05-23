#include <atomic>
#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
using namespace std;
const chrono::milliseconds printDur(200);
const unsigned int maxn = 1e7;

atomic_uint cur;
unsigned int val[maxn + 10], en;

void print()
{
    while (cur <= en)
    {
        cout << "\rGenerate " << cur.load() << " data...";
        cout.flush();
        this_thread::sleep_for(printDur);
    }
}
unsigned int fun(const unsigned int n)
{
    unsigned int ret = 0;
    unsigned int lst = 1;
    for (unsigned int i = 2; i <= n + 1; ++i)
    {
        const unsigned int cur = gcd(i, n);
        if (cur == 1 && lst == 1)
            ++ret;
        lst = cur;
    }
    return ret;
}
void solve()
{
    while (cur.load() <= en)
    {
        unsigned int v = cur.fetch_add(1);
        val[v] = fun(v);
    }
}
void write(const char* f, const char* head, const char* tail)
{
    ofstream out(f);
    for (unsigned int i = 0; i < en; ++i)
        out << val[i] << ",";
    out << val[maxn] << endl;
}
int main(int argc, char* argv[])
{
    vector<thread> th;
    en = stoul(argv[1]);
    cur.store(2);
    unsigned int tn = thread::hardware_concurrency();
    for (unsigned int i = 0; i < tn; ++i)
        th.emplace_back(solve);
    print();
    for (auto& i : th)
        i.join();
    write(argv[2], argv[3], argv[4]);
    cout << endl;
    return 0;
}
