// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

/**
 * @class FGameDataEntryDetailsCustomization
 * @brief Customizes the appearance and behavior of the details view for the "GameDataEntry" class.
 *
 * This class is a customization class intended to modify the details panel layout for the
 * "GameDataEntry" object in Unreal Engine's editor. It extends the IDetailCustomization
 * interface, allowing for tailored configuration of the class's properties in the editor.
 */
class GAMEDATAACCESSTOOLSEDITOR_API FGameDataEntryDetailsCustomization : public IDetailCustomization
{
  public:
    static TSharedRef<IDetailCustomization> MakeInstance();

    void CustomizeDetails(IDetailLayoutBuilder &DetailBuilder) override;
};
