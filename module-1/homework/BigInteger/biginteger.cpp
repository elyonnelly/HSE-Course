#include "biginteger.h"

BigInteger::BigInteger(int value_) {
    negative = value_ < 0;
    value_ = abs(value_);
    std::vector<int> result;
    while (value_ != 0) {
        result.push_back(value_ % BigInteger::BASIS);
        value_ /= BigInteger::BASIS;
    }
    for (int i = static_cast<int>(result.size()) - 1; i >= 0; i--) {
        value.push_back(result[i]);
    }
}

BigInteger::BigInteger(const BigInteger &copy) : value(copy.value), negative(copy.negative) {}

bool operator<(const BigInteger &x, const BigInteger &y) {
    if (x.negative != y.negative) {
        return x.negative > y.negative;
    }
    if (x.value.size() != y.value.size()) {
        return x.value.size() < y.value.size();
    }
    for (int i = 0; i < static_cast<int>(x.value.size()); i++) {
        if (x.value[i] != y.value[i]) {
            return x.value[i] < y.value[i];
        }
    }
    return false;
}

bool operator==(const BigInteger &x, const BigInteger &y) {
    if (x.value.size() != y.value.size() || x.negative != y.negative) {
        return false;
    }
    for (int i = 0; i < static_cast<int>(x.value.size()); i++) {
        if (x.value[i] != y.value[i]) {
            return false;
        }
    }
    return true;
}

bool operator>(const BigInteger &x, const BigInteger &y) {
    return !(x < y || x == y);
}

bool operator!=(const BigInteger &x, const BigInteger &y) {
    return !(x == y);
}

bool operator<=(const BigInteger &x, const BigInteger &y) {
    return x < y || x == y;
}

bool operator>=(const BigInteger &x, const BigInteger &y) {
    return x > y || x == y;
}

BigInteger::operator bool() const {
    return *this != BigInteger(0);
}

BigInteger BigInteger::operator-() const {
    BigInteger result = BigInteger(*this);
    result.negative = !negative;
    return result;
}

BigInteger operator+(const BigInteger &x, const BigInteger &y) {
    if (x.negative != y.negative) {
        return x.negative ? y - (-x) : x - (-y);
    }

    int index_x = static_cast<int>(x.value.size()) - 1;
    int index_y = static_cast<int>(y.value.size()) - 1;
    std::vector<int> sum;

    int transfer = 0;
    while (index_x >= 0 || index_y >= 0) {
        int x_val = index_x >= 0 ? x.value[index_x] : 0;
        int y_val = index_y >= 0 ? y.value[index_y] : 0;
        int subtotal = x_val + y_val + transfer;
        if (subtotal >= BigInteger::BASIS) {
            transfer = subtotal / BigInteger::BASIS;
            subtotal -= BigInteger::BASIS;
        } else {
            transfer = 0;
        }
        sum.push_back(subtotal);
        index_x--;
        index_y--;
    }
    if (transfer > 0) {
        sum.push_back(transfer);
    }
    return BigInteger(x.negative, BigInteger::reverse(sum));
}

BigInteger operator-(const BigInteger &x, const BigInteger &y) {
    if (x.negative != y.negative) {
        return x.negative ? -(-x + y) : x + (-y);
    }
    if (x < y) {
        return -(y - x);
    }
    int index_x = static_cast<int>(x.value.size()) - 1;
    int index_y = static_cast<int>(y.value.size()) - 1;
    std::vector<int> x_value = x.value;
    std::vector<int> y_value = y.value;
    std::vector<int> sub;

    while (index_x >= 0 && index_y >= 0) {
        if (index_x > 0 && x_value[index_x] < y_value[index_y]) {
            int offset = 1;
            x_value[index_x] += BigInteger::BASIS;
            while (index_x - offset >= 0 && x_value[index_x - offset] == 0) {
                x_value[index_x - offset] = BigInteger::BASIS - 1;
                offset++;
            }
            x_value[index_x - offset]--;
        }
        x_value[index_x] -= y_value[index_y];
        index_x--;
        index_y--;
    }
    return BigInteger(x.negative, BigInteger::remove_lead_zeros(x_value));
}

