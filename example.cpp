/*============================================================================
  Copyright (C) 2016 akitsu sanae
  https://github.com/akitsu-sanae/yurusanae
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/


#include "yurusanae.hpp"

int fib(int n) {
    if (n < 2)
        return n;
    else
        return fib(n-1) + fib(n-2);
}

YURU_TEST(fib_test) {
    assert_eq(fib(1), 1);
    assert_eq(fib(2), 1);
    assert_eq(fib(3), 2);

    assert_lt(fib(5), fib(6));

    assert_if([](int n3, int n1, int n2) {
            return n3 == n1 + n2;
            }, fib(6), fib(4), fib(5));

    debug(fib(4) < 12, "fib(4) is less than 12"); // to std::cerr
    debug(fib(5) > 42, "fib(5) is greater than 42", std::cout);
}

int main() {
    fib_test(); // or fib_test{};
}

