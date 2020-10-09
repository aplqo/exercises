#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
using namespace std;
const unsigned int maxb = 100000 * 10;

void createDir(const char* p)
{
    using namespace std::filesystem;
    const path pat(p);
    create_directory(pat);
    current_path(pat);
}
void copyLine(ifstream& in, ofstream& out, const unsigned int n)
{
    static char buf[maxb + 10];
    while (!isalnum(in.peek()))
        in.ignore(1);
    for (unsigned int i = 0; i < n; ++i)
    {
        in.getline(buf, maxb, '\n');
        streamsize len = in.gcount();
        while (len > 0 && !isalnum(buf[len - 1]))
            --len;
        buf[len++] = '\n';
        out.write(buf, len);
    }
}

int main(int, char* argv[])
{
    ifstream fin(argv[1]), fans(argv[2]);
    createDir(argv[4]);
    unsigned int t;
    fin >> t;
    for (unsigned int i = 0; i < t; ++i)
    {
        const string pre = argv[3] + to_string(i);
        ofstream in(pre + ".in"), ans(pre + ".ans");
        in << 1 << "\n";
        unsigned int n, m, q;
        fin >> n >> m;
        in << n << " " << m << "\n";
        copyLine(fin, in, m);
        fin >> q;
        in << q << "\n";
        copyLine(fin, in, q);
        copyLine(fans, ans, q);
    }
    return 0;
}