// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Concepts/Structs.h"

namespace UE::Ranges {

    /**
     * Concept for working with a Polymorphic struct type. Requires it be a valid UnrealEngine struct and have a
     * virtual destructor.
     *
     * @tparam T The struct type to test
     */
    template <typename T>
    concept PolymorphicStruct = UEStruct<T> && std::has_virtual_destructor_v<T>;

    
    /**
     * Creates a polymorphic struct of the specified Unreal Engine USTRUCT type.
     *
     * This function checks whether the provided UScriptStruct is a child of the
     * specified USTRUCT type T. If the check passes, it allocates memory for the
     * struct, initializes it, and returns a shared reference to the new struct
     * instance. If the check fails, it returns an empty TOptional.
     *
     * @tparam T The Unreal Engine USTRUCT type.
     * @param Struct Pointer to the UScriptStruct representing the type T.
     * @return TOptional containing a TSharedRef to the newly created struct instance,
     *         or an empty TOptional if the provided UScriptStruct is not a child of T.
     */
    template <typename T>
        requires PolymorphicStruct<T>
    TOptional<TSharedRef<T>> CreatePolymorphicStruct(UScriptStruct* Struct) noexcept {
        if (!Struct->IsChildOf<T>()) {
            return TOptional<TSharedRef<T>>();
        }

        auto Memory = FMemory::Malloc(Struct->GetStructureSize());
        Struct->InitializeStruct(Memory);
        return MakeShareable(static_cast<T*>(Memory)).ToSharedRef();
    } 

}
