#ifdef APDEBUG
#define APINPROG
#define COLOR
#include "debug_tools/time.cpp"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <iterator>
using namespace std;
const int maxn = 10;

char inf[maxn], suf[maxn];
void fun(char* bin, char* ein, char* bsu, char* esu)
{
    if (bin >= ein)
        return;
    cout << *(esu - 1);
    if (ein - bin == 1)
        return;
    char* pin = find(bin, ein, *(esu - 1));
    /*Process left child tree*/
    {
        char* psu = find_first_of(bsu, esu, bin, pin);
        fun(bin, pin, psu, psu + distance(bin, pin));
    }
    /*Process right child tree*/
    {
        char* psu = find_first_of(bsu, esu, pin + 1, ein);
        fun(pin + 1, ein, psu, psu + distance(pin + 1, ein));
    }
}
int main()
{
    ios_base::sync_with_stdio(false);
    cin >> inf >> suf;
    int il = strlen(inf), sl = strlen(suf);
    fun(inf, inf + il, suf, suf + sl);
    return 0;
}