BigInteger operator*(const BigInteger &x, const BigInteger &y) {
    BigInteger product = x;
    product.negative = x.negative != y.negative;
    BigInteger y_copy = y;
    y_copy--;
    while (y_copy > BigInteger(0)) {
        product += x;
        --y_copy;
    }
    return product;
}

BigInteger operator/(const BigInteger &x, const BigInteger &y) {
    BigInteger quotient = 0;
    quotient.negative = x.negative != y.negative;
    BigInteger x_copy = x;
    while (x_copy > y) {
        ++quotient;
        x_copy -= y;
    }
    return quotient;
}

BigInteger operator%(const BigInteger &x, const BigInteger &y) {
    if (x.negative) {
        return -((-x) % y);
    }
    return x - y * (x / y);
}

const BigInteger BigInteger::operator++(int) {
    BigInteger result = *this;
    *this += 1;
    return result;
}

BigInteger &BigInteger::operator++() {
    *this += 1;
    return *this;
}

const BigInteger BigInteger::operator--(int) {
    BigInteger result = *this;
    *this -= 1;
    return result;
}

BigInteger &BigInteger::operator--() {
    *this -= 1;
    return *this;
}

BigInteger &BigInteger::operator+=(const BigInteger &x) {
    *this = *this + x;
    return *this;
}

BigInteger &BigInteger::operator-=(const BigInteger &x) {
    *this = *this - x;
    return *this;
}

BigInteger &BigInteger::operator*=(const BigInteger &x) {
    *this = *this * x;
    return *this;
}

BigInteger &BigInteger::operator/=(const BigInteger &x) {
    *this = *this / x;
    return *this;
}

BigInteger &BigInteger::operator%=(const BigInteger &x) {
    *this = *this % x;
    return *this;
}

std::vector<int> BigInteger::reverse(std::vector<int> &v) {
    std::vector<int> result;
    bool leading_zeros = v[v.size() - 1] == 0;
    for (int i = static_cast<int>(v.size()) - 1; i >= 0; i--) {
        while (leading_zeros && v[i] == 0) {
            i--;
        }
        leading_zeros = false;
        result.push_back(v[i]);
    }
    return result;
}

std::vector<int> BigInteger::remove_lead_zeros(std::vector<int> &x) {
    std::vector<int> result;
    int i = 0;
    while (x[i] == 0) {
        i++;
    }
    for (i = i; i < static_cast<int>(x.size()); i++) {
        result.push_back(x[i]);
    }
    return result;
}

std::ostream &operator<<(std::ostream &out, const BigInteger &x) {
    out << x.toString();
    return out;
}

std::istream &operator>>(std::istream &in, BigInteger &x) {
    std::string value;
    in >> value;
    x = BigInteger(value);
    return in;
}

std::string BigInteger::toString() const {
    std::string string_value;
    if (negative) {
        string_value += "-";
    }
    for (auto val : value) {
        string_value += std::to_string(val);
    }
    return string_value;
}

int BigInteger::get_digit_max_length(int basis) {
    int length = 1;
    int max_value_excluded = 10;
    while (max_value_excluded < basis) {
        length++;
        max_value_excluded *= 10;
    }
    return length;
}

BigInteger::BigInteger(std::string str_value) {
    negative = str_value[0] == '-';
    int i = negative ? 1 : 0;
    int digit_length = get_digit_max_length(BigInteger::BASIS);
    while (i < str_value.length()) {
        if (i + digit_length < str_value.length()) {
            value.push_back(atoi(str_value.substr(i, digit_length).c_str()));
        } else {
            value.push_back(atoi(str_value.substr(i, str_value.length() - i).c_str()));
        }
        i += digit_length;
    }
}

BigInteger::BigInteger(bool negative_, std::vector<int> value_) : negative(negative_), value(std::move(value_)) {}

