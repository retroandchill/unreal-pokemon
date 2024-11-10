// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

namespace UE::Ranges {

    /**
     * Concept used to verify if an object can be serialized directly using an archive object.
     *
     * @tparam T The targe type for serialization
     */
    template <typename T>
    concept CanSerialize = requires(FArchive &Archive, T &Data) {
        { Archive << Data } -> std::same_as<FArchive &>;
    };

} // namespace UE::Ranges
