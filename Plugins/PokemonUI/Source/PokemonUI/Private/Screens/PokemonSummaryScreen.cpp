// "Unreal Pokémon" created by Retro & Chill.


#include "Screens/PokemonSummaryScreen.h"
#include "Components/Image.h"
#include "Components/Summary/SummaryNameInfo.h"
#include "Graphics/GraphicsLoadingSubsystem.h"
#include "Windows/SummaryPages.h"

void UPokemonSummaryScreen::NativeConstruct() {
    Super::NativeConstruct();

    
}

void UPokemonSummaryScreen::SetPokemon(const TScriptInterface<IPokemon> &Pokemon) {
    NameInfo->Refresh(Pokemon);
    SummaryPages->Refresh(Pokemon);

    auto GraphicsLoadingSubsystem = GetGameInstance()->GetSubsystem<UGraphicsLoadingSubsystem>();
    auto [SpriteMaterial, Size] = GraphicsLoadingSubsystem->GetPokemonBattleSprite(*Pokemon, this);
    PokemonSprite->SetBrushFromMaterial(SpriteMaterial);
    PokemonSprite->SetDesiredSizeOverride(Size);
}