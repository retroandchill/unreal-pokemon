// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include <memory>

/**
 * Define all the information necessary to initialize a gameplay attribute
 * @param ClassName The name of the class that owns the attribute
 * @param PropertyName The name of the property on the attribute
 */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)                                                                   \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)                                                         \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)                                                                       \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)                                                                       \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * Define all the information necessary to initialize a gameplay attribute with a custom initter process
 * @param ClassName The name of the class that owns the attribute
 * @param PropertyName The name of the property on the attribute
 */
#define ATTRIBUTE_ACCESSORS_CUSTOM_INITTER(ClassName, PropertyName)                                                    \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)                                                         \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)                                                                       \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)                                                                       \
    void Init##PropertyName(float NewVal)