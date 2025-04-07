#include <gtest/gtest.h>
#include <sstream>
#include <list>
#include <tuple>
#ifndef __PRETTY_FUNCTION__
#include "pretty.h"
#endif
#include "mysfinae.h"


int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

using namespace std::literals::string_literals;
using namespace otus_hw4;


TEST(test_mysfinae, test_sfinae_1)
{
    otus_hw4::func(  std::vector<int>{123, 456, 789, 010} );
    otus_hw4::func(323);
    int i = 345;
    otus_hw4::func(&i);
    int& ri = i;
    otus_hw4::func(ri);
    constexpr const int ci = 345435;
    otus_hw4::func(&ci);
    const int& rci = ci;
    otus_hw4::func(rci);
    otus_hw4::func("char* 323");
    otus_hw4::func("3q23"s);
    otus_hw4::func(  std::list<int>{1456, 7489, 010} );

    otus_hw4::func(static_cast<uint8_t>(-2));

    EXPECT_TRUE( std::cout );
}


TEST(test_mysfinae, test_sfinae_pack)
{
    otus_hw4::func( std::vector<int>{123, 456, 789, 010}, 345 );
    otus_hw4::func(023);
    otus_hw4::func(323, 345);
    otus_hw4::func("char* 323", "3q23"s, 23);
    otus_hw4::func(  std::list<int>{1456, 7489, 010} );

    EXPECT_TRUE( std::cout );
}
