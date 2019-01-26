#include "function.h"
#include "gtest/gtest.h"

//template<typename T>
//using function = my::function<T>;

TEST(correctess, simple_lambda)
{
    function<int(int)> func = [](int a) {
        return a + 10;
    };
    EXPECT_EQ(20, func(10));
}

namespace {
    int test_a(int a) {
        return a + 10;
    }
}

TEST(correctess, simple_func_ptr)
{
    function<int(int)> func = test_a;
    EXPECT_EQ(20, func(10));
}

TEST(correctess, assigment)
{
    function<int(int)> func_a = test_a;
    function<int(int)> empty;
    empty = func_a;
    EXPECT_EQ(20, empty(10));
}

TEST(correctess, assigment_func_ptr)
{
    function<int(int)> func;
    func = test_a;
    EXPECT_EQ(20, func(10));
}

TEST(correctess, bool_conversion)
{
    function<int(int)> func_empty;
    EXPECT_EQ(false, !!func_empty);
    function<int(int)> func = test_a;
    EXPECT_EQ(true, !!func);
}

TEST(correctess, move)
{
    function<int(int)> func_a = test_a;
    EXPECT_EQ(true, !!func_a);
    function<int(int)> func(std::move(func_a));
    EXPECT_EQ(false, !!func_a);
    EXPECT_EQ(true, !!func);
    EXPECT_EQ(20, func(10));
}