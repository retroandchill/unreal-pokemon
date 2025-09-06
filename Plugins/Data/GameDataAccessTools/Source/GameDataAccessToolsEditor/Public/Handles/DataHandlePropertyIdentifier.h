// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
class GAMEDATAACCESSTOOLSEDITOR_API FDataHandlePropertyIdentifier final : public IPropertyTypeIdentifier
{
  public:
    bool IsPropertyTypeCustomized(const IPropertyHandle &PropertyHandle) const override;
};
