#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <limits>
#include <string>
using namespace std;

void CreateOut(const char* in)
{
    using namespace std::filesystem;
    path out = path(in).stem().concat("-data");
    create_directory(out);
    current_path(out);
}
void WriteOut(ifstream& in, const unsigned int cnt)
{
    unsigned int n, m;
    in >> n >> m;
    if (!n && !m)
        exit(0);
    ofstream out(to_string(cnt) + ".in");
    out << n << " " << m << endl;
    for (unsigned int i = 0; i < n; ++i)
    {
        unsigned int p, d;
        in >> p >> d;
        out << p << " " << d << endl;
    }
    out << "0 0" << endl;
    out.close();
}
void WriteAns(ifstream& ans, const unsigned int cnt)
{
    ofstream out(to_string(cnt) + ".ans");
    string t;
    ans.ignore(numeric_limits<streamsize>::max(), '#');
    ans.ignore(3, '\n');
    out << "Jury #1" << endl;
    getline(ans, t);
    out << t << endl;
    getline(ans, t);
    out << t << endl;
    out.close();
}

int main(int argc, char* argv[])
{
    ifstream in(argv[1]), ans(argv[2]);
    CreateOut(argv[1]);
    unsigned int cnt = 0;
    while (true)
    {
        WriteOut(in, cnt);
        WriteAns(ans, cnt);
        ++cnt;
    }
    return 0;
}