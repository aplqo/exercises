#pragma STDC FENV_ACCESS ON
#ifdef GCC
#define quick_exit(a) abort()
#endif
#include <chrono>
#include <condition_variable>
#include <csignal>
#include <cstdio>
#include <thread>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <mutex>
#include <string>
#include <cfenv>
using namespace std;
using namespace std::chrono;

namespace ns_run
{
inline int run(int argc, char *c[]);
}
namespace apdebug
{
unsigned int tlim = 1000, hardlim = tlim * 10;
char *cmd[100], ccmd = 1;
char testcmd[100], *out = nullptr, *in = nullptr;
condition_variable cv;
mutex mstat;
thread watch;
bool stat;
/*---Timer---*/
void watchdog()
{
    unique_lock<mutex> lk(mstat);
    if (cv.wait_for(lk, milliseconds(hardlim), []() -> bool { return !stat; }))
        return;
    cerr << "MLE/TLE: "
         << "Hard time limit( " << apdebug::hardlim << "ms ) exceeded.";
    quick_exit(0);
}
template <class tim, class uni1, class uni2>
class timer
{
public:
    timer(const char *u1, const char *u2, unsigned int l)
        : n1(u1), n2(u2), lim(l), pr(false) {}
    inline void start()
    {
        stat = true;
        watch = thread(watchdog);
        beg = tim::now();
    }
    void stop()
    {
        if (!stat)
            return;
        aft = tim::now();
        stat = false;
        pr = true;
        cv.notify_all();
        if (watch.joinable())
            watch.join();
    }
    void print()
    {
        if (!pr)
            return;
        pr = false;
        cerr << endl;
        uni1 d1 = duration_cast<uni1>(aft - beg);
        uni2 d2 = duration_cast<uni2>(aft - beg);
        cerr << "Program finished after " << d1.count() << n1;
        cerr << " ( " << d2.count() << n2 << " )." << endl;
        if (d1.count() > lim)
            cerr << "Warn: TLE may happen" << endl;
    }

private:
    bool pr;
    typename tim::time_point beg, aft;
    unsigned int lim;
    const char *n1, *n2;
};
timer<steady_clock, milliseconds, microseconds> t("ms", "us", tlim);
/*---Signal Handlers---*/
extern "C" void sig(int s)
{
    cerr << "RE: Received SIG";
    switch (s)
    {
    case SIGSEGV:
        cerr << "SEGV: invalid memory access (segmentation fault)";
        break;
    case SIGINT:
        cerr << "INT: external interrupt, usually initiated by the user";
        break;
    case SIGILL:
        cerr << "ILL: invalid program image, such as invalid instruction";
        break;
    case SIGTERM:
        cerr << "TERM: termination request";
        break;
    }
    cerr << endl;
    quick_exit(0);
}
extern "C" void fpe_handler(int sig)
{
    cerr << "RE: Received SIGFPE: erroneous arithmetic operation such as divide by zero" << endl;
    cerr << "Exceptions raised:" << endl;
    if (fetestexcept(FE_DIVBYZERO))
        cerr << " FE_DIVBYZERO: pole error occurred in an earlier floating-point operation";
    if (fetestexcept(FE_INEXACT))
        cerr << " FE_INEXACT: inexact result: rounding was necessary to store the result of an earlier floating-point operation";
    if (fetestexcept(FE_INVALID))
        cerr << " FE_INVALID: domain error occurred in an earlier floating-point operation ";
    if (fetestexcept(FE_OVERFLOW))
        cerr << " FE_OVERFLOW: the result of the earlier floating-point operation was too large to be representable";
    if (fetestexcept(FE_UNDERFLOW))
        cerr << " FE_UNDERFLOW: the result of the earlier floating-point operation was subnormal with a loss of precision";
    quick_exit(0);
}
void regsig()
{
#ifdef MSVC
    _controlfp(0, _MCW_EM);
#endif
    signal(SIGSEGV, sig);
    signal(SIGFPE, fpe_handler);
    signal(SIGILL, sig);
    signal(SIGINT, sig);
    signal(SIGTERM, sig);
}
void atex() noexcept
{
    t.stop();
    t.print();
}
int call_main()
{
    atexit(atex);
    regsig();
    t.start();
    int ret = ns_run::run(ccmd, cmd);
    t.stop();
    t.print();
    return ret;
}
int test()
{
    if (out == nullptr || strlen(testcmd) == 0)
        return 0;
    cerr << "-----Test result-----" << endl;
    strcat(testcmd, " ");
    strcat(testcmd, in);
    strcat(testcmd, " ");
    strcat(testcmd, out);
    return system(testcmd);
}
} // namespace apdebug
int main(int argc, char *argv[])
{
    apdebug::cmd[0] = argv[0];
    for (int i = 1; i < argc; ++i)
    {
        if (!strcmp(argv[i], "-in"))
        {
            freopen(argv[++i], "r", stdin);
            apdebug::in = argv[i];
        }
        else if (!strcmp(argv[i], "-out"))
        {
            freopen(argv[++i], "w", stdout);
            apdebug::out = argv[i];
        }
        else if (!strcmp(argv[i], "-test"))
            strcpy(apdebug::testcmd, argv[++i]);
        else if (!strcmp(argv[i], "-time"))
        {
            apdebug::tlim = atoi(argv[++i]);
            apdebug::hardlim = apdebug::tlim * 10;
        }
        else if (!strcmp(argv[i], "-hlimit"))
            apdebug::hardlim = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-args"))
        {
            apdebug::ccmd += atoi(argv[++i]);
            for (int j = 1; j <= apdebug::ccmd; ++j, ++i)
                apdebug::cmd[j] = argv[i];
        }
    }
    try
    {
        apdebug::call_main();
    }
    catch (const exception &e)
    {
        cerr << "RE: Throw an exception what():" << e.what();
        quick_exit(0);
    }
    catch (...)
    {
        cerr << "RE: Throw an unknown exception";
        quick_exit(0);
    }
    apdebug::test();
    return 0;
}
namespace ns_run
{
#ifndef APINPROG

inline int run(int argc, char *argv[])
{
    string cm(argv[0]);
    for (int i = 1; i < argc; i++)
        cm += " " + string(argv[i]);
    return system(cm.c_str());
}

#else
#define main(...) ns_run::run(int argc, char *argv[])
#endif
} // namespace ns_run
