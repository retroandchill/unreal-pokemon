// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "Factories/Factory.h"
#include "Simple2D/Assets/SimpleFlipbook.h"

#include "SimpleFlipbookFactory.generated.h"

/**
 *
 */
UCLASS()
class SIMPLE2DEDITOR_API USimpleFlipbookFactory : public UFactory {
    GENERATED_BODY()

  public:
    USimpleFlipbookFactory();

    USimpleFlipbook *FactoryCreateNew(UClass *InClass, UObject *InParent, FName InName, EObjectFlags Flags,
                                      UObject *Context, FFeedbackContext *Warn) override;
    bool ShouldShowInNewMenu() const override;
};
