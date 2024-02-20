#include "BigFloat.h"
#include "../googletest/googletest/include/gtest/gtest.h"

TEST(Constructors, default_constructor){
    BigFloat num;

    EXPECT_EQ(num.positive(),true);
    EXPECT_EQ(num.value(),"00000000000");
    EXPECT_EQ(num.precision(),10);
}

TEST(Constructors, constructor_without_precision1){
    BigFloat num("12435.49");

    EXPECT_EQ(num.value(),"124354900000000");
    EXPECT_EQ(num.precision(),10);
}

TEST(Constructors, constructor_without_precision2){
    BigFloat num("1.0000000000000000000000000001");

    ASSERT_EQ(num.value(),"1"+std::string(10,'0'));
}

TEST(Constructors, constructor_with_short_precision){
    BigFloat num("5",5);

    ASSERT_EQ(num.value(),"500000");
}

TEST(Constructors, constructor_with_long_precision){
    BigFloat num("5",50);

    ASSERT_EQ(num.precision(),50);
}

TEST(Constructors, minus_null_test_and_leading_zeroes){
    BigFloat num("-00000.00000000");

    EXPECT_EQ(num.positive(),true);
    EXPECT_EQ(num.value(),std::string(11,'0'));
}

TEST(Operators, assigment_operator){
    BigFloat num1("1");
    BigFloat num2 = num1;

    EXPECT_EQ(num2.value(),"1"+std::string(10,'0'));
    EXPECT_EQ(num2.positive(),true);
}

TEST(Operators, equality_operator){
    BigFloat num1("1234.567890",16);
    BigFloat num2("1234.567890",16);

    ASSERT_EQ(num1==num2,true);
}

TEST(Operators, not_equality_operator){
    BigFloat num1("-1");
    BigFloat num2("1");

    ASSERT_EQ(num1 != num2, true);
}

TEST(Operators, less_operator1){
    BigFloat num1("1");
    BigFloat num2("1.0000000000000000000001",25);

    ASSERT_EQ(num1 < num2, true);
}

TEST(Operators, less_operator2){
    BigFloat num1("-0.00000001");
    BigFloat num2("0.000001");

    ASSERT_EQ(num1 < num2, true);
}

TEST(Operators, less_operator3){
    BigFloat num1("5.6666");
    BigFloat num2("5.6666");

    ASSERT_EQ(num1 < num2, false);
}

TEST(Operators, greater_operator){
    BigFloat num1("0");
    BigFloat num2("-0.01");

    ASSERT_EQ(num1 < num2, false);
}

TEST(Operators, less_or_equal_operator){
    BigFloat num1("1");
    BigFloat num2("0.999999999");

    ASSERT_EQ(num2 <= num1, true);
}

TEST(Operators, greater_or_equal_operator){
    BigFloat num1 = 25.9_bf;
    BigFloat num2 = 25.9_bf;
    BigFloat num3 = 26_bf;

    EXPECT_EQ(num1 >= num2, true);
    EXPECT_EQ(num3 >= num2, true);
}

TEST(Operators, unary_minus_operator){
    BigFloat num1("15");
    BigFloat num2("0");

    EXPECT_EQ((-num1).positive(),false);
    EXPECT_EQ((-num2).positive(),true);
}

TEST(Operators, sum_operator1){
    BigFloat num1("1.99999999");
    BigFloat num2("0.00000001");
    BigFloat num3("2");

    ASSERT_EQ((num1+num2)==num3,true);
}

TEST(Operators, sum_operator2){
    BigFloat num1("123.456");
    BigFloat num2("-123.456");
    BigFloat num3("0");

    ASSERT_EQ((num1+num2)==num3,true);
}

TEST(Operators, sum_operator3){
    BigFloat num1("1");
    BigFloat num2("0");

    ASSERT_EQ((num1+num2)==num1,true);
}

TEST(Operators, sum_operator4){
    BigFloat num1("1",30);
    BigFloat num2("1",20);
    BigFloat num3("2",30);

    EXPECT_EQ((num1+num2)==num3,true);
    EXPECT_EQ((num1+num2).precision(),30);
}

TEST(Operators, minus_operator1){
    ASSERT_EQ(BigFloat("12")-BigFloat("12.01"), BigFloat("-0.01"));
}

TEST(Operators, minus_operator2){
    ASSERT_EQ(BigFloat("1")-BigFloat("0.5")+BigFloat("0.5"),BigFloat("1"));
}

TEST(Operators, minus_operator3){
    ASSERT_EQ((BigFloat("1",87)-BigFloat("1",14)).precision(),87);
}

TEST(Operators, multiplication_operator1){
    ASSERT_EQ((BigFloat("1",14)*BigFloat("3",6)).precision(),20);
}

TEST(Operators, multiplication_operator2){
    BigFloat num1("0");
    BigFloat num2("1");

    ASSERT_EQ((num1*num2),num1);
}

TEST(Operators, multiplication_operator3){
    BigFloat num1("-1");
    BigFloat num2("1");
    BigFloat num3("-3");

    EXPECT_EQ((num1*num2).positive(),false);
    EXPECT_EQ((num1*num3).positive(),true);
}


TEST(Operators, division_operator1){
    BigFloat num1("1",16);
    BigFloat num2("1",4);

    EXPECT_EQ((num1/num2).precision(),20);
    EXPECT_EQ((num2/num1).precision(),20);
}

TEST(Operators, division_operator2){
    BigFloat num1("-1");
    BigFloat num2("1");
    BigFloat num3("-2");

    EXPECT_EQ((num1/num2).positive(),false);
    EXPECT_EQ((num1/num3).positive(),true);
}

TEST(Methods, reduce_precision){
    BigFloat num1("1245.523595923593992",30);
    num1.reduce_precision(10);

    ASSERT_EQ(num1.value(),"12455235959235");
}

TEST(Methods, members_function1){
    std::string str = "1234.56789";
    BigFloat num1(str);

    EXPECT_EQ(num1.positive(),true);
    EXPECT_EQ(num1.value(),"12345678900000");
    EXPECT_EQ(num1.precision(),10);
}

TEST(Methods, members_function2){
    std::string str = "-1234.56789111111";
    BigFloat num1(str);

    EXPECT_EQ(num1.positive(),false);
    EXPECT_EQ(num1.value(),"12345678911111");
    EXPECT_EQ(num1.precision(),10);
}

TEST(Long_Precision, sum_long_precision){
    BigFloat num1("1.111111111111122222233333344444999999999999",42);
    BigFloat num2("-111.9999999999999998888844444477771111100011021",43);
    BigFloat ans("-110.8888888888888776666511111033321111100011031",43);

    ASSERT_EQ(num1+num2,ans);
}

TEST(Long_precision, divide_long_precision){
    BigFloat num1("2.0000000000000000000000000000001",31);
    BigFloat num2("2");
    BigFloat ans("1.00000000000000000000000000000005000000000",41);

    ASSERT_EQ(num1/num2,ans);
}