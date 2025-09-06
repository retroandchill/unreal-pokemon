// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Data/Windowskin.h"
#include "Factories/Factory.h"

#include "WindowskinFactory.generated.h"

/**
 * Editor factory for Windowskin assets
 */
UCLASS(HideCategories = (Object))
class RPGMENUSEDITOR_API UWindowskinFactory : public UFactory
{
    GENERATED_BODY()

  public:
    UWindowskinFactory();

    UWindowskin *FactoryCreateNew(UClass *InClass, UObject *InParent, FName InName, EObjectFlags Flags,
                                  UObject *Context, FFeedbackContext *Warn) override;
    bool ShouldShowInNewMenu() const override;
};
