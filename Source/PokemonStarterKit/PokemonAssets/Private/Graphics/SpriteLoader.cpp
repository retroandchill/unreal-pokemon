// "Unreal Pokémon" created by Retro & Chill.

#include "Graphics/SpriteLoader.h"
#include "Graphics/GraphicsAssetClasses.h"
#include "Pokemon/Pokemon.h"
#include "RetroLib/Optionals/IfPresent.h"
#include "RetroLib/Utils/Construct.h"
#include "Species/SpeciesData.h"
#include "Trainers/TrainerType.h"

FBattleRender USpriteLoader::GetPokemonBattleSprite(const TScriptInterface<IPokemon> &Pokemon, bool bBack)
{
    return GetSpeciesBattleSprite(Pokemon->GetSpecies().ID, bBack,
                                  {.Gender = Pokemon->GetGender(), .bShiny = Pokemon->IsShiny()});
}

FVoidCoroutine USpriteLoader::AsyncGetPokemonBattleSprite(const TScriptInterface<IPokemon> &Pokemon,
                                                          FBattleRender &OutRender, bool bBack, FLatentActionInfo)
{
    OutRender = co_await AsyncGetPokemonBattleSprite(Pokemon, bBack);
}

UE5Coro::TCoroutine<FBattleRender> USpriteLoader::AsyncGetPokemonBattleSprite(const TScriptInterface<IPokemon> &Pokemon,
                                                                              bool bBack, FForceLatentCoroutine)
{
    co_return co_await GetLazyPokemonBattleSprite(Pokemon, bBack).LoadAsync() |
        Retro::Optionals::OrElseGet(Retro::Construct<FBattleRender>);
}

FBattleRender USpriteLoader::GetSpeciesBattleSprite(FName Species, bool bBack,
                                                    const FPokemonAssetParams &AdditionalParams)
{
    auto SpriteResolutionList = AdditionalParams.CreateResolutionList(Species, bBack ? TEXT("Back") : TEXT("Front"));
    return FBattleRender(); //Pokemon::Assets::Graphics::PokemonBattleSprites.ResolveAsset(SpriteResolutionList).Get(FBattleRender());
}

FSoftBattleRender USpriteLoader::GetLazyPokemonBattleSprite(const TScriptInterface<IPokemon> &Pokemon, bool bBack)
{
    return GetLazySpeciesBattleSprite(Pokemon->GetSpecies().ID, bBack,
                                      {.Gender = Pokemon->GetGender(), .bShiny = Pokemon->IsShiny()});
}

FSoftBattleRender USpriteLoader::GetLazySpeciesBattleSprite(FName Species, bool bBack,
                                                            const FPokemonAssetParams &AdditionalParams)
{
    auto SpriteResolutionList = AdditionalParams.CreateResolutionList(Species, bBack ? TEXT("Back") : TEXT("Front"));
    // clang-format off
    return FSoftBattleRender(); //Pokemon::Assets::Graphics::PokemonBattleSprites.ResolveSoftAsset(SpriteResolutionList).Get(FSoftBattleRender());
    // clang-format on
}

FImageAsset USpriteLoader::GetPokemonIcon(const TScriptInterface<IPokemon> &Pokemon)
{
    return GetSpeciesIcon(Pokemon->GetSpecies().ID, {.Gender = Pokemon->GetGender()});
}

FImageAsset USpriteLoader::GetSpeciesIcon(FName Species, const FPokemonAssetParams &AdditionalParams)
{
    auto SpriteResolutionList = AdditionalParams.CreateResolutionList(Species, TEXT("Icons"));
    return FImageAsset(); //Pokemon::Assets::Graphics::PokemonMenuSprites.ResolveAsset(SpriteResolutionList).Get(FImageAsset());
}

FSoftImageAsset USpriteLoader::GetLazyPokemonIcon(const TScriptInterface<IPokemon> &Pokemon)
{
    return GetLazySpeciesIcon(Pokemon->GetSpecies().ID, {.Gender = Pokemon->GetGender()});
}

FSoftImageAsset USpriteLoader::GetLazySpeciesIcon(FName Species, const FPokemonAssetParams &AdditionalParams)
{
    auto SpriteResolutionList = AdditionalParams.CreateResolutionList(Species, TEXT("Icons"));
    return FSoftImageAsset(); //Pokemon::Assets::Graphics::PokemonMenuSprites.ResolveSoftAsset(SpriteResolutionList).Get(FSoftImageAsset());
}

FBattleRender USpriteLoader::GetTrainerSprite(const TScriptInterface<ITrainer> &Trainer, bool bBack)
{
    TArray<FString, TInlineAllocator<2>> StringParts = {bBack ? TEXT("Back") : TEXT("Front"),
                                                        Trainer->GetTrainerType().ID.ToString()};
    auto JoinedString = FString::Join(StringParts, TEXT("/"));
    return FBattleRender(); //Pokemon::Assets::Graphics::TrainerSprites.LoadAsset(JoinedString).Get(FBattleRender());
}