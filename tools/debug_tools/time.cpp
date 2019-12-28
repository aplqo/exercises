#pragma STDC FENV_ACCESS ON
#if __cplusplus < 201103L
#error ISO c++11 is required to use test program!
#endif
#ifdef __GNUC__
#define quick_exit(a) abort()
#endif
#include "debug_tools/output.h"
#include <cfenv>
#include <chrono>
#include <condition_variable>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <type_traits>
using namespace std;
using namespace std::chrono;

namespace ns_run
{
    inline int run(int argc, char* c[]);
}
namespace apdebug_time
{
    using namespace apdebug_out;
    unsigned int tlim = 1000, hardlim = tlim * 10;
    char *cmd[100], ccmd = 1;
    char testcmd[100], *out = nullptr, *in = nullptr;

    /*-----Timer-----*/
    template <class tim, class uni1, class uni2>
    class timer
    {
    public:
        timer(const char* u1, const char* u2, unsigned int l)
            : n1(u1)
            , n2(u2)
            , lim(l)
            , pr(false)
            , stat(false)
        {
        }
        inline void start()
        {
            cerr << col::BLUE << "[Info] Start program" << endl;
            cerr << col::NONE;
            stat = true;
            watch = thread(&timer::watchdog, this);
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
            cerr << col::GREEN << "[Passed] Program finished after " << d1.count() << n1;
            cerr << " ( " << d2.count() << n2 << " )." << endl;
            if (d1.count() > lim)
                cerr << col::YELLOW << "[TLE] Warn: TLE may happen" << endl;
        }

    private:
        bool pr;
        typename tim::time_point beg, aft;
        unsigned int lim;
        const char *n1, *n2;

        /*-thread support-*/
        condition_variable cv;
        mutex mstat;
        thread watch;
        bool stat;
        void watchdog()
        {
            unique_lock<mutex> lk(mstat);
            if (cv.wait_for(lk, milliseconds(hardlim), [&]() -> bool { return !stat; }))
                return;
            cerr << col::RED << endl;
            cerr << "[MLE/TLE] "
                 << "Hard time limit( " << apdebug_time::hardlim << "ms ) exceeded." << endl;
            quick_exit(0);
        }
    };
    timer<steady_clock, milliseconds, microseconds> t("ms", "us", tlim);
    /*-----Signal support-----*/
    extern "C" void sig(int s)
    {
        cerr << endl
             << col::PURPLE << "[RE] Received SIG";
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
        cerr << col::PURPLE << endl;
        cerr << "[RE] Received SIGFPE: erroneous arithmetic operation such as divide by zero" << endl;
        cerr << "Exceptions raised:" << endl;
        if (fetestexcept(FE_DIVBYZERO))
            cerr << "\tFE_DIVBYZERO: pole error occurred in an earlier floating-point operation" << endl;
        if (fetestexcept(FE_INEXACT))
            cerr << "\tFE_INEXACT: inexact result: rounding was necessary to store the result of an earlier floating-point operation" << endl;
        if (fetestexcept(FE_INVALID))
            cerr << "\tFE_INVALID: domain error occurred in an earlier floating-point operation " << endl;
        if (fetestexcept(FE_OVERFLOW))
            cerr << "\tFE_OVERFLOW: the result of the earlier floating-point operation was too large to be representable" << endl;
        if (fetestexcept(FE_UNDERFLOW))
            cerr << "\tFE_UNDERFLOW: the result of the earlier floating-point operation was subnormal with a loss of precision" << endl;
        quick_exit(0);
    }
    void regsig()
    {
#ifdef _MSC_VER
        _controlfp(0, _MCW_EM); //Enable floating point exception
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
        regsig();
        atexit(atex);
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
        cerr << col::BLUE << "[Info] Start testing" << endl;
        cerr << col::NONE;
        cerr.flush();
        int ret = system(testcmd);
        if (ret)
            cerr << col::RED << "[WA] Test program finished return " << ret << endl;
        else
            cerr << col::GREEN << "[Pass] Test program finished" << endl;
        return ret;
    }
    void info(const char* str, const char* val)
    {
        cerr << col::CYAN << "[Info] " << str << ": " << val << endl;
    }
    void printT(int n, const char* in)
    {
        cerr << col::CYAN << "[Info] " << in << ": ";
        cerr << n << "ms ( " << noshowpoint << (n / 1000.0) << "s )" << endl;
    }
} // namespace apdebug
int main(int argc, char* argv[])
{
    using namespace apdebug_out;
    cerr << "----------Aplqo debug tool----------" << endl;
    apdebug_time::cmd[0] = argv[0];
    for (int i = 1; i < argc; ++i)
    {
        if (!strcmp(argv[i], "-in"))
        {
            freopen(argv[++i], "r", stdin);
            apdebug_time::info("Input file", argv[i]);
            apdebug_time::in = argv[i];
        }
        else if (!strcmp(argv[i], "-out"))
        {
            freopen(argv[++i], "w", stdout);
            apdebug_time::info("Output file", argv[i]);
            apdebug_time::out = argv[i];
        }
        else if (!strcmp(argv[i], "-test"))
            strcpy(apdebug_time::testcmd, argv[++i]);
        else if (!strcmp(argv[i], "-time"))
        {
            apdebug_time::tlim = atoi(argv[++i]);
            apdebug_time::hardlim = apdebug_time::tlim * 10;
        }
        else if (!strcmp(argv[i], "-hlimit"))
            apdebug_time::hardlim = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-args"))
        {
            apdebug_time::ccmd += atoi(argv[++i]);
            cerr << apdebug_time::col::CYAN << "[Info] Arguments: ";
            for (int j = 1; j <= apdebug_time::ccmd; ++j, ++i)
            {
                apdebug_time::cmd[j] = argv[i];
                cerr << argv[i] << " ";
            }
            cerr << endl;
        }
    }
    if (apdebug_time::out != nullptr && strlen(apdebug_time::testcmd) != 0)
    {
        strcat(apdebug_time::testcmd, " ");
        strcat(apdebug_time::testcmd, apdebug_time::in);
        strcat(apdebug_time::testcmd, " ");
        strcat(apdebug_time::testcmd, apdebug_time::out);
        apdebug_time::info("Test command", apdebug_time::testcmd);
    }
    apdebug_time::printT(apdebug_time::tlim, "Time limit");
    apdebug_time::printT(apdebug_time::hardlim, "Hard time limit");
    cerr.flush();
    try
    {

        apdebug_time::call_main();
    }
    catch (const exception& e)
    {
        cerr << endl
             << col::RED << "[RE] Throw an exception what():" << e.what();
        quick_exit(0);
    }
    catch (...)
    {
        cerr << endl
             << col::RED << "[RE] Throw an unknown exception";
        quick_exit(0);
    }
    apdebug_time::test();
    return 0;
}
namespace ns_run
{
#ifndef APINPROG

    inline int run(int argc, char* argv[])
    {
        string cm(argv[0]);
        for (int i = 1; i < argc; i++)
            cm += " " + string(argv[i]);
        return system(cm.c_str());
    }

#else
#define main(...) ns_run::run(int argc, char* argv[])
#endif
} // namespace ns_run
