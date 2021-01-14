#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iterator>
using namespace std;

inline void copyLine(ifstream& src, ofstream& dest)
{
    src.get(*dest.rdbuf());
    src.ignore(1);
    dest.put('\n');
}
void copyInput(ifstream& src, const string& name)
{
    ofstream dest(name + ".in");
    dest << "1\n";
    unsigned int n, m;
    src >> n >> m;
    dest << n << " " << m << "\n";
    src.ignore(10, '\n');
    for (unsigned int i = m; i; --i)
        copyLine(src, dest);
}
void copyAnswer(ifstream& src, const string& name)
{
    ofstream dest(name + ".ans");
    copyLine(src, dest);
    copyLine(src, dest);
}
int main(int, char* argv[])
{
    ifstream in(argv[1]), ans(argv[2]);
    {
        const filesystem::path p(argv[3]);
        if (!filesystem::exists(p))
            filesystem::create_directory(p);
        filesystem::current_path(p);
    }
    unsigned int t;
    in >> t;
    for (unsigned int i = 0; i < t; ++i)
    {
        const auto str = argv[4] + to_string(i);
        copyInput(in, str);
        copyAnswer(ans, str);
    }
    return 0;
}