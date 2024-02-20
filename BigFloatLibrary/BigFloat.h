#ifndef HSE_PROJECT_BIGFLOAT_BIGFLOAT_H
#define HSE_PROJECT_BIGFLOAT_BIGFLOAT_H
#pragma once
#include <iostream>
#include <string>

class BigFloat {
public:
    BigFloat();
    explicit BigFloat(const std::string &str, int precision = 10);

    ~BigFloat() = default;
    BigFloat(const BigFloat &other) = default;

    BigFloat& operator=(const BigFloat &other) = default;

    [[nodiscard]] bool positive() const;
    [[nodiscard]] int precision() const;
    [[nodiscard]] std::string value() const;

    bool operator==(const BigFloat &other) const;
    bool operator!=(const BigFloat &other) const;
    bool operator<=(const BigFloat &other) const;
    bool operator>=(const BigFloat &other) const;
    bool operator<(const BigFloat &other) const;
    bool operator>(const BigFloat &other) const;

    BigFloat operator-() const;
    BigFloat operator+(const BigFloat &other) const;
    BigFloat operator-(const BigFloat &other) const;
    BigFloat operator*(const BigFloat &other) const;
    BigFloat operator/(const BigFloat &other) const;

    void reduce_precision(int precision);

    std::string to_string() const;

private:
    bool _is_positive;
    int _precision;
    std::string _value;

    [[nodiscard]] BigFloat sum(const BigFloat &other) const;
    [[nodiscard]] BigFloat subtraction(const BigFloat &other) const;
    [[nodiscard]] BigFloat mini_multiplication(const char &digit) const;
    static bool less(const std::string &first, const std::string &second);
    static std::string subtract(const std::string &first, const std::string &second);
};

std::ostream& operator<<(std::ostream& os, const BigFloat &num);

BigFloat operator"" _bf(const char *str);

#endif //HSE_PROJECT_BIGFLOAT_BIGFLOAT_H
