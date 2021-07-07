#include <utility>
#include <vector>
#include <string>

#ifndef BIGINTEGER_BIGINTEGER_H
#define BIGINTEGER_BIGINTEGER_H


class BigInteger {
public:
    BigInteger() = default;

    BigInteger(int value_);

    BigInteger(const BigInteger& copy);

    ~BigInteger() = default;

    BigInteger operator-() const;

    BigInteger& operator+=(const BigInteger& x);

    BigInteger& operator-=(const BigInteger& x);

    BigInteger& operator*=(const BigInteger& x);

    BigInteger& operator/=(const BigInteger& x);

    BigInteger& operator%=(const BigInteger& x);

    BigInteger& operator++();

    BigInteger& operator--();

    const BigInteger operator++(int);

    const BigInteger operator--(int);

    friend BigInteger operator+(const BigInteger& x, const BigInteger& y);

    friend BigInteger operator-(const BigInteger& x, const BigInteger& y);

    friend BigInteger operator*(const BigInteger& x, const BigInteger& y);

    friend BigInteger operator/(const BigInteger& x, const BigInteger& y);

    friend BigInteger operator%(const BigInteger& x, const BigInteger& y);

    friend bool operator==(const BigInteger& x, const BigInteger& y);

    friend bool operator!=(const BigInteger& x, const BigInteger& y);

    friend bool operator<(const BigInteger& x, const BigInteger& y);

    friend bool operator>(const BigInteger& x, const BigInteger& y);

    friend bool operator<=(const BigInteger& x, const BigInteger& y);

    friend bool operator>=(const BigInteger& x, const BigInteger& y);

    operator bool() const;

    friend std::ostream& operator<<(std::ostream& out, const BigInteger& x);

    friend std::istream& operator>>(std::istream& in, BigInteger& x);

    std::string toString() const;

private:
    static const int BASIS = 10;

    std::vector<int> value;
    bool negative;

    BigInteger(bool negative_, std::vector<int> value_);

    BigInteger(std::string value);

    static int get_digit_max_length(int basis);

    static std::vector<int> reverse(std::vector<int>& v);

    static std::vector<int> remove_lead_zeros(std::vector<int>& x);
};


#endif //BIGINTEGER_BIGINTEGER_H
