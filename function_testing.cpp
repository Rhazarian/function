#include "function.h"
#include "gtest/gtest.h"

TEST(correctness, simple_lambda)
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

TEST(correctness, simple_func_ptr)
{
    function<int(int)> func = test_a;
    EXPECT_EQ(20, func(10));
}

TEST(correctness, assigment)
{
    function<int(int)> func_a = test_a;
    function<int(int)> empty;
    empty = func_a;
    EXPECT_EQ(20, empty(10));
}

TEST(correctness, assigment_func_ptr)
{
    function<int(int)> func;
    func = test_a;
    EXPECT_EQ(20, func(10));
}

TEST(correctness, bool_conversion)
{
    function<int(int)> func_empty;
    EXPECT_EQ(false, !!func_empty);
    function<int(int)> func = test_a;
    EXPECT_EQ(true, !!func);
}

TEST(correctness, move)
{
    function<int(int)> func_a = test_a;
    EXPECT_EQ(true, !!func_a);
    function<int(int)> func(std::move(func_a));
    EXPECT_EQ(false, !!func_a);
    EXPECT_EQ(true, !!func);
    EXPECT_EQ(20, func(10));
}

TEST(correctness, assign_fptr_to_lambda)
{
    function<int(int)> func = [](int a) {
        return a + 10;
    };
    EXPECT_EQ(20, func(10));
    func = test_a;
    EXPECT_EQ(20, func(10));
}

TEST(correctness, assign_labmda_to_fptr)
{
    function<int(int)> func = test_a;
    EXPECT_EQ(20, func(10));
    func = [](int a) {
        return a + 10;
    };
    EXPECT_EQ(20, func(10));
}

TEST(correctness, assign_nullptr_to_fptr)
{
    function<int(int)> func = test_a;
    EXPECT_EQ(20, func(10));
    func = nullptr;
    EXPECT_EQ(false, !!func);
}