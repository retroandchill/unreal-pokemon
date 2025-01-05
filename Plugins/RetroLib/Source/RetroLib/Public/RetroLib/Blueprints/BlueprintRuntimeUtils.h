// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifdef __UNREAL__
#include "RetroLib/Exceptions/InvalidArgumentException.h"

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro {
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
            throw FInvalidArgumentException("Failed to resolve the struct parameter.");
        }
    }

    /**
     * Validates that the given property matches the expected struct.
     * @param Property The property containing the struct information.
     * @param Struct The expected struct to validate against.
     * @throws FTypeException If the given property's struct does not match the expected struct type.
     */
    RETROLIB_API void ValidateStructsMatch(const FStructProperty &Property, const UScriptStruct *Struct);

    /**
     * Ensures that the provided enum property matches the expected enum type.
     * @param Property The enum property to be validated.
     * @param Enum The expected enum type to validate against.
     * @throws FTypeException If the provided enum property does not match the expected enum type.
     */
    RETROLIB_API void ValidateEnumsMatch(const FEnumProperty &Property, const UEnum *Enum);

    /**
     * Get the registration information for a given variant.
     * @param Property The property that has the struct info
     * @return The struct registration
     * @throws FVariantException If the struct type is not a valid variant
     */
    RETROLIB_API IVariantRegistration &GetVariantRegistration(const FStructProperty &Property);

} // namespace Retro
#endif