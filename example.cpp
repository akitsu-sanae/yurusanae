/*============================================================================
  Copyright (C) 2016 akitsu sanae
  https://github.com/akitsu-sanae/yurusanae
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/


#include "yurusanae.hpp"

int fib(int n) {
    if (n < 2)
        return n;
    else
        return fib(n-1) + fib(n-2);
}


//
//  default test
//     you can use assert_eq and assert
//
YURU_TEST(fib_test) {
    assert_eq(fib(1), 1);
    assert_eq(fib(2), 1);
    assert_eq(fib(3), 2);

    assert(fib(5) < fib(6), "fib(5) is less than fib(6)");

    assert(fib(7) + fib(8) == fib(9), "fib(7) + fib(8) = fib(9)");
}


//
//  customed test
//     you can use assert_eq, assert and neko
//
struct custom_test : public yurusanae::test_base {
    void neko() const {
        std::cout << "nyan-nyan" << std::endl;
    }
};

YURU_TEST(customed_fib_test, custom_test) {
    assert_eq(fib(1), 1);
    assert_eq(fib(2), 1);
    assert_eq(fib(3), 2);

    assert(fib(5) < fib(6), "fib(5) is less than fib(6)");

    assert(fib(7) + fib(8) == fib(9), "fib(7) + fib(8) = fib(9)");
    neko();  // customed command!!
}

//
// How to call test
//
int main() {
    fib_test(); // or fib_test{};
    customed_fib_test();  // ot customed_fib_test{};
}

