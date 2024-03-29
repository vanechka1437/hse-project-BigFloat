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

// if first and second are positive
BigFloat BigFloat::sum(const BigFloat &other) const {

    BigFloat result;
    BigFloat num1(*this);
    BigFloat num2(other);

    result._precision = std::max(num1._precision,num2._precision);
    // create uniform precision
    num1._value += std::string(result._precision-num1._precision,'0');
    num2._value += std::string(result._precision-num2._precision,'0');
    // create uniform integer part
    size_t size = std::max(num1._value.size(),num2._value.size());
    num1._value = std::string(size-num1._value.size(),'0') + num1._value;
    num2._value = std::string(size-num2._value.size(),'0') + num2._value;
    // element-wise sum
    result._value = std::string(num1._value.size(),'0');
    int overflow = 0;
    for (size_t i = result._value.size(); i >= 1; --i){
        int digits_sum = (num1._value[i-1] - '0') + (num2._value[i-1] - '0') + overflow;
        result._value[i-1] = static_cast<char>(digits_sum%10 + '0');
        overflow = digits_sum/10;
    }
    if (overflow > 0) {
        result._value = "1" + result._value;
    }
    return result;
}

// if first and second are positive and first > second
BigFloat BigFloat::subtraction(const BigFloat &other) const {
    BigFloat result;
    BigFloat num1(*this);
    BigFloat num2(other);
    result._precision = std::max(num1._precision,num2._precision);
    // create uniform precision
    num1._value += std::string(result._precision-num1._precision,'0');
    num2._value += std::string(result._precision-num2._precision,'0');
    // create uniform integer part
    size_t size = std::max(num1._value.size(),num2._value.size());
    num1._value = std::string(size-num1._value.size(),'0') + num1._value;
    num2._value = std::string(size-num2._value.size(),'0') + num2._value;
    // element-wise subtraction
    result._value = std::string(num1._value.size(),'0');
    int overflow = 0;
    for (size_t i = result._value.size(); i >= 1; --i) {
        int digits_sum = (num1._value[i-1] - '0') - (num2._value[i-1] - '0') + overflow;
        if (digits_sum < 0) {
            overflow = -1;
            digits_sum += 10;
        } else {
            overflow = 0;
        }
        result._value[i-1] = static_cast<char>(digits_sum + '0');
    }
    size_t position = result._value.find_first_not_of('0');
    if (position == std::string::npos){
        result._value = std::string(result._precision+1,'0');
        return result;
    }
    if (position < result._value.size() - result._precision) {
        result._value = result._value.substr(position);
    } else {
        result._value = "0" + result._value.substr(result._value.size()-result._precision);
    }
    return result;
}

bool BigFloat::operator==(const BigFloat &other) const{
    if (this->_is_positive != other._is_positive) {
        return false;
    }
    BigFloat num1(*this);
    BigFloat num2(other);
    size_t size = std::max(num1._precision,num2._precision);
    num1._value += std::string(size-num1._precision,'0');
    num2._value += std::string(size-num2._precision,'0');
    return num1._value == num2._value;
}

bool BigFloat::operator!=(const BigFloat &other) const {
    return !(*this == other);
}

bool BigFloat::operator<(const BigFloat &other) const {
    if (this->_is_positive && !other._is_positive) {
        return false;
    }
    if (!(this->_is_positive) && other._is_positive) {
        return true;
    }
    BigFloat num1(*this);
    BigFloat num2(other);
    // create uniform precision
    size_t size1 = std::max(num1._precision,num2._precision);
    num1._value += std::string(size1-num1._precision,'0');
    num2._value += std::string(size1-num2._precision,'0');
    // create uniform integer part
    size_t size2 = std::max(num1._value.size(),num2._value.size());
    num1._value = std::string(size2-num1._value.size(),'0') + num1._value;
    num2._value = std::string(size2-num2._value.size(),'0') + num2._value;

    if (num1._is_positive) {
        return num1._value < num2._value;
    }
    return num1._value > num2._value;
}

bool BigFloat::operator<=(const BigFloat &other) const {
    return ((*this < other) || (*this == other));
}

bool BigFloat::operator>(const BigFloat &other) const {
    return !(*this <= other);
}

bool BigFloat::operator>=(const BigFloat &other) const {
    return !(*this < other);
}

BigFloat BigFloat::operator+(const BigFloat &other) const {
    if (this->_is_positive && other._is_positive){
        return this->sum(other);
    }
    if (!this->_is_positive && !other._is_positive) {
        BigFloat result = this->sum(other);
        result._is_positive = false;
        return result;
    }
    BigFloat num1(*this);
    BigFloat num2(other);
    if (num1._is_positive) {
        num2._is_positive = true;
        if (num1 < num2) {
            BigFloat result = num2.subtraction(num1);
            result._is_positive = false;
            return result;
        }
        return num1.subtraction(num2);
    }
    num1._is_positive = true;
    if (num2 < num1) {
        BigFloat result = num1.subtraction(num2);
        result._is_positive = false;
        return result;
    }
    return num2.subtraction(num1);
}

BigFloat BigFloat::operator-() const {
    BigFloat null("0");
    BigFloat result(*this);
    if (*this == null) {
        return result;
    }
    result._is_positive = false;
    return result;
}

BigFloat BigFloat::operator-(const BigFloat &other) const {
    return (*this + (-other));
}

