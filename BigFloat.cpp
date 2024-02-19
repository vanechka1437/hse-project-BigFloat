#include "BigFloat.h"

// default value for BigFloat: 0.0
BigFloat::BigFloat() {
    _is_positive = true;
    _value = "00000000000";
    _precision = 10;
}

BigFloat::BigFloat(const std::string &str, int precision) {
    _is_positive = (str[0] != '-');
    _precision = precision;
    _value = _is_positive ? str : str.substr(1);

    size_t position1 = _value.find_first_of('.');
    size_t position2 = _value.find_first_not_of('0');
    size_t position3 = _value.find_first_not_of("0.");

    // if number == 0
    if (position3 == std::string::npos) {
        _value = std::string(precision+1,'0');
        _is_positive = true;
        return;
    }
    // if number is integer: without dot
    if (position1 == std::string::npos) {
        _value = _value.substr(position2) + std::string(precision,'0');
        return;
    }

    // create right precision
    size_t input_precision = _value.size() - position1 - 1;
    if (precision > input_precision) {
        _value += std::string(precision-input_precision,'0');
    }
    if (precision < input_precision){
        _value = _value.substr(0,position1+precision+1);
    }

    // delete leading zeroes
    if (position1 == position2) {
        _value = "0" + _value.substr(position2+1);
    } else {
        _value = _value.substr(position2,position1-position2) + _value.substr(position1+1);
    }
}

bool BigFloat::positive() const {
    return _is_positive;
}

int BigFloat::precision() const{
    return _precision;
}

std::string BigFloat::value() const {
    return _value;
}

std::ostream& operator<<(std::ostream& os, const BigFloat &num) {
    os << (num.positive() ? "" : "-") << num.value().substr(0, num.value().size() - num.precision()) << "." << num.value().substr(num.value().size() - num.precision());
    return os;
}

BigFloat operator"" _bf(const char *str) {
    if (str == nullptr) {
        return BigFloat{};
    }
    return BigFloat{std::string(str)};
}