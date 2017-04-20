/*============================================================================
  Copyright (C) 2017 akitsu sanae
  https://github.com/akitsu-sanae/yurusanae
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/

#include <iostream>
#include "yurusanae.hpp"

int fib1(int n) {
    if (n <= 2)
        return n;
    else
        return fib1(n-1) + fib1(n-2);
}

int fib2(int n) {
    auto n1 = 1;
    auto n2 = 1;
    auto result = 1;
    for (int i=0; i<n; i++) {
        result = n1 + n2;
        n2 = n1;
        n1 = result;
    }
    return result;
}

YURU_BENCH(fib1_bench) {
    std::cout << fib1(36) << std::endl;
}

YURU_BENCH(fib2_small_bench) {
    std::cout << fib2(36) << std::endl;
}

YURU_BENCH(fib2_big_bench, 1000) {
    std::cout << fib2(36) << std::endl;
}

int main() {
    fib1_bench{}.exec();
    fib2_small_bench{}.exec();
    fib2_big_bench{}.exec();
}


