// doctest.h - the lightest feature-rich C++ single-header testing framework for unit tests and TDD
// Copyright (c) 2016-2021 Viktor Kirilov
// SPDX-License-Identifier: MIT
// version 2.4.9
// https://github.com/doctest/doctest
//
// This is a simplified version for the academic project
// For full version, download from: https://github.com/doctest/doctest/releases

#ifndef DOCTEST_LIBRARY_INCLUDED
#define DOCTEST_LIBRARY_INCLUDED

// =================================================================================================
// == FEATURE DETECTION ============================================================================
// =================================================================================================

#define DOCTEST_VERSION_MAJOR 2
#define DOCTEST_VERSION_MINOR 4
#define DOCTEST_VERSION_PATCH 9
#define DOCTEST_VERSION_STR "2.4.9"
#define DOCTEST_VERSION \
    (DOCTEST_VERSION_MAJOR * 10000 + DOCTEST_VERSION_MINOR * 100 + DOCTEST_VERSION_PATCH)

// =================================================================================================
// == COMPILER VERSION =============================================================================
// =================================================================================================

#if defined(_MSC_VER) && !defined(__clang__)
#define DOCTEST_MSVC _MSC_VER
#elif defined(__clang__)
#define DOCTEST_CLANG __clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__
#elif defined(__GNUC__)
#define DOCTEST_GCC __GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__
#endif

// =================================================================================================
// == COMPILER WARNINGS HELPERS ====================================================================
// =================================================================================================

#ifdef __clang__
#define DOCTEST_PRAGMA_TO_STR(x) _Pragma(#x)
#define DOCTEST_CLANG_SUPPRESS_WARNING_PUSH _Pragma("clang diagnostic push")
#define DOCTEST_CLANG_SUPPRESS_WARNING(w) DOCTEST_PRAGMA_TO_STR(clang diagnostic ignored w)
#define DOCTEST_CLANG_SUPPRESS_WARNING_POP _Pragma("clang diagnostic pop")
#define DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH(w) \
    DOCTEST_CLANG_SUPPRESS_WARNING_PUSH DOCTEST_CLANG_SUPPRESS_WARNING(w)
#else
#define DOCTEST_CLANG_SUPPRESS_WARNING_PUSH
#define DOCTEST_CLANG_SUPPRESS_WARNING(w)
#define DOCTEST_CLANG_SUPPRESS_WARNING_POP
#define DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH(w)
#endif

#ifdef __GNUC__
#define DOCTEST_PRAGMA_TO_STR(x) _Pragma(#x)
#define DOCTEST_GCC_SUPPRESS_WARNING_PUSH _Pragma("GCC diagnostic push")
#define DOCTEST_GCC_SUPPRESS_WARNING(w) DOCTEST_PRAGMA_TO_STR(GCC diagnostic ignored w)
#define DOCTEST_GCC_SUPPRESS_WARNING_POP _Pragma("GCC diagnostic pop")
#define DOCTEST_GCC_SUPPRESS_WARNING_WITH_PUSH(w) \
    DOCTEST_GCC_SUPPRESS_WARNING_PUSH DOCTEST_GCC_SUPPRESS_WARNING(w)
#else
#define DOCTEST_GCC_SUPPRESS_WARNING_PUSH
#define DOCTEST_GCC_SUPPRESS_WARNING(w)
#define DOCTEST_GCC_SUPPRESS_WARNING_POP
#define DOCTEST_GCC_SUPPRESS_WARNING_WITH_PUSH(w)
#endif

#ifdef _MSC_VER
#define DOCTEST_MSVC_SUPPRESS_WARNING_PUSH __pragma(warning(push))
#define DOCTEST_MSVC_SUPPRESS_WARNING(w) __pragma(warning(disable : w))
#define DOCTEST_MSVC_SUPPRESS_WARNING_POP __pragma(warning(pop))
#define DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(w) \
    DOCTEST_MSVC_SUPPRESS_WARNING_PUSH DOCTEST_MSVC_SUPPRESS_WARNING(w)
#else
#define DOCTEST_MSVC_SUPPRESS_WARNING_PUSH
#define DOCTEST_MSVC_SUPPRESS_WARNING(w)
#define DOCTEST_MSVC_SUPPRESS_WARNING_POP
#define DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(w)
#endif

// =================================================================================================
// == COMPILER FEATURES DETECTION ==================================================================
// =================================================================================================

#include <iosfwd>
#include <cstddef>
#include <ostream>

#ifndef DOCTEST_CONFIG_ASSERTION_PARAMETERS_BY_VALUE
#define DOCTEST_CONFIG_ASSERTION_PARAMETERS_BY_VALUE
#endif

#define DOCTEST_EMPTY

#if defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND)
#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#endif

// =================================================================================================
// == FEATURE CONFIGURATION ========================================================================
// =================================================================================================

