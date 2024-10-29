// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

namespace Blueprint::Globals {

    /**
     * Definition for a property type for a primitive property.
     */
    template <typename>
    struct TPropertyType : std::false_type {};

    /**
     * Concept that defines whether a given type has a property value or not.
     */
    template <typename T>
    concept HasPropertyType = TPropertyType<T>::value;
}

#define PRIMITIVE_PROPERTY(Property) \
    static_assert(std::derived_from<Property, FProperty>); \
    template <> \
    struct Blueprint::Globals::TPropertyType<Property::TCppType> : std::true_type { \
        using FType = Property; \
    }

PRIMITIVE_PROPERTY(FBoolProperty);
PRIMITIVE_PROPERTY(FInt8Property);
PRIMITIVE_PROPERTY(FByteProperty);
PRIMITIVE_PROPERTY(FInt16Property);
PRIMITIVE_PROPERTY(FUInt16Property);
PRIMITIVE_PROPERTY(FIntProperty);
PRIMITIVE_PROPERTY(FUInt32Property);
PRIMITIVE_PROPERTY(FInt64Property);
PRIMITIVE_PROPERTY(FUInt64Property);
PRIMITIVE_PROPERTY(FFloatProperty);
PRIMITIVE_PROPERTY(FDoubleProperty);
PRIMITIVE_PROPERTY(FNameProperty);
PRIMITIVE_PROPERTY(FStrProperty);
PRIMITIVE_PROPERTY(FTextProperty);