// if all are positive
BigFloat BigFloat::mini_multiplication(const char &digit) const {
    BigFloat result;
    if (digit == '0') {
        return result;
    }
    int d = digit - '0';
    result._is_positive = true;
    result._precision = this->_precision;
    result._value = std::string(this->_value.size(),'0');
    int overflow = 0;
    for (size_t i = result._value.size(); i >= 1; --i) {
        int sum = d * (this->_value[i-1] - '0') + overflow;
        result._value[i-1] = static_cast<char>(sum%10 + '0');
        overflow = sum/10;
    }
    if (overflow > 0) {
        result._value = std::string(1,overflow + '0') + result._value;
    }
    return result;
}

BigFloat BigFloat::operator*(const BigFloat &other) const {
    BigFloat num1(*this);
    BigFloat num2(other);
    BigFloat result;

    num1._is_positive = true;
    num2._is_positive = true;

    int digit = 0;
    for (size_t i = num2._value.size(); i >= 1; --i) {
        BigFloat sum = num1.mini_multiplication(num2._value[i-1]);
        sum._value += std::string(digit,'0');
        result = result + sum;
        digit++;
    }
    result._precision = num1._precision + num2._precision;

    if ((this->_is_positive && !other._is_positive) || (!this->_is_positive && other._is_positive)) {
        if (result._value.find_first_not_of('0') != std::string::npos){
            result._is_positive = false;
        }
    }

    return result;
}

void BigFloat::reduce_precision(int precision) {
    this->_value = this->_value.substr(0,_value.size() - _precision + precision);
    this->_precision = precision;
}

// without null
bool BigFloat::less(const std::string &first, const std::string &second) {
    size_t pos1 = first.find_first_not_of('0');
    if (pos1 == std::string::npos) {
        return true;
    }
    size_t pos2 = second.find_first_not_of('0');
    if (pos2 == std::string::npos) {
        return false;
    }
    if (first.size()-pos1 != second.size()-pos2) {
        return first.size()-pos1 < second.size()-pos2;
    }
    return first.substr(pos1) <= second.substr(pos2);
}

std::string BigFloat::subtract(const std::string &first, const std::string &second) {
    std::string result(first.size(),'0');
    std::string num = std::string(first.size()-second.size(),'0') + second;
    int overflow = 0;
    for (size_t i = first.size(); i >= 1; --i) {
        int sum = (first[i-1] - '0') - (num[i-1] - '0') + overflow;
        if (sum < 0) {
            sum += 10;
            overflow = -1;
        } else {
            overflow = 0;
        }
        result[i-1] = static_cast<char>(sum + '0');
    }
    size_t position = result.find_first_not_of('0');
    if (position == std::string::npos){
        return "0";
    }
    result = result.substr(position);
    return result;
}

BigFloat BigFloat::operator/(const BigFloat &other) const {
    if (other == BigFloat("0")) {
        return BigFloat("0");
    }

    BigFloat result;

    result._precision = this->_precision + other._precision;

    std::string num1 = this->_value;
    std::string num2 = other._value;
    std::string del = "";
    std::string res = "";

    num1 += std::string(std::max(this->precision(),other._precision),'0');
    num2 += std::string(std::max(this->precision(),other._precision),'0');

    size_t pos1 = num1.find_first_not_of('0');
    size_t pos2 = num1.find_last_not_of('0');
    size_t border1 = num1.size() - this->_precision - 1;
    if (pos1 <= border1 && pos2 > border1) {
        num1 = num1.substr(pos1,pos2-pos1+1);
    } else if (pos1 > border1 && pos2 > border1) {
        num1 = "0" + num1.substr(border1+1, pos2 - border1);
    } else {
        num1 = num1.substr(pos1, border1 - pos1 + 1);
    }

    size_t pos3 = num2.find_first_not_of('0');
    size_t pos4 = num2.find_last_not_of('0');
    size_t border2 = num2.size() - other._precision - 1;
    if (pos3 <= border2 && pos4 > border2) {
        num2 = num2.substr(pos3,pos4-pos3+1);
    } else if (pos3 > border2 && pos4 > border2) {
        num2 = "0" + num2.substr(border2+1, pos4 - border2);
    } else {
        num2 = num2.substr(pos3, border2 - pos3 + 1);
    }

    for (size_t i = 0; i < num1.size(); i++) {
        del += num1[i];
        if (less(num2, del)) {
            int x = 0;
            while (less(num2,del)) {
                del = subtract(del,num2);
                x++;
            }
            res += std::to_string(x);
        }
    }

    if (res.empty()) {
        res = "0";
    }

    for (int i = 0; i < result._precision; i++) {
        del += "0";
        if (less(num2,del)) {
            int x = 0;
            while (less(num2, del)) {
                del = subtract(del,num2);
                x++;
            }
            res += std::to_string(x);
        } else {
            res += "0";
        }
    }
    result._value = res;

    if ((this->_is_positive && !other._is_positive) || (!this->_is_positive && other._is_positive)) {
        if(result._value.find_first_not_of('0') != std::string::npos){
            result._is_positive = false;
        }
    }

    return result;
}

std::string BigFloat::to_string() const {
    return ((this->_is_positive ? "" : "-") + this->_value.substr(0, this->_value.size() - this->_precision) + "." + this->_value.substr(this->_value.size() - this->_precision));
}

