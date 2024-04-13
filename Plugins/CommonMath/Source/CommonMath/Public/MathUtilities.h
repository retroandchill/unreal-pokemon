// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MathUtilities.generated.h"

/**
 * Common utility functions for math
 */
UCLASS()
class COMMONMATH_API UMathUtilities : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    /**
     * The number of seconds in a minute.
     */
    static constexpr float SECONDS_PER_MINUTE = 60.f;

    /**
     * Linear interpolation between two values, given the duration of the change and the time passed since the start of
     * the change (delta)
     * @param StartValue The starting value for the change
     * @param EndValue Where the value should be at the end of the duration
     * @param Duration The duration of the change
     * @param Delta The change in time
     * @return The interpolated value
     */
    template <typename A, typename B, typename C, typename D>
        requires std::is_arithmetic_v<A> && std::is_arithmetic_v<B> && std::is_arithmetic_v<C> &&
                     std::is_arithmetic_v<D> &&
                     (std::is_floating_point_v<A> || std::is_floating_point_v<B> || std::is_floating_point_v<C> ||
                      std::is_floating_point_v<D>)
    static auto LinearInterpolation(A StartValue, B EndValue, C Duration,
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
     * @param Base The base of the operation
     * @param Exponent The exponent of the operation
     * @return The result of the exponentiation
     */
    static int32 Pow(int32 Base, int32 Exponent);

    /**
     * Find the midpoint between two vector objects
     * @param Start The first vector to check
     * @param End The second vector to check
     * @return The midpoint between the two vectors
     */
    UFUNCTION(BlueprintPure, Category = "Math|Vector")
    static FVector Midpoint(const FVector &Start, const FVector &End);
};
