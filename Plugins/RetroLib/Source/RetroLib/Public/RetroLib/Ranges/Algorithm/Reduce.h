/**
 * @file Reduce.h
 * @brief Functional chain component used for reducing a range down to a single value.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#include "RetroLib/Functional/CreateBinding.h"
#include "RetroLib/Functional/ExtensionMethods.h"
#include "RetroLib/Ranges/FeatureBridge.h"

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro::Ranges {
    struct FReduceInvoker {
        /**
         * @brief Reduces a range of elements into a single value by applying a functor
         * iteratively.
         *
         * This function takes a range, an initial identity value, and a functor
         * to combine the elements in the range into a single aggregated result.
         * The functor is applied in left-to-right order.
         *
         * @tparam R The type of the range.
         * @tparam I The type of the identity value.
         * @tparam F The type of the functor.
         * @param Range The range of elements to be reduced.
         * @param Identity The initial identity value used in the reduction process.
         * @param Functor A callable object that specifies how to combine two elements.
         * @return The final aggregated result of the reduction operation.
         */
        RETROLIB_EXPORT template <std::ranges::input_range R, typename I, HasFunctionCallOperator F>
            requires std::invocable<F, I, TRangeCommonReference<R>> &&
                     std::convertible_to<std::invoke_result_t<F, I, TRangeCommonReference<R>>, I>
        constexpr auto operator()(R &&Range, I &&Identity, F Functor) const {
            auto Result = std::forward<I>(Identity);
            for (auto &&Value : std::forward<R>(Range)) {
                Result = std::invoke(Functor, std::move(Result), std::forward<decltype(Value)>(Value));
            }
            return Result;
        }
    };

    RETROLIB_EXPORT constexpr auto Reduce = ExtensionMethod<FReduceInvoker{}>;
} // namespace Retro::Ranges