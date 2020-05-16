#include <atomic>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
using namespace std;
const int maxn = 1000;

atomic_uint cur;
string p;

void writeFile(const string& s, const string& v)
{
    ofstream f(s);
    f << v;
}
void print()
{
    using namespace std::chrono;
    while (cur <= maxn)
    {
        cout << "\rGenerating " << cur << " data.";
        cout.flush();
        this_thread::sleep_for(milliseconds(200));
    }
}
void thrd()
{
    while (cur <= maxn)
    {
        unsigned int v = cur.fetch_add(1);
        const string s = to_string(v);
        writeFile(s + ".in", s);
        system((p + " " + s + ".in " + s + ".ans").c_str());
    }
}
int main(int argc, char* argv[])
{
    p = argv[1];
    cur.store(2);
    vector<thread> thr;
    {
        unsigned int t = thread::hardware_concurrency();
        for (unsigned int i = 0; i < t; ++i)
            thr.emplace_back(thrd);
    }
    print();
    for (auto& i : thr)
        i.join();
    return 0;
}