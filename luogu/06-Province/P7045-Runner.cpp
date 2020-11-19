#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
using namespace std;

struct Fifo
{
    Fifo() = default;
    void create(string&& c)
    {
        path = c;
        mkfifoat(AT_FDCWD, path.c_str(), 0600);
    }
    char* name() const { return const_cast<char*>(path.c_str()); }
    ~Fifo() { remove(path.c_str()); }

    string path;
} infifo, outfifo;

void createFifo(const string& thr)
{
    infifo.create(thr + "-in.fifo");
    outfifo.create(thr + "-out.fifo");
}
pid_t execInteract(const char* pro, const char* in, const char* out, const char* info)
{
    const pid_t p = vfork();
    if (p == 0)
        execl(pro, pro, in, infifo.name(), outfifo.name(), out, info, nullptr);
    else
        return p;
}
int execProgram(const char* pro, const unsigned int argc, char* argv[])
{
    char** args = new char* [argc + 5] {};
    args[0] = const_cast<char*>(pro);
    args[1] = infifo.name();
    args[2] = outfifo.name();
    args[3] = argv[3];
    args[4] = argv[4];
    args[5] = argv[5];
    copy(argv + 10, argv + argc, args + 6);
    const pid_t p = vfork();
    if (p == 0)
        execv(pro, args);
    else
    {
        int wstat;
        waitpid(p, &wstat, 0);
        delete[] args;
        return wstat;
    }
}

int main(int argc, char* argv[])
{
    const char *in = "/dev/null", *out = "/dev/null", *info = "/dev/null";
    if (strcmp(argv[1], "*") != 0)
        in = argv[1];
    if (strcmp(argv[2], "*") != 0)
        out = argv[2];
    if (strcmp(argv[8], "*") != 0)
        info = argv[8];
    createFifo(argv[9]);
    const pid_t p = execInteract(argv[7], in, out, info);
    const int v = execProgram(argv[6], argc, argv);
    int wstat;
    waitpid(p, &wstat, 0);
    return v;
}