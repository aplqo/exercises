#include <filesystem>
#include <fstream>
#include <string>
using namespace std;

void copyInput(ifstream& in, const std::filesystem::path& p)
{
    ofstream f(p);
    f << "1" << endl;
    unsigned int n, m, k, a;
    in >> n >> m >> k >> a;
    f << n << " " << m << " " << k << " " << a << endl;
    for (unsigned int i = 0; i < n; ++i)
    {
        unsigned int t;
        in >> t;
        f << t << " ";
    }
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int l, r;
        in >> l >> r;
        f << l << " " << r << endl;
    }
}
void copyAnswer(const char* ans, const unsigned int t, const std::filesystem::path& dir)
{
    ifstream fans(ans);
    for (unsigned int i = 0; i < t; ++i)
    {
        ofstream of((dir / to_string(i)).replace_extension(".ans"));
        unsigned int t;
        fans >> t;
        of << t;
    }
}
int main(int argc, char* argv[])
{
    ifstream in(argv[1]);
    const std::filesystem::path d(argv[3]);
    std::filesystem::create_directory(d);
    unsigned int t;
    in >> t;
    for (unsigned int i = 0; i < t; ++i)
        copyInput(in, (d / to_string(i)).replace_extension(".in"));
    copyAnswer(argv[2], t, d);
    return 0;
}