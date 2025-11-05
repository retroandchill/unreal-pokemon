// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include <string>

#include "MoveCheckResult.generated.h"

class IInteractable;

/**
 * The result struct of a movement check validation. Can be returned as a structured binding.
 */
USTRUCT(BlueprintType)
struct GRIDBASED2D_API FMoveCheckResult {
    GENERATED_BODY()

    /**
     * Can the character move to the target tile?
     */
    UPROPERTY(BlueprintReadOnly, Category = "Move Check")
    bool bCanMove = true;

    /**
     * The list of interactable actors that are in the movement path.
     */
    UPROPERTY(BlueprintReadOnly, Category = "Move Check")
    TArray<TScriptInterface<IInteractable>> FoundActors;

    /**
     * Used to enable getting the element as part of a structured binding
     * @tparam Index The index to get the element of
     * @return The found result
     */
    template <std::size_t Index>
    std::tuple_element_t<Index, FMoveCheckResult> &get() {
        if constexpr (Index == 0)
            return bCanMove;
        if constexpr (Index == 1)
            return FoundActors;
    }
};

// Structured binding metadata declaration
namespace std {
    template <>
    struct tuple_size<FMoveCheckResult> : integral_constant<size_t, 2> {};

    template <>
    struct tuple_element<0, FMoveCheckResult> {
        using type = bool;
    };

    template <>
    struct tuple_element<1, FMoveCheckResult> {
        using type = TArray<TScriptInterface<IInteractable>>;
    };

    template <size_t Index>
    struct tuple_element<Index, FMoveCheckResult> : conditional<Index == 0, std::string, int> {
        static_assert(Index < 2, "Index out of bounds for MoveCheckResult");
    };
} // namespace std
