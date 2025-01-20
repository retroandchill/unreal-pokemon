// "Unreal Pokémon" created by Retro & Chill.


#include "SimpleFlipbook/Assets/SimpleFlipbook.h"

USimpleFlipbook::USimpleFlipbook() {
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaskedMaterialRef(TEXT("/Paper2D/MaskedUnlitSpriteMaterial"));
    DefaultMaterial = MaskedMaterialRef.Object;
}