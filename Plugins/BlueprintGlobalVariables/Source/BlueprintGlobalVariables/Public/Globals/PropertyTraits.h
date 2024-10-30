// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

namespace Blueprint::Globals {

    /**
     * Definition for a property type for a primitive property.
     */
    template <typename>
    struct TPropertyType : std::false_type {
    };

    /**
     * Concept that defines whether a given type has a property value or not.
     */
    template <typename T>
    concept HasPropertyType = TPropertyType<T>::value;
    
}

#define DECLARE_PRIMITIVE_PROPERTY(Property) \
    static_assert(std::derived_from<Property, FProperty>); \
    template <> \
    struct Blueprint::Globals::TPropertyType<Property::TCppType> : std::true_type { \
        using FType = Property; \
    }

#define DEFINE_PRIMITIVE_PROPERTY(Property) \
    static const bool __##Property##__Registered = Blueprint::Globals::RegisterPrimitiveProperty<Property::TCppType>()

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