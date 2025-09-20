// Fill out your copyright notice in the Description page of Project Settings.

#include "Handles/DataHandlePropertyIdentifier.h"
#include "Handles/DataHandleUtils.h"

bool FDataHandlePropertyIdentifier::IsPropertyTypeCustomized(const IPropertyHandle &PropertyHandle) const
{
    const auto Property = CastFieldChecked<FStructProperty>(PropertyHandle.GetProperty());
    return GameData::IsDataHandleStruct(Property->Struct);
}
