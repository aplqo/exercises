//for libstdc++
#define _GLIBCXX_HAVE_QUICK_EXIT
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <csignal>
#include <cstring>
#include <future>
#include <string>
using namespace std;
using namespace std::chrono;

namespace ns_run
{
inline int run(int argc, char *c[]);
}
/*---Signal Handlers---*/
extern "C" void apdebug_sig(int s)
{
	cerr << "RE: Received SIG";
	switch (s)
	{
	case SIGSEGV:
		cerr << "SEGV";
		break;
	case SIGFPE:
		cerr << "FPE";
		break;
	case SIGINT:
		cerr << "INT";
		break;
	case SIGILL:
		cerr << "ILL";
		break;
	case SIGTERM:
		cerr << "TERM";
		break;
	}
	cerr << endl;
	quick_exit(0);
}
extern "C" void apdebug_abrt(int s)
{
	quick_exit(0);
}
namespace apdebug
{
unsigned int tlim = 1000, hardlim = tlim * 10;
char *cmd[100], ccmd = 1;
char testcmd[100] = "", *out = nullptr, *in = nullptr;
/*---Timer---*/
template <class tim, class uni1, class uni2>
class timer
{
public:
	timer(const char *u1, const char *u2, unsigned int l) : n1(u1), n2(u2), lim(l), stat(false){};
	inline void start()
	{
		beg = tim::now();
		stat = true;
	};
	inline void stop() { aft = tim::now(); };
	void print()
	{
		if (!stat)
			return;
		stat = false;
		cerr << endl;
		uni1 d1 = duration_cast<uni1>(aft - beg);
		uni2 d2 = duration_cast<uni2>(aft - beg);
		cerr << "Program finished after " << d1.count() << n1;
		cerr << " ( " << d2.count() << n2 << " )." << endl;
		if (d1.count() > lim)
			cerr << "Warn: TLE may happen" << endl;
	}

private:
	typename tim::time_point beg, aft;
	unsigned int lim;
	const char *n1, *n2;
	bool stat;
};
timer<steady_clock, milliseconds, microseconds> t("ms", "us", tlim);
void regsig()
{
	signal(SIGSEGV, apdebug_sig);
	signal(SIGFPE, apdebug_sig);
	signal(SIGILL, apdebug_sig);
	signal(SIGINT, apdebug_sig);
	signal(SIGTERM, apdebug_sig);
	signal(SIGABRT, apdebug_abrt);
}
void atex()
{
	t.stop();
	t.print();
	return;
}
int call_main()
{
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
	apdebug::regsig();
	atexit(apdebug::atex);
	ios_base::sync_with_stdio(false);
	int beg = 1;
#ifndef APINPROG
	beg = 2;
	apdebug::cmd[0] = argv[1];
#endif
	for (int i = beg; i < argc; ++i)
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
	future<int> f = async(launch::async, apdebug::call_main);
	if (f.wait_for(milliseconds(apdebug::hardlim)) == future_status::timeout)
	{
		cerr << "MLE/TLE: "
			 << "Hard time limit( " << apdebug::hardlim << "ms ) exceeded.";
		quick_exit(0);
	}
	try
	{
		int ret = f.get();
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
