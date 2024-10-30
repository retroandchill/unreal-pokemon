// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Concepts/Enums.h"
#include "Ranges/Concepts/Structs.h"
#include "Ranges/Views/AnyView.h"

namespace Blueprint::Globals {

    /**
     * Definition for a property type for a primitive property.
     */
    template <typename>
    struct TPropertyMeta : std::false_type {
    };

    template <typename T>
        requires UE::Ranges::UEStruct<T>
    struct TPropertyMeta<T> : std::true_type {
        using FType = FStructProperty;
    };

    template <typename T>
        requires UE::Ranges::UEEnum<T>
    struct TPropertyMeta<T> : std::true_type {
        using FType = FEnumProperty;
    };

    /**
     * Concept that defines whether a given type has a property value or not.
     */
    template <typename T>
    concept HasPropertyType = TPropertyMeta<T>::value;

    /**
     * Typedef for getting the wrapping property type for a primitive.
     * @tparam T The type of primitive to get the property for
     */
    template <typename T>
        requires HasPropertyType<T>
    using TPropertyType = typename TPropertyMeta<T>::FType;

    /**
     * Concept for verifying if a property has a C++ type defined on its class definition.
     * @tparam T The type to check
     */
    template <typename T>
    concept HasCppType = std::derived_from<T, FProperty> && requires { typename T::TCppType; };

    /**
     * Concept for verifying if a property has a static definition for its property getter.
     * @tparam T The type to check
     */
    template <typename T>
    concept HasStaticPropertyGetter = HasCppType<T> && std::derived_from<T, TPropertyTypeFundamentals<typename T::TCppType>>;
}

#define DECLARE_PRIMITIVE_PROPERTY(Property) \
    static_assert(std::derived_from<Property, FProperty>); \
    template <> \
    struct Blueprint::Globals::TPropertyMeta<Property::TCppType> : std::true_type { \
        using FType = Property; \
    }; \
    static_assert(Blueprint::Globals::HasCppType<Property>)

DECLARE_PRIMITIVE_PROPERTY(FBoolProperty);
DECLARE_PRIMITIVE_PROPERTY(FInt8Property);
DECLARE_PRIMITIVE_PROPERTY(FByteProperty);
DECLARE_PRIMITIVE_PROPERTY(FInt16Property);
DECLARE_PRIMITIVE_PROPERTY(FUInt16Property);
DECLARE_PRIMITIVE_PROPERTY(FIntProperty);
DECLARE_PRIMITIVE_PROPERTY(FUInt32Property);
DECLARE_PRIMITIVE_PROPERTY(FInt64Property);
DECLARE_PRIMITIVE_PROPERTY(FUInt64Property);
DECLARE_PRIMITIVE_PROPERTY(FFloatProperty);
DECLARE_PRIMITIVE_PROPERTY(FDoubleProperty);
DECLARE_PRIMITIVE_PROPERTY(FNameProperty);
DECLARE_PRIMITIVE_PROPERTY(FStrProperty);
DECLARE_PRIMITIVE_PROPERTY(FTextProperty);