#ifndef DOCTEST_CONFIG_SUPER_FAST_ASSERTS
#define DOCTEST_CONFIG_SUPER_FAST_ASSERTS
#endif

// =================================================================================================
// == COMMON CODE BETWEEN THE IMPLEMENTATION AND THE TEST RUNNER ===================================
// =================================================================================================

#define DOCTEST_STRINGIFY_IMPL(x) #x
#define DOCTEST_STRINGIFY(x) DOCTEST_STRINGIFY_IMPL(x)

#define DOCTEST_ANONYMOUS(x) DOCTEST_CAT(x, __LINE__)
#define DOCTEST_CAT_IMPL(s1, s2) s1##s2
#define DOCTEST_CAT(s1, s2) DOCTEST_CAT_IMPL(s1, s2)

// =================================================================================================
// == ASSERTION MACROS =============================================================================
// =================================================================================================

#define DOCTEST_REQUIRE(expr) \
    do { \
        if (!(expr)) { \
            std::cout << "FAILURE: " << __FILE__ << ":" << __LINE__ << " - REQUIRE(" << #expr << ") failed\n"; \
            std::terminate(); \
        } \
    } while (0)

#define DOCTEST_CHECK(expr) \
    do { \
        if (!(expr)) { \
            std::cout << "FAILURE: " << __FILE__ << ":" << __LINE__ << " - CHECK(" << #expr << ") failed\n"; \
        } \
    } while (0)

#define DOCTEST_REQUIRE_EQ(lhs, rhs) \
    do { \
        if (!((lhs) == (rhs))) { \
            std::cout << "FAILURE: " << __FILE__ << ":" << __LINE__ << " - REQUIRE_EQ(" << #lhs << ", " << #rhs << ") failed - values: " << (lhs) << " != " << (rhs) << "\n"; \
            std::terminate(); \
        } \
    } while (0)

#define DOCTEST_CHECK_EQ(lhs, rhs) \
    do { \
        if (!((lhs) == (rhs))) { \
            std::cout << "FAILURE: " << __FILE__ << ":" << __LINE__ << " - CHECK_EQ(" << #lhs << ", " << #rhs << ") failed - values: " << (lhs) << " != " << (rhs) << "\n"; \
        } \
    } while (0)

#define DOCTEST_REQUIRE_NE(lhs, rhs) \
    do { \
        if ((lhs) == (rhs)) { \
            std::cout << "FAILURE: " << __FILE__ << ":" << __LINE__ << " - REQUIRE_NE(" << #lhs << ", " << #rhs << ") failed - values: " << (lhs) << " == " << (rhs) << "\n"; \
            std::terminate(); \
        } \
    } while (0)

#define DOCTEST_CHECK_NE(lhs, rhs) \
    do { \
        if ((lhs) == (rhs)) { \
            std::cout << "FAILURE: " << __FILE__ << ":" << __LINE__ << " - CHECK_NE(" << #lhs << ", " << #rhs << ") failed - values: " << (lhs) << " == " << (rhs) << "\n"; \
        } \
    } while (0)

#define DOCTEST_REQUIRE_GT(lhs, rhs) \
    do { \
        if (!((lhs) > (rhs))) { \
            std::cout << "FAILURE: " << __FILE__ << ":" << __LINE__ << " - REQUIRE_GT(" << #lhs << ", " << #rhs << ") failed - values: " << (lhs) << " <= " << (rhs) << "\n"; \
            std::terminate(); \
        } \
    } while (0)

#define DOCTEST_CHECK_GT(lhs, rhs) \
    do { \
        if (!((lhs) > (rhs))) { \
            std::cout << "FAILURE: " << __FILE__ << ":" << __LINE__ << " - CHECK_GT(" << #lhs << ", " << #rhs << ") failed - values: " << (lhs) << " <= " << (rhs) << "\n"; \
        } \
    } while (0)

#define DOCTEST_REQUIRE_LT(lhs, rhs) \
    do { \
        if (!((lhs) < (rhs))) { \
            std::cout << "FAILURE: " << __FILE__ << ":" << __LINE__ << " - REQUIRE_LT(" << #lhs << ", " << #rhs << ") failed - values: " << (lhs) << " >= " << (rhs) << "\n"; \
            std::terminate(); \
        } \
    } while (0)

#define DOCTEST_CHECK_LT(lhs, rhs) \
    do { \
        if (!((lhs) < (rhs))) { \
            std::cout << "FAILURE: " << __FILE__ << ":" << __LINE__ << " - CHECK_LT(" << #lhs << ", " << #rhs << ") failed - values: " << (lhs) << " >= " << (rhs) << "\n"; \
        } \
    } while (0)

#define DOCTEST_REQUIRE_GE(lhs, rhs) \
    do { \
        if (!((lhs) >= (rhs))) { \
            std::cout << "FAILURE: " << __FILE__ << ":" << __LINE__ << " - REQUIRE_GE(" << #lhs << ", " << #rhs << ") failed - values: " << (lhs) << " < " << (rhs) << "\n"; \
            std::terminate(); \
        } \
    } while (0)

