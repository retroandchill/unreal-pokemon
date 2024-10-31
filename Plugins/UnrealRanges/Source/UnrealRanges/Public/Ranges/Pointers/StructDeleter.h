// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

namespace UE::Ranges {
    /**
     * @struct FStructDeleter
     * @brief A deleter class used to manage the lifecycle of structures allocated with `UScriptStruct`.
     *
     * This structure provides a custom deleter for unique pointers (`TUniquePtr`) that ensures proper
     * destruction and deallocation of script structures (`UScriptStruct`). It leverages `DestroyStruct`
     * and `FMemory::Free` to clean up the allocated memory correctly.
     *
     * @note The `Struct` member points to the `UScriptStruct` type that describes the structure to be deleted.
     */
    struct FStructDeleter {
        /**
         * @brief Pointer to a constant `UScriptStruct` used to describe the structure type.
         *
         * This object pointer holds a constant reference to a `UScriptStruct`, which provides
         * metadata about the structure type. It is often used to identify the type for
         * operations such as destruction and memory management.
         */
        TObjectPtr<const UScriptStruct> Struct;

        /**
         * @brief Default constructor for the `FStructDeleter` class.
         *
         * This default constructor initializes an instance of the `FStructDeleter` class
         * with no specific `UScriptStruct` type assigned. It is useful for creating
         * `FStructDeleter` objects when the specific structure type is to be defined later.
         */
        FStructDeleter() = default;

        /**
         * @brief Constructs an `FStructDeleter` with a specified `UScriptStruct`.
         *
         * This constructor initializes an instance of the `FStructDeleter` class with a given `UScriptStruct` pointer,
         * which is used to describe the structure to be deleted. The construction is marked as `noexcept`,
         * ensuring that no exceptions are thrown during initialization.
         *
         * @param Struct A pointer to a constant `UScriptStruct` that describes the structure type for deletion.
         * This parameter provides the necessary metadata for correct management and destruction of
         * structures allocated with `UScriptStruct`.
         *
         * @return None.
         */
        explicit FStructDeleter(const UScriptStruct *Struct) noexcept : Struct(Struct) {
        }

        /**
         * @brief Functor for deleting structures managed by `UScriptStruct`.
         *
         * This operator overload is invoked to safely destroy and deallocate a given
         * structure instance. It leverages the `DestroyStruct` method of `UScriptStruct`
         * and `FMemory::Free` to ensure proper cleanup.
         *
         * @tparam T The type of the structure to be deleted.
         * @param Data Pointer to the structure instance to be deleted. If this pointer
         * is non-null and the `Struct` member is valid, the structure will be destroyed
         * and its memory freed.
         */
        template <typename T>
        FORCEINLINE void operator()(T *Data) const noexcept {
            if (Data != nullptr && IsValid(Struct)) {
                Struct->DestroyStruct(Data);
                FMemory::Free(Data);
            }
        }
    };
} // namespace UE::Ranges