#ifndef CHECKED_H
#define CHECKED_H
#include "debug_tools/output.h"
#include <cstdlib>
#include <iostream>
#include <type_traits>

namespace checked
{
    using namespace apdebug_out;
    using std::cerr;
    using std::common_type;
    using std::is_integral;
    using std::istream;
    using std::ostream;
    using std::quick_exit;

    template <class T>
    class CheckedInteger
    {
    public:
        static_assert(is_integral<T>::value, "Check type must be integral!");
        CheckedInteger() = default;
        CheckedInteger(T s)
            : dat(s)
        {
        }
        template <class U>
        inline auto operator+(U r)
        {
            auto ret = dat + r;
            if (ret - dat != r || ((r > 0) ^ (ret > this->dat)))
                err();
            return ret;
        }
        template <class U>
        inline auto operator*(U a)
        {
            auto ret = dat * a.dat;
            if (ret / a.dat != this->dat)
                err();
            return ret;
        }
        template <class U>
        inline auto operator-(U a)
        {
            auto ret = dat - a.dat;
            if (ret + a.dat != this->dat || ((a.dat < 0) ^ (ret < this->dat)))
                err();
            return ret;
        }
        template <class U>
        inline auto operator/(U r)
        {
            if (r == 0)
            {
                cerr << col::PURPLE << "[RE] Integral type divided by zero!" << endl;
                quick_exit(0);
            }
            return dat / r;
        }
        operator T() { return dat; };
        /*Increment and decrement*/
        inline typename CheckedInteger<T>& operator++()
        {
            T ret = dat;
            ++dat;
            if (ret >= dat)
                err();
            return *this;
        }
        inline typename CheckedInteger<T> operator++(int t)
        {
            T ret = dat;
            ++dat;
            if (ret >= dat)
                err();
            return ret;
        }
        inline typename CheckedInteger<T>& operator--()
        {
            T ret = dat;
            --dat;
            if (ret <= dat)
                err();
            return *this;
        }
        inline typename CheckedInteger<T> operator--(int t)
        {
            T ret = dat;
            --dat;
            if (ret <= dat)
                err();
            return ret;
        }
#define oper(op)       \
    template <class U> \
    inline auto operator op(U r) { return dat op r; }
        oper(%);
        /*Bitwise arithmetic operators*/
        oper(&);
        oper(|);
        oper (^);
        oper(>>);
        oper(<<);
        /*Relational operators*/
        oper(>);
        oper(<);
        oper(==);
        oper(!=);
        oper(>=);
        oper(<=);
#undef oper
        /*Binary arithmetic operators*/
#define assop(op)      \
    template <class U> \
    inline typename CheckedInteger<T> operator op##=(U a) { return *this = *this op a; }
        assop(/);
        assop(*);
        assop(-);
        assop(+);
        assop(%);
        assop(&);
        assop(|);
        assop (^);
        assop(>>);
        assop(<<);
#undef assop
        inline auto operator~()
        {
            return ~dat;
        }
        /*Stream extraction and insertion*/
        inline ostream operator<<(ostream& os)
        {
            os << dat;
            return os;
        }
        inline istream operator>>(istream& is)
        {
            is >> dat;
            return is;
        }

    private:
        static void err()
        {
            cerr << col::YELLOW << "[Warn] Integer overflow with type " << typeid(T).name() << endl;
            quick_exit(0);
        }
        T dat;
    };
}

#endif