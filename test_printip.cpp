#include <gtest/gtest.h>
#include <sstream>
#include <list>
#include <tuple>
#ifndef __PRETTY_FUNCTION__
#include "pretty.h"
#endif
#include "myprintip.h"


int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

using namespace std::literals::string_literals;
using otus_hw4::print_ip0;

TEST(test_myprintip, test_print_ip0_1)
{
    print_ip0(std::cout, "LPCSTR Hello, World!") << std::endl;
    print_ip0(std::cout, "string Hello, World!"s) << std::endl;

    print_ip0(std::cout, 42, 2) << std::endl;
    print_ip0(std::cout, -1) << std::endl;
    print_ip0(std::cout, static_cast<uint8_t>(-1)) << std::endl;
    print_ip0(std::cout, "Hello, World!"s, 10U) << std::endl;
    print_ip0(std::cout, 15LL, 16U, 17);
    print_ip0(std::cout, 400) << std::endl;
    print_ip0(std::cout, 300) << std::endl;

    EXPECT_TRUE( std::cout );
}

TEST(test_myprintip, test_print_ip0_vec)
{
    std::vector<int> v{123, 456, 789, 010, -2};
    print_ip0(std::cout, v) << std::endl;

    EXPECT_TRUE( std::cout );
}

TEST(test_myprintip, test_print_ip0_list)
{
    std::list<int> L{-3, 1789, 2010, -10};
    print_ip0(std::cout, L) << std::endl;

    EXPECT_TRUE( std::cout );
}

