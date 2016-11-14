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
    virtual ~test_base() {}
    virtual std::string name() const = 0;

    template<typename T>
    void assert_eq(T const& lhs, T const& rhs) const {
        if (lhs == rhs)
            return;
        std::stringstream ss;
        ss << "\033[31mtest[" << name() <<  "] fail.. \033[39m ";
        ss << "assertion: " << lhs << " == " << rhs << std::endl;
        throw test_fail{ss.str()};
    };
    template<typename T>
    void assert_lt(T const& lhs, T const& rhs) const {
        if (lhs < rhs)
            return;
        std::stringstream ss;
        ss << "\033[31mtest[" << name() <<  "] fail.. \033[39m ";
        ss << "assertion: " << lhs << " < " << rhs << std::endl;
        throw test_fail{ss.str()};
    }

    template<typename F, typename ... Args>
    void assert_if(F&& f, Args&& ... args) const {
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

}

#define YURU_TEST(x) \
    struct x : public yurusanae::test_base{  \
        explicit x(); \
        std::string name() const override { \
            return #x; \
        } \
    }; \
    inline x::x()



#endif
