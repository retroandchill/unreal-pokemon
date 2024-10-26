// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Exceptions/InvalidArgumentException.h"
#include "Ranges/Exceptions/VariantException.h"
#include "Ranges/Utilities/Unreachable.h"

namespace UE::Ranges {
    class IVariantRegistration;
}

namespace UE::Ranges {
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
     * Get the registration information for a given variant.
     * @param Property The property that has the struct info
     * @return The struct registration
     * @throws FVariantException If the struct type is not a valid variant
     */
    IVariantRegistration &GetVariantRegistration(const FStructProperty &Property);

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
        check(false); \
        UE::Ranges::Unreachable(); \
    }