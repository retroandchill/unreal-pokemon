// "Unreal Pokémon" created by Retro & Chill.


#include "Graphics/Async/AsyncGetPokemonBattleSprite.h"
#include "Graphics/AssetClasses.h"
#include "Pokemon/Pokemon.h"
#include "Species/SpeciesData.h"

UAsyncGetPokemonBattleSprite * UAsyncGetPokemonBattleSprite::GetAsyncGetPokemonBattleSprite(
    const TScriptInterface<IPokemon> &Pokemon, bool bBack) {
    return GetAsyncGetSpeciesBattleSprite(Pokemon->GetSpecies().ID, bBack,
                                  {.Gender = Pokemon->GetGender(), .bShiny = Pokemon->IsShiny()});
}

UAsyncGetPokemonBattleSprite * UAsyncGetPokemonBattleSprite::GetAsyncGetSpeciesBattleSprite(FName Species, bool bBack,
const FPokemonAssetParams &AdditionalParams) {
    auto Node = NewObject<UAsyncGetPokemonBattleSprite>();
    Node->Species = Species;
    Node->bBack = bBack;
    Node->AdditionalParams = AdditionalParams;
    return Node;
}

void UAsyncGetPokemonBattleSprite::Activate() {
    auto SpriteResolutionList =
        USpriteLoader::CreatePokemonSpriteResolutionList(Species, AdditionalParams, bBack ? TEXT("Back") : TEXT("Front"));
    auto SoftAsset = Pokemon::Assets::Graphics::PokemonSprites.ResolveSoftAsset(SpriteResolutionList);
    if (!SoftAsset.IsSet()) {
        LoadFailed.Broadcast();
        SetReadyToDestroy();
        return;
    }

    LoadHandle = SoftAsset->LoadAsync();
    LoadHandle->OnLoadComplete(this, &UAsyncGetPokemonBattleSprite::OnLoadComplete);
}

void UAsyncGetPokemonBattleSprite::OnLoadComplete(const TOptional<UPaperFlipbook &> &Flipbook) {
    if (Flipbook.IsSet()) {
        LoadCompleted.Broadcast(Flipbook.GetPtrOrNull());
    } else {
        LoadFailed.Broadcast();
    }
    SetReadyToDestroy();
}