#include<iostream>
#include<cstdio>
#include<complex>
using std::cin;
using std::cout;
using std::complex;
using std::sqrt;
using std::printf;

int main()
{
	double a, b, c;
	cin >> a >> b >> c;
	complex<double> x1, x2;

	complex<double> delt = sqrt(complex<double>(b * b - 4 * a * c));
	x1 = (0 - b + delt) / (2 * a);
	x2 = (0 - b - delt) / (2 * a);

	if (delt == complex<double>(0))
	{
		printf("x1=x2=%.5lf", x1.real());
	}
	else if (delt.imag() > 0)
	{
		printf("x1=%.5lf%+.5lfi;", x1.real(), x1.imag());
		printf("x2=%.5lf%+.5lfi", x2.real(), x2.imag());
	}
	else
	{
		printf("x1=%.5lf;x2=%.5lf", x1.real(), x2.real());
	}
	return 0;
}
