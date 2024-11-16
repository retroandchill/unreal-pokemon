// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Concepts/Structs.h"

namespace UE::Ranges {

    /**
     * Concept for working with a Polymorphic struct type. Requires it be a valid UnrealEngine struct, have a
     * virtual destructor, and have a GetStruct() method that returns the type of the struct.
     *
     * @tparam T The struct type to test
     */
    template <typename T>
    concept PolymorphicStruct = UEStruct<T> && std::has_virtual_destructor_v<T> && requires(T&& Struct) {
        { Struct.GetStruct() } -> std::same_as<UScriptStruct *>;
    };

    
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

    template <typename T>
        requires PolymorphicStruct<T>
    void operator<<(FArchive& Archive, TSharedRef<T>& Ptr) noexcept {
        UScriptStruct* Struct = Archive.IsSaving() ? Ptr->GetStruct() : nullptr;
        Archive << Struct;
        check(Struct != nullptr && Struct->IsChildOf<T>())
        if (Archive.IsLoading()) {
            Ptr = static_cast<T*>(FMemory::Malloc(Struct->GetStructureSize()));
        }
        Struct->SerializeItem(Ptr.Get(), Archive, nullptr);
    }

}
