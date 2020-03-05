#include <fstream>
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
    ifstream in(argv[1]);
    for (unsigned int i = 0; i < 9; ++i)
    {
        char t;
        for (unsigned int j = 0; j < 3; ++j)
        {
            t = in.get();
            cout << t << " ";
            in.ignore(2, ' ');
        }
        t = in.get();
        cout << t << endl;
        in.ignore(2, '\n');
    }
    return 0;
}