// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "SimpleFlipbook/Assets/SimpleFlipbook.h"

#include "SimpleFlipbookFactory.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEFLIPBOOKEDITOR_API USimpleFlipbookFactory : public UFactory {
    GENERATED_BODY()

public:
    USimpleFlipbookFactory();

    USimpleFlipbook *FactoryCreateNew(UClass *InClass, UObject *InParent, FName InName, EObjectFlags Flags,
                                  UObject *Context, FFeedbackContext *Warn) override;
    bool ShouldShowInNewMenu() const override;

};
