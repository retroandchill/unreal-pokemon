// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Exceptions/InvalidArgumentException.h"
#include "Ranges/Exceptions/VariantException.h"
#include "Ranges/Utilities/Unreachable.h"

namespace UE::Ranges {
    class IVariantRegistration;
    class FTypeException;

    /**
     * Validate that the given struct property is valid.
     * @tparam T The type of pointer for the struct (most likely either void* or uint8*)
     * @param StructProperty The property for the struct
     * @param StructPointer The opaque pointer to the struct
     * @throws FInvalidArgumentException If either the property or the struct are nullptr.
     */
    template <typename T>
    void ValidateStructProperty(const FStructProperty *StructProperty, const T *StructPointer) {
        if (StructProperty == nullptr || StructPointer == nullptr) {
            throw FInvalidArgumentException(
                EBlueprintExceptionType::AccessViolation,
                NSLOCTEXT("ValidateStructProperty", "Bad property", "Failed to resolve the struct parameter."));
        }
    }

    /**
     * Validates that the given property matches the expected struct.
     * @param Property The property containing the struct information.
     * @param Struct The expected struct to validate against.
     * @throws FTypeException If the given property's struct does not match the expected struct type.
     */
    UNREALRANGES_API void ValidateStructsMatch(const FStructProperty &Property, const UScriptStruct *Struct);

    /**
     * Ensures that the provided enum property matches the expected enum type.
     * @param Property The enum property to be validated.
     * @param Enum The expected enum type to validate against.
     * @throws FTypeException If the provided enum property does not match the expected enum type.
     */
    UNREALRANGES_API void ValidateEnumsMatch(const FEnumProperty &Property, const UEnum *Enum);

    /**
     * Get the registration information for a given variant.
     * @param Property The property that has the struct info
     * @return The struct registration
     * @throws FVariantException If the struct type is not a valid variant
     */
    UNREALRANGES_API IVariantRegistration &GetVariantRegistration(const FStructProperty &Property);

} // namespace UE::Ranges

#define P_GET_WILDCARD_PARAM(PropVar, PointerVar)                                                                      \
    Stack.StepCompiledIn<FProperty>(nullptr);                                                                          \
    const auto PropVar = Stack.MostRecentProperty;                                                                     \
    auto PointerVar = Stack.MostRecentPropertyAddress;

#define P_GET_OPAQUE_STRUCT(PropVar, PointerVar)                                                                       \
    Stack.StepCompiledIn<FStructProperty>(nullptr);                                                                    \
    const auto PropVar = CastField<FStructProperty>(Stack.MostRecentProperty);                                         \
    auto PointerVar = Stack.MostRecentPropertyAddress;

#define P_GET_RESULT(Type, Name) auto &Name = *static_cast<Type *>(RESULT_PARAM)

#define CUSTOM_THUNK_STUB(RetType, Method, ...)                                                                        \
    RetType Method(__VA_ARGS__) {                                                                                      \
        check(false);                                                                                                  \
        UE::Ranges::Unreachable();                                                                                     \
    }