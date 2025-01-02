/**
 * @file TestAdapter.h
 * @brief TODO: Fill me out
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#ifdef __UNREAL__
#include "Tests/TestHarnessAdapter.h"

#define CHECK_THROWS_AS(Expression, Exception) \
    try { \
        Expression; \
        FAutomationTestFramework::Get().GetCurrentTest()->AddError(TEXT("No exception thrown")); \
    } catch (Exception &) { \
    } catch (std::exception &) { \
        FAutomationTestFramework::Get().GetCurrentTest()->AddError(TEXT("Wrong exception thrown")); \
    }

#define CHECK_NOTHROW(Expression) \
    try { \
        Expression; \
    } catch (std::exception &) { \
        FAutomationTestFramework::Get().GetCurrentTest()->AddError(TEXT("Exception thrown where none expected")); \
    }
        
#else
#include <catch2/catch_test_macros.hpp>

#define TEST_CASE_NAMED(Name, ...) TEST_CASE(__VA_ARGS__)
#endif