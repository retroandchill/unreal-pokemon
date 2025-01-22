// "Unreal Pokémon" created by Retro & Chill.


#include "Simple2D/Assets/SimpleFlipbookFactory.h"

USimpleFlipbookFactory::USimpleFlipbookFactory() {
    SupportedClass = USimpleFlipbook::StaticClass();
    bCreateNew = true;
    bEditAfterNew = true;
}

USimpleFlipbook * USimpleFlipbookFactory::FactoryCreateNew(UClass *InClass, UObject *InParent, FName InName,
    EObjectFlags Flags, UObject *Context, FFeedbackContext *Warn) {
    return NewObject<USimpleFlipbook>(InParent, InClass, InName, Flags);
}

bool USimpleFlipbookFactory::ShouldShowInNewMenu() const {
    return true;
}