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
    template<typename T>
    void assert_lt(T const& lhs, T const& rhs) {
        assert_count++;
        if (lhs < rhs)
            return;
        std::stringstream ss;
        ss << "\033[31mtest[" << name() <<  "] fail.. \033[39m ";
        ss << "assertion: " << lhs << " < " << rhs << std::endl;
        throw test_fail{ss.str()};
    }

    template<typename F, typename ... Args>
    void assert_if(F&& f, Args&& ... args) {
        assert_count++;
        if (f(std::forward<Args>(args) ...))
            return;
        std::stringstream ss;
        ss << "\033[31mtest[" << name() <<  "] fail.. \033[39m ";
        ss << "assertion: f(" << detail::format(args ...) << ")" << std::endl;
        throw test_fail{ss.str()};
    }

    void debug(bool cond, std::string const& msg, std::ostream& os = std::cerr) const {
        if (!cond)
            os << "\033[32m[debug in " << name() <<  "]\033[39m " << msg << std::endl;
    }
};

template<size_t N = 100, typename T = std::chrono::milliseconds>
struct benchmark_base {
    virtual std::string name() const = 0;
    virtual void exec_impl() const = 0;
    void exec() const {
        auto buf = std::cout.rdbuf();
        std::stringstream dummy;
        std::cout.rdbuf(dummy.rdbuf());
        std::cerr << "\033[32m" << std::flush;
        auto start = std::chrono::system_clock::now();
        for (int i=0; i<N; i++) {
            std::cerr << "\r\033[K";
            std::cerr << "[benchmark for " << name() << "] progress: " << 100.0 * (double)i/N << "%" << std::flush;
            exec_impl();
        }
        auto end = std::chrono::system_clock::now();
        std::cout.rdbuf(buf);
        auto diff = std::chrono::duration_cast<T>(end - start).count() / N;
        std::cerr << "\r\033[K";
        std::cerr << "[benchmark for " << name() <<  ": " << diff << "ms]";
        std::cerr << "\033[39m" << std::endl;
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
    struct x : public yurusanae::benchmark_base<> { \
        std::string name() const override { \
            return #x; \
        } \
        void exec_impl() const override; \
    }; \
    void x::exec_impl() const

#define YURU_BENCH_DETAIL_2(x, N) \
    struct x : public yurusanae::benchmark_base<N> { \
        std::string name() const override { \
            return #x; \
        } \
        void exec_impl() const override; \
    }; \
    void x::exec_impl() const

#define YURU_BENCH_DETAIL_3(x, N, T) \
    struct x : public yurusanae::benchmark_base<N, T> { \
        std::string name() const override { \
            return #x; \
        } \
        void exec_impl() const override; \
    }; \
    void x::exec_impl() const

#define YURUSANAE_BENCH_SELECTER(_1, _2, _3, COUNT, ...) COUNT
#define YURU_BENCH(...) YURUSANAE_BENCH_SELECTER(__VA_ARGS__, YUTU_BENCH_DETAIL_3, YURU_BENCH_DETAIL_2, YURU_BENCH_DETAIL_1) (__VA_ARGS__)

#endif
