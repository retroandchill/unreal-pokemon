// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/RangeConcepts.h"
#include "Ranges/TerminalClosure.h"

namespace UE::Ranges {

    /**
     * Terminal invoker used to convert a range of FStrings into a single unified string.
     */
    struct FToStringInvoker {
        template <typename T>
            requires std::convertible_to<T, FStringView>
        constexpr explicit FToStringInvoker(T &&Conjunction) : Conjunction(Forward<T>(Conjunction)) {
        }

        /**
         * Perform the string concatenation on the range.
         * @tparam R The type of range to perform the concatenation on.
         * @param Range The range to concatenate.
         * @return The combined string.
         */
        template <typename R>
            requires (ranges::input_range<R> || UEContainer<R>) && std::is_convertible_v<ranges::range_value_t<R>, FString>
        constexpr auto operator()(R &&Range) const {
            return FString::Join(Forward<R>(Range), Conjunction.GetData());
        }

      private:
        FStringView Conjunction;
    };

    /**
     * Pipe terminal used to combine strings together into a single string.
     */
    struct FToString {
        /**
         * Create a terminal closure to concatenate the range elements into a string.
         * @tparam T The type of string data to convert into a view.
         * @param Conjunction The conjunction to place between each element.
         * @return The terminal closure.
         */
        template <typename T>
            requires std::convertible_to<T, FStringView>
        auto operator()(T &&Conjunction) const {
            return TTerminalClosure(FToStringInvoker(Forward<T>(Conjunction)));
        }
    };

    /**
     * Pipe terminal used to combine strings together into a single string. Used by passing in any type that can be
     * converted into FStringView.
     */
    inline constexpr FToString ToString;

} // namespace UE::Ranges