#include <filesystem>
#include <fstream>
#include <string>
using namespace std;

inline void create(const char* inp)
{
    using namespace std::filesystem;
    path p(inp);
    create_directory(p.replace_extension().filename());
    current_path(p.replace_extension().filename());
}
int main(int argc, char* argv[])
{
    string bas(argv[1]);
    ifstream oin(bas + ".in"), oans(bas + ".ans");
    create(bas.c_str());
    unsigned int t;
    oin >> t;
    for (unsigned int i = 0; i < t; ++i)
    {
        unsigned long long a0, a1, b0, b1;
        string res;
        oin >> a0 >> a1 >> b0 >> b1;
        getline(oans, res);
        ofstream in(to_string(i) + ".in"), ans(to_string(i) + ".ans");
        in << "1" << endl;
        in << a0 << " " << a1 << " " << b0 << " " << b1 << endl;
        ans << res << endl;
    }
    return 0;
}
