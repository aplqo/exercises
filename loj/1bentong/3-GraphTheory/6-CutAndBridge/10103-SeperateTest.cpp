#include <filesystem>
#include <fstream>
#include <string>
using namespace std;

int main(int, char* argv[])
{
    ifstream in(argv[1]), ans(argv[2]);
    std::filesystem::create_directory(argv[3]);
    std::filesystem::current_path(argv[3]);
    for (unsigned int v = 0;; ++v)
    {
        ofstream oin(to_string(v) + ".in"), oans(to_string(v) + ".ans");
        unsigned int n, c;
        in >> n >> c;
        if (!n && !c)
            break;
        oin << n << " " << c << "\n";
        for (unsigned int i = 0; i < c; ++i)
        {
            unsigned int u, v;
            in >> u >> v;
            oin << u << " " << v << "\n";
        }
        oin << "0 0"
            << "\n";

        unsigned int a;
        ans >> a;
        oans << a << "\n";
    }
    return 0;
}