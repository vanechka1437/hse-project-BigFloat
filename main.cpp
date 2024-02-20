#include "BigFloat.h"
#include <ctime>

BigFloat pi(int &precision) {
    BigFloat res("0",precision);
    BigFloat x("16",precision);
    for (int i = 0; i < precision; i++) {
        BigFloat c = (BigFloat("4",precision)/BigFloat(std::to_string(8*i+1),precision));
        c = c - (BigFloat("2",precision)/BigFloat(std::to_string(8*i+4),precision));
        c = c - (BigFloat("1",precision)/BigFloat(std::to_string(8*i+5),precision));
        c = c - (BigFloat("1",precision)/BigFloat(std::to_string(8*i+6),precision));
        x = x/BigFloat("16",precision);
        x.reduce_precision(precision);
        res = res + c*x;
    }
    res.reduce_precision(precision);
    return res;
}

int main() {
    int PRECISION = 100;
    clock_t start_time = clock();
    BigFloat result = pi(PRECISION);
    clock_t end_time = clock();
    double elapsed_time = static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC;
    std::cout << result << std::endl;
    std::cout << "time: " << elapsed_time << " sec" << std::endl;
    return 0;
}