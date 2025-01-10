// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#if !RETROLIB_WITH_MODULES
#include <cmath>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro {
    /**
     * The number of seconds in a minute.
     */
    RETROLIB_EXPORT static constexpr float SECONDS_PER_MINUTE = 60.f;

    /**
     * Linear interpolation between two values, given the duration of the change and the time passed since the start of
     * the change (delta)
     *
     * @param StartValue The starting value for the change
     * @param EndValue Where the value should be at the end of the duration
     * @param Duration The duration of the change
     * @param Delta The change in time
     * @return The interpolated value
     */
    RETROLIB_EXPORT template <typename A, typename B, typename C, typename D>
        requires std::is_arithmetic_v<A> && std::is_arithmetic_v<B> && std::is_arithmetic_v<C> &&
                     std::is_arithmetic_v<D> &&
                     (std::is_floating_point_v<A> || std::is_floating_point_v<B> || std::is_floating_point_v<C> ||
                      std::is_floating_point_v<D>)
    constexpr auto LinearInterpolation(A StartValue, B EndValue, C Duration,
                                       D Delta) -> decltype(StartValue * EndValue * Duration * Delta) {
        if (Duration <= 0)
            return EndValue;

        if (Delta <= 0)
            return StartValue;

        if (Delta >= Duration)
            return EndValue;

        return StartValue + (EndValue - StartValue) * Delta / Duration;
    }

    /**
     * Take the power of two exponents and return the result as an integer.
     *
     * @param Base The base of the operation
     * @param Exponent The exponent of the operation
     * @return The result of the exponentiation
     */
    RETROLIB_EXPORT inline int Pow(int Base, int Exponent) {
        return static_cast<int>(std::pow(Base, Exponent));
    }
} // namespace Retro