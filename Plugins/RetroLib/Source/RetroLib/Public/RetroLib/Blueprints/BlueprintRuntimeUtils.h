// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifdef __UNREAL__
#include "RetroLib/Exceptions/InvalidArgumentException.h"

#if !RETROLIB_WITH_MODULES
#include <bit>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro {
    class IVariantRegistration;
    class IVariantConversion;
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

    RETROLIB_API IVariantConversion &GetVariantConversion(const FStructProperty &From, const FStructProperty &To);

    template <typename T>
    void InvokeFunctionIsolated(UFunction *GetOptionsFunction, T&& Params) {
        auto GetOptionsCDO = GetOptionsFunction->GetOuterUClass()->GetDefaultObject();

        auto VirtualStackAllocator = FBlueprintContext::GetThreadSingleton()->GetVirtualStackAllocator();
        UE_VSTACK_MAKE_FRAME(ProcessEventBookmark, VirtualStackAllocator);
        auto Frame = std::bit_cast<uint8 *>(UE_VSTACK_ALLOC_ALIGNED(VirtualStackAllocator,
                                                                    GetOptionsFunction->PropertiesSize,
                                                                    GetOptionsFunction->GetMinAlignment()));
        // zero the local property memory
        if (auto NonParamsPropertiesSize = GetOptionsFunction->PropertiesSize - GetOptionsFunction->ParmsSize;
            NonParamsPropertiesSize > 0) {
            FMemory::Memzero(Frame + GetOptionsFunction->ParmsSize, NonParamsPropertiesSize);
            }

        // initialize the parameter properties
        if (GetOptionsFunction->ParmsSize > 0) {
            FMemory::Memcpy(Frame, &Params, GetOptionsFunction->ParmsSize);
        }

        // Create a new local execution stack.
        FFrame NewStack(GetOptionsCDO, GetOptionsFunction, Frame, nullptr, GetOptionsFunction->ChildProperties);

        checkSlow(NewStack.Locals || Function->ParmsSize == 0);

        for (auto LocalProp = GetOptionsFunction->FirstPropertyToInit; LocalProp != nullptr; LocalProp = LocalProp->PostConstructLinkNext) {
            LocalProp->InitializeValue_InContainer(NewStack.Locals);
        }

        // Call native function or UObject::ProcessInternal.
        const bool bHasReturnParam = GetOptionsFunction->ReturnValueOffset != MAX_uint16;
        uint8 *ReturnValueAddress = bHasReturnParam ? std::bit_cast<uint8 *>(std::addressof(std::forward<T>(Params))) + GetOptionsFunction->ReturnValueOffset : nullptr;
        GetOptionsFunction->Invoke(GetOptionsCDO, NewStack, ReturnValueAddress);
    }

} // namespace Retro
#endif