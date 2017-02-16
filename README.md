# Yurusanae

Yurusanae is C++ header only testing/benchmark library.  

# Install

`#include "yurusanae.hpp"`

# Usage

### Unit testing

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

### Benchmark

Here are two Fibonacci functions.

```cpp
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
```
First `fib1` is simple and slow. The other `fib2` is very very fast.  
Let's compare these performance!  
You can write benchmark code with yurusanae like this:
```cpp
YURU_BENCH(fib1_bench) {
    std::cout << fib1(36) << std::endl;
}

YURU_BENCH(fib2_bench, 100000) {
    std::cout << fib2(36) << std::endl;
}
```
`fib2_bench`'s second argument is how many times run the code. default is 100.  

And, run the benchmark:
```cpp
int main() {
    fib1_bench{}.exec();
    fib2_bench{}.exec();
}
```
output:
```
[benchmark for fib1_bench: 118ms]
[benchmark for fib2_bench: 0ms]
```

# Copyright
Copyright (C) 2016 akitsu sanae.  
Distributed under the Boost Software License, Version 1.0. 
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost/org/LICENSE_1_0.txt)  


