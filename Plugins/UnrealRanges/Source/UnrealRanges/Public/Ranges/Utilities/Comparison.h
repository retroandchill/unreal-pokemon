// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Concepts/Comparison.h"
#include "Ranges/Functional/Overloaded.h"

namespace UE::Ranges {

    struct FEquals {
        template <typename T, typename U = T>
            requires std::equality_comparable_with<T, U>
        constexpr bool operator()(const T& LHS, const U& RHS) const {
            return LHS == RHS;
        }
    };

    constexpr FEquals Equals;

    struct FNotEqual {
        template <typename T, typename U = T>
            requires InequalityComparable<T, U>
        constexpr bool operator()(const T& LHS, const U& RHS) const {
            return LHS != RHS;
        }
    };

    constexpr FNotEqual NotEqual;

    struct FLessThan {
        template <typename T, typename U = T>
            requires LessThanComparable<T, U>
        constexpr bool operator()(const T& LHS, const U& RHS) const {
            return LHS < RHS;
        }
    };

    constexpr FLessThan LessThan;

    struct FLessThanOrEqual {
        template <typename T, typename U = T>
            requires LessThanOrEqualComparable<T, U>
        constexpr bool operator()(const T& LHS, const U& RHS) const {
            return LHS <= RHS;
        }
    };

    constexpr FLessThanOrEqual LessThanOrEqual;

    struct FGreaterThan {
        template <typename T, typename U = T>
            requires GreaterThanComparable<T, U>
        constexpr bool operator()(const T& LHS, const U& RHS) const {
            return LHS > RHS;
        }
    };

    constexpr FGreaterThan GreaterThan;

    struct FGreaterThanOrEqual {
        template <typename T, typename U = T>
            requires GreaterThanOrEqualComparable<T, U>
        constexpr bool operator()(const T& LHS, const U& RHS) const {
            return LHS >= RHS;
        }
    };

    constexpr FGreaterThanOrEqual GreaterThanOrEqual;
    
}