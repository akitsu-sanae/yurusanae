/*============================================================================
  Copyright (C) 2016 akitsu sanae
  https://github.com/akitsu-sanae/yurusanae
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/



#ifndef YURUSANAE_HPP
#define YURUSANAE_HPP

#include <exception>
#include <iostream>
#include <sstream>
#include <chrono>
#include <array>

namespace yurusanae {

namespace detail {

inline std::string format() {
    return "";
}

template<typename Head, typename ... Args>
inline std::string format(Head const& head, Args const& ... args) {
    std::stringstream ss;
    ss << head << ", ";
    return ss.str() + format(args ...);
}

struct benchmark_bar {
    benchmark_bar() {
        std::cerr << "     bench name     |    average(ms)     |    minimum(ms)     |    maximum(ms)     " << std::endl;
        std::cerr << "-----------------------------------------------------------------------------------" << std::endl;
    }
    static void make() {
        static auto dummy = benchmark_bar{};
    }
};

}

struct test_fail : public std::exception {
    explicit test_fail(std::string msg) :
        message(msg)
    {}
    std::string message;
    const char* what() const noexcept override{
        return message.c_str();
    }
};

struct test_base {
    virtual ~test_base() = default;
    virtual std::string name() const = 0;
    int assert_count = 0;

    template<typename T>
    void assert_eq(T const& lhs, T const& rhs) {
        assert_count++;
        if (lhs == rhs)
            return;
        std::stringstream ss;
        ss << "\033[31mtest[" << name() <<  "] fail.. \033[39m ";
        ss << "assertion: " << lhs << " == " << rhs << std::endl;
        throw test_fail{ss.str()};
    };

    void assert(bool is_ok, std::string const& msg = "") {
        assert_count++;
        if (is_ok)
            return;
        std::stringstream ss;
        ss << "\033[31mtest[" << name() <<  "] fail.. \033[39m ";
        ss << "assertion: " << msg << std::endl;
        throw test_fail{ss.str()};
    }
};

template<size_t SMALL_N, size_t BIG_N>
struct benchmark_base {
    virtual std::string name() const = 0;
    virtual void exec_impl() const = 0;
    void exec() const {
        detail::benchmark_bar::make();
        auto buf = std::cout.rdbuf();
        std::stringstream dummy;
        std::cout.rdbuf(dummy.rdbuf());
        std::array<double, BIG_N> times{};
        for (int big_i=0; big_i<BIG_N; big_i++) {
            auto start = std::chrono::system_clock::now();
            for (int small_i=0; small_i<SMALL_N; small_i++) {
                std::cerr << "\r\033[K";
                std::cerr << "[benchmark for " << name() << "] progress: " << 100.0 * (double)(big_i*SMALL_N + small_i)/(BIG_N * SMALL_N) << "%" << std::flush;
                exec_impl();
            }
            auto end = std::chrono::system_clock::now();
            times[big_i] = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        }
        std::cout.rdbuf(buf);
        double sum = 0.0;
        auto min = times.begin();
        auto max = times.begin();
        for (auto it=times.begin(); it!=times.end(); ++it) {
            sum += *it;
            if (*min > *it)
                min = it;
            if (*max < *it)
                max = it;
        }
        std::cerr << "\r\033[K";
        std::printf("%20s|%20f|%20f|%20f\n", name().c_str(), sum / BIG_N, *min, *max);
    }
    virtual ~benchmark_base() = default;
};

}

#define YURU_TEST_DETAIL_1(x) \
    struct x : public yurusanae::test_base {  \
        explicit x(); \
        std::string name() const override { \
            return #x; \
        } \
        ~x() { \
            std::cerr << "\033[33m[passed " << name() << ": number of assertions: " << assert_count <<  "]\033[39m" << std::endl; \
        } \
    }; \
    inline x::x()

#define YURU_TEST_DETAIL_2(x, base) \
    struct x : public base {  \
        explicit x(); \
        std::string name() const override { \
            return #x; \
        } \
        ~x() { \
            std::cerr << "\033[33m[passed " << name() << ": number of assertions: " << assert_count <<  "]\033[39m" << std::endl; \
        } \
    }; \
    inline x::x()

#define YURUSANAE_TEST_SELECTER(_1, _2, COUNT, ...) COUNT
#define YURU_TEST(...) YURUSANAE_TEST_SELECTER(__VA_ARGS__, YURU_TEST_DETAIL_2, YURU_TEST_DETAIL_1) (__VA_ARGS__)

#define YURU_BENCH_DETAIL_1(x) \
    struct x : public yurusanae::benchmark_base<10, 10> { \
        std::string name() const override { \
            return #x; \
        } \
        void exec_impl() const override; \
    }; \
    void x::exec_impl() const

#define YURU_BENCH_DETAIL_2(x, N) \
    struct x : public yurusanae::benchmark_base<N, 10> { \
        std::string name() const override { \
            return #x; \
        } \
        void exec_impl() const override; \
    }; \
    void x::exec_impl() const

#define YURU_BENCH_DETAIL_3(x, SMALL_N, BIG_N) \
    struct x : public yurusanae::benchmark_base<SMALL_N, BIG_N> { \
        std::string name() const override { \
            return #x; \
        } \
        void exec_impl() const override; \
    }; \
    void x::exec_impl() const

#define YURUSANAE_BENCH_SELECTER(_1, _2, _3, COUNT, ...) COUNT
#define YURU_BENCH(...) YURUSANAE_BENCH_SELECTER(__VA_ARGS__, YURU_BENCH_DETAIL_3, YURU_BENCH_DETAIL_2, YURU_BENCH_DETAIL_1) (__VA_ARGS__)

#endif
