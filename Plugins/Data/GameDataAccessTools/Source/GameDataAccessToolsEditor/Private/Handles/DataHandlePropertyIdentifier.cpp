// Fill out your copyright notice in the Description page of Project Settings.

#include "Handles/DataHandlePropertyIdentifier.h"
#include "TypeGenerator/CSScriptStruct.h"

bool FDataHandlePropertyIdentifier::IsPropertyTypeCustomized(const IPropertyHandle &PropertyHandle) const
{
    const auto Property = CastFieldChecked<FStructProperty>(PropertyHandle.GetProperty());
    const auto ManagedStruct = Cast<UCSScriptStruct>(Property->Struct);
    if (ManagedStruct == nullptr)
    {
        return false;
    }

    return ManagedStruct->HasMetaData("DataHandle") && ManagedStruct->FindPropertyByName("Id") != nullptr;
}
