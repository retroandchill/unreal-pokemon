// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include <memory>

namespace UE::Ranges {
    /**
     * @class FPropertyDeleter
     * @brief A utility class deleting the data allocated by a property object.
     */
    struct FPropertyDeleter {

        /**
         * @brief Default constructor for FPropertyDeleter.
         *
         * This constructor initializes an instance of FPropertyDeleter with default values.
         *
         * @return An instance of FPropertyDeleter with default initialization.
         */
        FPropertyDeleter() = default;

        /**
         * @brief Constructor that initializes FPropertyDeleter with a specific property.
         *
         * This constructor accepts a pointer to an FProperty object and initializes the FPropertyDeleter instance with
         * it.
         *
         * @param Property A pointer to the FProperty object to be managed.
         * @return An instance of FPropertyDeleter initialized with the given property.
         */
        explicit FPropertyDeleter(const FProperty *Property) : Property(Property) {
        }

        /**
         * @brief Retrieves the property associated with the deleter.
         *
         * This function returns a pointer to the property managed by the FPropertyDeleter instance.
         *
         * @return A pointer to the managed FProperty.
         */
        FORCEINLINE const FProperty *GetProperty() const {
            return Property;
        }

        /**
         * @brief Functor for deleting data managed by `FProperty`.
         *
         * This operator overload is invoked to safely destroy and deallocate a given
         * structure instance. It leverages the `DestroyAndFreeValue` method of `FProperty`
         * to ensure proper cleanup.
         *
         * @tparam T The type of the data to be deleted.
         * @param Data Pointer to the data instance to be deleted. If this pointer
         * is non-null and the `Property` member is valid, the structure will be destroyed
         * and its memory freed.
         */
        template <typename T>
        void operator()(T *Data) {
            if (Data != nullptr) {
                check(Property != nullptr)
                Property->DestroyAndFreeValue(Data);
            }
        }

      private:
        TObjectPtr<const FProperty> Property;
    };

    /**
     * Type alias for a unique ptr to a property managed pointer.
     */
    using FUniquePropertyPtr = std::unique_ptr<void, FPropertyDeleter>;

    /**
     * @brief Creates a unique pointer to a property-managed object.
     *
     * This function allocates and initializes the property value,
     * and wraps it in a unique pointer managed by FPropertyDeleter.
     *
     * @param Property A pointer to the FProperty object which specifies the type and
     * initialization of the data being allocated.
     * @return A FUniquePropertyPtr which manages the allocated and initialized property value.
     */
    FORCEINLINE FUniquePropertyPtr MakeUniqueProperty(const FProperty *Property) noexcept {
        return FUniquePropertyPtr(Property->AllocateAndInitializeValue(), FPropertyDeleter(Property));
    }

    /**
     * @brief Creates a unique pointer to a property-managed object with data copying.
     *
     * This function allocates memory for the given data using the specified property,
     * copies the data into the allocated memory, and wraps it in a unique pointer
     * managed by FPropertyDeleter.
     *
     * @param Data A pointer to the data of type T that needs to be copied.
     * @param Property A pointer to the FProperty object which specifies the type and
     * copying of the data being allocated.
     * @return A FUniquePropertyPtr which manages the allocated and initialized property value.
     */
    template <typename T>
    FORCEINLINE FUniquePropertyPtr MakeUniqueProperty(const T *Data, const FProperty *Property) noexcept {
        auto CopyData = [](const T *D, const FProperty *P) {
            auto Memory = FMemory::Malloc(static_cast<size_t>(P->GetSize()));
            P->CopyCompleteValue(Memory, D);
            return Memory;
        };
        return FUniquePropertyPtr(CopyData(Data, Property), FPropertyDeleter(Property));
    }

    FORCEINLINE FUniquePropertyPtr CloneUniqueProperty(const FUniquePropertyPtr &Other) noexcept {
        if (Other.get() == nullptr || Other.get_deleter().GetProperty() == nullptr) {
            return FUniquePropertyPtr();
        }

        return FUniquePropertyPtr(MakeUniqueProperty(Other.get(), Other.get_deleter().GetProperty()));
    }
} // namespace UE::Ranges