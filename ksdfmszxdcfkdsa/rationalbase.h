#pragma once
#ifndef RATIONAL_BASE_HPP
#define RATIONAL_BASE_HPP
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

class Rational {
private:
    long long numerator_ = 0;
    long long denominator_ = 1;

    static long long magnitude(long long x) {
        return x < 0 ? -x : x;
    }
    static long long gcd(long long a, long long b) {
        a = magnitude(a); b = magnitude(b);
        while (b != 0) {
            long long r = a % b;
            a = b; b = r;
        }
        return a;
    }
    static long long checkedAdd(long long a, long long b) {
        const long long m = std::numeric_limits<long long>::max();
        if ((b > 0 && a > m - b) ||
            (b < 0 && a < -m - b))
            throw std::overflow_error("Integer overflow");
        return a + b;
    }
    static long long checkedMul(long long a, long long b) {
        const long long m = std::numeric_limits<long long>::max();
        if (b != 0 && magnitude(a) > m / magnitude(b))
            throw std::overflow_error("Integer overflow");
        return a * b;
    }
    // Exact comparison without multiplication or floating-point rounding.
    static int comparePositive(long long a, long long b,
        long long c, long long d) {
        bool reverse = false;
        for (;;) {
            long long q1 = a / b, q2 = c / d;
            if (q1 != q2) {
                int result = q1 < q2 ? -1 : 1;
                return reverse ? -result : result;
            }
            long long r1 = a % b, r2 = c % d;
            if (r1 == 0 || r2 == 0) {
                int result = r1 == r2 ? 0 : (r1 == 0 ? -1 : 1);
                return reverse ? -result : result;
            }
            a = b; b = r1; c = d; d = r2;
            reverse = !reverse;
        }
    }
public:
    void Init(long long n, long long d) {
        const long long low = std::numeric_limits<long long>::min();
        if (d == 0)
            throw std::invalid_argument("Denominator must not be zero");
        if (n == low || d == low)
            throw std::out_of_range("LLONG_MIN is not supported");
        if (d < 0) { n = -n; d = -d; }
        numerator_ = n; denominator_ = d;
        reduce();
    }
    void Read() {
        long long n, d;
        if (!(std::cin >> n >> d))
            throw std::invalid_argument("Expected two integers");
        Init(n, d);
    }
    void Display() const { std::cout << toString(); }
    std::string toString() const {
        return std::to_string(numerator_) + "/" +
            std::to_string(denominator_);
    }
    long long getNumerator() const { return numerator_; }
    long long getDenominator() const { return denominator_; }
    void reduce() {
        long long g = gcd(numerator_, denominator_);
        numerator_ /= g; denominator_ /= g;
    }
    Rational add(const Rational& other) const {
        long long g = gcd(denominator_, other.denominator_);
        long long left = checkedMul(numerator_, other.denominator_ / g);
        long long right = checkedMul(other.numerator_, denominator_ / g);
        Rational result;
        result.Init(checkedAdd(left, right),
            checkedMul(denominator_, other.denominator_ / g));
        return result;
    }
    Rational sub(const Rational& other) const {
        Rational negative;
        negative.Init(-other.numerator_, other.denominator_);
        return add(negative);
    }
    Rational mul(const Rational& other) const {
        long long g1 = gcd(numerator_, other.denominator_);
        long long g2 = gcd(other.numerator_, denominator_);
        Rational result;
        result.Init(checkedMul(numerator_ / g1, other.numerator_ / g2),
            checkedMul(denominator_ / g2, other.denominator_ / g1));
        return result;
    }
    Rational div(const Rational& other) const {
        if (other.numerator_ == 0)
            throw std::domain_error("Division by zero rational");
        Rational reciprocal;
        reciprocal.Init(other.denominator_, other.numerator_);
        return mul(reciprocal);
    }
    bool equal(const Rational& other) const {
        return numerator_ == other.numerator_ &&
            denominator_ == other.denominator_;
    }
    bool less(const Rational& other) const {
        if ((numerator_ < 0) != (other.numerator_ < 0))
            return numerator_ < 0;
        int comparison = comparePositive(magnitude(numerator_), denominator_,
            magnitude(other.numerator_), other.denominator_);
        return numerator_ < 0 ? comparison > 0 : comparison < 0;
    }
    bool greate(const Rational& other) const { return other.less(*this); }
};
#endif#pragma once
