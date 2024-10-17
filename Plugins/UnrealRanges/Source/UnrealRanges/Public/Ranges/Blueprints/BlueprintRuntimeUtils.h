// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Exceptions/InvalidArgumentException.h"
#include "Ranges/Exceptions/VariantException.h"

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
            throw FInvalidArgumentException(EBlueprintExceptionType::AccessViolation,
                                                  NSLOCTEXT("ValidateStructProperty", "Bad property",
                                                            "Failed to resolve the struct parameter."));
        }
    }

    /**
     * Get the registration information for a given variant.
     * @param Property The property that has the struct info
     * @return The struct registration
     * @throws FVariantException If the struct type is not a valid variant
     */
    IVariantRegistration& GetVariantRegistration(const FStructProperty &Property);

    template <typename T>
        requires !std::is_reference_v<T>
    constexpr auto DefaultReturnValue() {
        if constexpr (std::is_void_v<T>) {
            // No-op
        } else if constexpr (std::is_pointer_v<T>) {
            return nullptr;
        } else if constexpr (std::is_integral_v<T> || std::is_floating_point_v<T> || std::is_enum_v<T>) {
            return static_cast<T>(0);
        } else if constexpr (std::same_as<bool, T>) {
            return false;
        } else if constexpr (std::is_default_constructible_v<T>) {
            return T();
        }
    }
    
}

#define P_GET_OPAQUE_STRUCT(PropVar, PointerVar) \
    Stack.StepCompiledIn<FStructProperty>(nullptr); \
    const auto PropVar = CastField<FStructProperty>(Stack.MostRecentProperty); \
    auto PointerVar = Stack.MostRecentPropertyAddress;

#define P_GET_RESULT(Type, Name) auto &Name = *static_cast<Type*>(RESULT_PARAM);

#define CUSTOM_THUNK_STUB(RetType, Method, ...) \
    RetType Method(__VA_ARGS__) { \
        check(false) \
        return UE::Ranges::DefaultReturnValue<RetType>(); \
    }