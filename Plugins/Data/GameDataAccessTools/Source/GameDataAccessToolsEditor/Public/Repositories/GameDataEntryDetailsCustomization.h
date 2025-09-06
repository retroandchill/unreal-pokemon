// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

/**
 *
 */
class GAMEDATAACCESSTOOLSEDITOR_API FGameDataEntryDetailsCustomization : public IDetailCustomization
{
  public:
    static TSharedRef<IDetailCustomization> MakeInstance();

    void CustomizeDetails(IDetailLayoutBuilder &DetailBuilder) override;
};
