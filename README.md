# Yurusanae

Yurusanae is C++ header only testing library.  

# Install

`#include "yurusanae.hpp"`

# Usage

Think the case of testing Fibonacci sequence.  
Function fib is defined like this:
```cpp
int fib(int n) {
    if (n < 2)
        return n;
    else
        return fib(n-1) + fib(n-2);
}
```

You can write testing code for this fib function by yurusanae like this:
```cpp
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
```

then, calling this test:
```cpp
int main() {
    fib_test(); // or fib_test{};
}
```

Yurusanae is useless? Ok, you can write custom test like this:
```cpp
struct custom_test : public yurusanae::test_base {
    // new command
    void neko() const {
        std::cout << "nyan-nyan" << std::endl;
    }
};

YURU_TEST(customed_fib_test, custom_test) {
    neko(); // use customed command
}
```

and, calling this test as same as default one:
```cpp
int main() {
    customed_fib_test();
}
```

# Copyright
Copyright (C) 2016 akitsu sanae.  
Distributed under the Boost Software License, Version 1.0. 
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost/org/LICENSE_1_0.txt)  


