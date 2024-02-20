#include "BigFloat.h"
#include "../googletest/googletest/include/gtest/gtest.h"

TEST(Constructors, default_constructor){
    BigFloat num;

    EXPECT_EQ(num.positive(),true);
    EXPECT_EQ(num.value(),"00000000000");
    EXPECT_EQ(num.precision(),10);
}

TEST(Constructors, constructor_without_precision){
    BigFloat num("12435.49");

}