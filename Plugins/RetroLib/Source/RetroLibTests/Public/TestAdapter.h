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

#if !WITH_LOW_LEVEL_TESTS
#define CORO_FUNCTIONS()                                                                                               \
    template <typename F>                                                                                              \
        requires std::invocable<F> && std::convertible_to<std::invoke_result_t<F>, UE5Coro::TCoroutine<>>              \
    void CoroIt(const FString &InDescription, F &&Functor) {                                                           \
        LatentIt(InDescription,                                                                                        \
                 [this, Functor = std::forward<F>(Functor)](FDoneDelegate Delegate) -> UE5Coro::TCoroutine<> {         \
                     co_await std::invoke(std::move(Functor));                                                         \
                     Delegate.ExecuteIfBound();                                                                        \
                 });                                                                                                   \
    }

#define CHECK_THROWS_AS(Expression, Exception)                                                                         \
    try {                                                                                                              \
        Expression;                                                                                                    \
        FAutomationTestFramework::Get().GetCurrentTest()->AddError(TEXT("No exception thrown"));                       \
    } catch (Exception &) {                                                                                            \
    } catch (...) {                                                                                                    \
        FAutomationTestFramework::Get().GetCurrentTest()->AddError(TEXT("Wrong exception thrown"));                    \
    }

#define CHECK_NOTHROW(Expression)                                                                                      \
    try {                                                                                                              \
        Expression;                                                                                                    \
    } catch (...) {                                                                                                    \
        FAutomationTestFramework::Get().GetCurrentTest()->AddError(TEXT("Exception thrown where none expected"));      \
    }

#define CO_REQUIRE(...)                                                                                                \
    if (!(__VA_ARGS__)) {                                                                                              \
        FAutomationTestFramework::Get().GetCurrentTest()->AddError(TEXT("Condition failed!"));                         \
        co_return;                                                                                                     \
    }
#endif

#else
#include <catch2/catch_test_macros.hpp>

#define TEST_CASE_NAMED(Name, ...) TEST_CASE(__VA_ARGS__)
#endif