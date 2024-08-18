// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Optional/OptionalClosure.h"
#include <range/v3/view/view.hpp>
#include "Map.h"

namespace UE::Ranges {
    
    struct FExtract {

        template <typename R, typename T = ranges::range_value_t<R>>
            requires ranges::input_range<R> && Optionals::UEOptional<std::remove_cvref_t<T>>
        auto operator()(R &&Range) const {
            return Range | Map([](T&& Optional) {
                check(Optional.IsSet())
                return Optional.GetPtrOrNull();
            });
        }
        
    };

    /**
     * Performs a checked extraction of an optional either extracting out the value (if by value) or a pointer
     * (if by reference)
     */
    constexpr ranges::views::view_closure<FExtract> Extract;
    
}
