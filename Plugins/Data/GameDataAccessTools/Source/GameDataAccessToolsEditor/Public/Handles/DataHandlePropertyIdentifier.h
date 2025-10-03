// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * @class FDataHandlePropertyIdentifier
 * @brief A property identifier used to determine if a property is of a customized type.
 *
 * This class inherits from IPropertyTypeIdentifier and provides an implementation
 * for identifying specific property types, specifically customized property types
 * related to data handles in the editor module.
 *
 * It is primarily registered within the editor module to work with
 * custom property layouts and facilitates specific behavior for properties
 * of type FStructProperty.
 */
class GAMEDATAACCESSTOOLSEDITOR_API FDataHandlePropertyIdentifier final : public IPropertyTypeIdentifier
{
  public:
    bool IsPropertyTypeCustomized(const IPropertyHandle &PropertyHandle) const override;
};
