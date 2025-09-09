// Fill out your copyright notice in the Description page of Project Settings.

#include "Handles/DataHandlePropertyIdentifier.h"
#include "Interop/GameDataTypeManagedCallbacks.h"
#include "TypeGenerator/CSScriptStruct.h"

bool FDataHandlePropertyIdentifier::IsPropertyTypeCustomized(const IPropertyHandle &PropertyHandle) const
{
    const auto Property = CastFieldChecked<FStructProperty>(PropertyHandle.GetProperty());
    const auto ManagedStruct = Cast<UCSScriptStruct>(Property->Struct);
    if (ManagedStruct == nullptr)
    {
        return false;
    }

    if (CastField<FNameProperty>(ManagedStruct->FindPropertyByName("ID")) == nullptr) {
        return false;
    }

    const auto ManagedType = ManagedStruct->GetManagedTypeInfo<>()->GetManagedTypeHandle();
    return FGameDataTypeManagedCallbacks::Get().IsValidGameDataHandle(*ManagedType);
}