#define DOCTEST_CHECK_GE(lhs, rhs) \
    do { \
        if (!((lhs) >= (rhs))) { \
            std::cout << "FAILURE: " << __FILE__ << ":" << __LINE__ << " - CHECK_GE(" << #lhs << ", " << #rhs << ") failed - values: " << (lhs) << " < " << (rhs) << "\n"; \
        } \
    } while (0)

#define DOCTEST_REQUIRE_LE(lhs, rhs) \
    do { \
        if (!((lhs) <= (rhs))) { \
            std::cout << "FAILURE: " << __FILE__ << ":" << __LINE__ << " - REQUIRE_LE(" << #lhs << ", " << #rhs << ") failed - values: " << (lhs) << " > " << (rhs) << "\n"; \
            std::terminate(); \
        } \
    } while (0)

#define DOCTEST_CHECK_LE(lhs, rhs) \
    do { \
        if (!((lhs) <= (rhs))) { \
            std::cout << "FAILURE: " << __FILE__ << ":" << __LINE__ << " - CHECK_LE(" << #lhs << ", " << #rhs << ") failed - values: " << (lhs) << " > " << (rhs) << "\n"; \
        } \
    } while (0)

#define DOCTEST_REQUIRE_THROWS(expr) \
    do { \
        bool threw = false; \
        try { \
            (expr); \
        } catch (...) { \
            threw = true; \
        } \
        if (!threw) { \
            std::cout << "FAILURE: " << __FILE__ << ":" << __LINE__ << " - REQUIRE_THROWS(" << #expr << ") failed - no exception was thrown\n"; \
            std::terminate(); \
        } \
    } while (0)

#define DOCTEST_CHECK_THROWS(expr) \
    do { \
        bool threw = false; \
        try { \
            (expr); \
        } catch (...) { \
            threw = true; \
        } \
        if (!threw) { \
            std::cout << "FAILURE: " << __FILE__ << ":" << __LINE__ << " - CHECK_THROWS(" << #expr << ") failed - no exception was thrown\n"; \
        } \
    } while (0)

#define DOCTEST_REQUIRE_NOTHROW(expr) \
    do { \
        try { \
            (expr); \
        } catch (...) { \
            std::cout << "FAILURE: " << __FILE__ << ":" << __LINE__ << " - REQUIRE_NOTHROW(" << #expr << ") failed - an exception was thrown\n"; \
            std::terminate(); \
        } \
    } while (0)

#define DOCTEST_CHECK_NOTHROW(expr) \
    do { \
        try { \
            (expr); \
        } catch (...) { \
            std::cout << "FAILURE: " << __FILE__ << ":" << __LINE__ << " - CHECK_NOTHROW(" << #expr << ") failed - an exception was thrown\n"; \
        } \
    } while (0)

// =================================================================================================
// == TEST CASE MACROS =============================================================================
// =================================================================================================

#define DOCTEST_TEST_CASE(name) \
    static void DOCTEST_ANONYMOUS(DOCTEST_TEST_FUNC_)(); \
    namespace { \
        struct DOCTEST_ANONYMOUS(DOCTEST_TEST_CLASS_) { \
            DOCTEST_ANONYMOUS(DOCTEST_TEST_CLASS_)() { \
                std::cout << "Running test: " << name << "\n"; \
                DOCTEST_ANONYMOUS(DOCTEST_TEST_FUNC_)(); \
                std::cout << "Test completed: " << name << "\n"; \
            } \
        }; \
        static DOCTEST_ANONYMOUS(DOCTEST_TEST_CLASS_) DOCTEST_ANONYMOUS(DOCTEST_TEST_INSTANCE_); \
    } \
    static void DOCTEST_ANONYMOUS(DOCTEST_TEST_FUNC_)()

#define DOCTEST_SUBCASE(name) \
    do { \
        std::cout << "  Subcase: " << name << "\n"; \
    } while (0); \
    if (true)

// =================================================================================================
// == MAIN FUNCTION IMPLEMENTATION =================================================================
// =================================================================================================

#ifdef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <iostream>

namespace doctest {
    int main_impl(int argc, char** argv) {
        (void)argc;
        (void)argv;
        
        std::cout << "===============================================================================\n";
        std::cout << "doctest version is \"" << DOCTEST_VERSION_STR << "\"\n";
        std::cout << "running tests...\n";
        std::cout << "===============================================================================\n";
        
        // Tests are run automatically through static initialization
        
        std::cout << "===============================================================================\n";
        std::cout << "All tests completed!\n";
        std::cout << "===============================================================================\n";
        
        return 0;
    }
}

int main(int argc, char** argv) {
    return doctest::main_impl(argc, argv);
}

#endif // DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#endif // DOCTEST_LIBRARY_INCLUDED
