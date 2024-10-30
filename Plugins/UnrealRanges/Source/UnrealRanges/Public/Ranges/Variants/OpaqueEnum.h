// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Concepts/Enums.h"

namespace UE::Ranges {
    /**
     * FOpaqueEnum is a utility class designed to store and manage enumeration values in a generic way
     * with the capability to handle different UE enumeration types through unified access methods.
     */
    class UNREALRANGES_API FOpaqueEnum {
      public:
        FOpaqueEnum() = default;

        /**
         * Constructs an FOpaqueEnum object that stores an enumeration value and its corresponding UE enumeration type.
         *
         * @param Enum A pointer to the UEnum that represents the type of the enumeration.
         * @param Value The numeric value of the enumeration to be stored.
         * @return A new instance of the FOpaqueEnum class initialized with the provided enumeration type and value.
         */
        explicit FOpaqueEnum(UEnum *Enum, int64 Value = 0);

        /**
         * Constructs an FOpaqueEnum object that stores an enumeration value and its corresponding UE enumeration type.
         *
         * @param Value The enumeration value to be stored.
         * @return A new instance of the FOpaqueEnum class initialized with the provided enumeration value.
         */
        template <typename T>
            requires UEEnum<T>
        explicit FOpaqueEnum(T Value) : NumericValue(static_cast<int64>(Value)), Enum(StaticEnum<T>()) {
        }

        /**
         * Retrieves the stored enumeration's numeric value.
         *
         * @return The numeric value of the stored enumeration.
         */
        FORCEINLINE int64 GetNumericValue() const {
            return NumericValue;
        }

        /**
         * Retrieves the stored enumeration type.
         *
         * @return A pointer to the UEnum that represents the type of the stored enumeration.
         */
        FORCEINLINE const UEnum *GetEnum() const {
            return Enum;
        }

        /**
         * Checks if the stored enumeration is valid by verifying that the Enum pointer is not null.
         *
         * @return True if the Enum pointer is not null, indicating a valid stored enumeration; false otherwise.
         */
        FORCEINLINE bool IsValid() const {
            return Enum != nullptr;
        }

        /**
         * Retrieves the stored enumeration value as its respective enumeration type.
         *
         * This method performs a type check to ensure that the stored enumeration type matches
         * the requested type and then casts the stored numeric value to the specified enumeration type.
         *
         * @tparam T The enumeration type that the stored value should be cast to.
         * @return The stored enumeration's value cast to the specified type T.
         */
        template <typename T>
            requires UEEnum<T>
        FORCEINLINE T GetValue() const {
            check(Enum == StaticEnum<T>())
            return static_cast<T>(NumericValue);
        }

        /**
         * Attempts to retrieve the stored enumeration value as the specified type T.
         *
         * This method checks if the stored enumeration type matches the requested type T.
         * If they match, it casts the stored numeric value to the specified enumeration type
         * and returns it wrapped in TOptional. If they do not match, it returns an empty TOptional.
         *
         * @tparam T The enumeration type that the stored value should be cast to.
         * @return TOptional containing the stored enumeration's value cast to the specified type T, or an empty
         * TOptional if the types do not match.
         */
        template <typename T>
            requires UEEnum<T>
        TOptional<T> TryGet() const {
            if (Enum != StaticEnum<T>()) {
                return TOptional<T>();
            }

            return TOptional<T>(static_cast<T>(NumericValue));
        }

      private:
        int64 NumericValue = 0;
        TObjectPtr<const UEnum> Enum;
    };
} // namespace UE::Ranges