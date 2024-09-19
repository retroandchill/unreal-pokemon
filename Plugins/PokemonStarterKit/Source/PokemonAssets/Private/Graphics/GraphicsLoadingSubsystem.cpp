// "Unreal Pokémon" created by Retro & Chill.

#include "Graphics/GraphicsLoadingSubsystem.h"
#include "DynamicAssetLoadingSettings.h"
#include "Pokemon/Pokemon.h"
#include "PokemonDataSettings.h"
#include "range/v3/view/transform.hpp"
#include "Ranges/Algorithm/ToArray.h"
#include "Ranges/Optional/GetPtrOrNull.h"
#include "Ranges/Optional/Map.h"
#include "Ranges/Optional/Or.h"
#include "Ranges/Views/ContainerView.h"
#include "Ranges/Views/Extract.h"
#include "Ranges/Views/Map.h"
#include "Species/SpeciesData.h"
#include "Trainers/Trainer.h"
#include "Trainers/TrainerType.h"
#include "PaperFlipbook.h"
#include "Assets/AssetLoader.h"
#include "Assets/AssetUtilities.h"
#include "Graphics/AssetClasses.h"

UPaperFlipbook* UGraphicsLoadingSubsystem::GetPokemonBattleSprite(const TScriptInterface<IPokemon>& Pokemon,
                                                                  bool bBack) const {
    return USpriteLoader::GetPokemonBattleSprite(Pokemon, bBack);
}

UPaperFlipbook* UGraphicsLoadingSubsystem::GetSpeciesBattleSprite(FName Species, bool bBack,
                                                                  const FPokemonAssetParams& AdditionalParams) const {
    return USpriteLoader::GetSpeciesBattleSprite(Species, bBack);
}

UPaperFlipbook* UGraphicsLoadingSubsystem::GetPokemonIcon(const TScriptInterface<IPokemon>& Pokemon) {
    return USpriteLoader::GetPokemonIcon(Pokemon);
}

UPaperFlipbook* UGraphicsLoadingSubsystem::GetSpeciesIcon(FName Species,
                                                          const FPokemonAssetParams& AdditionalParams) {
    return USpriteLoader::GetSpeciesIcon(Species, AdditionalParams);
}

UPaperFlipbook *UGraphicsLoadingSubsystem::GetTrainerSprite(const TScriptInterface<ITrainer>& Trainer) const {
    return USpriteLoader::GetTrainerSprite(Trainer);
}

UPaperFlipbook *UGraphicsLoadingSubsystem::GetTrainerTypeSprite(FName TrainerType) const {
    return Pokemon::Assets::Graphics::TrainerSprites.LoadAsset(TrainerType).GetPtrOrNull();
}

UObject *UGraphicsLoadingSubsystem::GetTypeIconGraphic(FName Type) const {
    auto &PathSettings = *GetDefault<UDynamicAssetLoadingSettings>();
    auto &[AssetPath] = PathSettings.TypeIconsPackageName;
    auto FullName = UAssetUtilities::GetFullAssetName(Type.ToString(), PathSettings.TypeIconPrefix);
    return UAssetLoader::LookupAssetByName(AssetPath, FullName).GetPtrOrNull();
}

TArray<UObject *> UGraphicsLoadingSubsystem::GetTypeIconGraphics(const TArray<FName> &Types) const {
    auto &PathSettings = *GetDefault<UDynamicAssetLoadingSettings>();
    auto &[AssetPath] = PathSettings.TypeIconsPackageName;
    // clang-format off
    return Types |
           UE::Ranges::Map([](FName AssetName) { return AssetName.ToString(); }) |
           UE::Ranges::Map(&UAssetUtilities::GetFullAssetName, PathSettings.TypeIconPrefix) |
           UE::Ranges::Map([&AssetPath](FStringView Name) {
               return UAssetLoader::LookupAssetByName(AssetPath, Name);
           }) |
           UE::Ranges::Extract |
           UE::Ranges::ToArray;
    // clang-format on
}

UObject *UGraphicsLoadingSubsystem::GetStatusIconGraphic(FName Status) const {
    auto &PathSettings = *GetDefault<UDynamicAssetLoadingSettings>();
    auto &[AssetPath] = PathSettings.StatusIconsPackageName;
    auto FullName = UAssetUtilities::GetFullAssetName(Status.ToString(), PathSettings.StatusIconPrefix);
    return UAssetLoader::LookupAssetByName(AssetPath, FullName).GetPtrOrNull();
}

UObject *UGraphicsLoadingSubsystem::GetTypePanelGraphic(FName Type) const {
    auto &PathSettings = *GetDefault<UDynamicAssetLoadingSettings>();
    auto &[AssetPath] = PathSettings.TypePanelsPackageName;
    auto FullName = UAssetUtilities::GetFullAssetName(Type.ToString(), PathSettings.TypePanelPrefix);
    return UAssetLoader::LookupAssetByName(AssetPath, FullName).GetPtrOrNull();
}

UObject *UGraphicsLoadingSubsystem::GetPokeBallIcon(FName PokeBall) const {
    auto &PathSettings = *GetDefault<UDynamicAssetLoadingSettings>();
    auto &[AssetPath] = PathSettings.SummaryBallPackageName;
    auto FullName = UAssetUtilities::GetFullAssetName(PokeBall.ToString(), PathSettings.SummaryBallPrefix);
    // clang-format off
    return UAssetLoader::LookupAssetByName(AssetPath, FullName) |
           UE::Optionals::Or([&AssetPath] {
               return UAssetLoader::LookupAssetByName(AssetPath,
                                                 GetDefault<UPokemonDataSettings>()->DefaultPokeBall.ToString());
           }) |
           UE::Optionals::GetPtrOrNull;
    // clang-format on
}

UObject *UGraphicsLoadingSubsystem::GetItemIcon(FName ItemID) const {
    constexpr auto DefaultItem = TEXT("000");
    auto &[AssetPath] = GetDefault<UDynamicAssetLoadingSettings>()->ItemIconPackageName;
    // clang-format off
    return UAssetLoader::LookupAssetByName(AssetPath, ItemID.ToString()) |
           UE::Optionals::Or([&AssetPath] {
               return UAssetLoader::LookupAssetByName(AssetPath, DefaultItem);
           }) |
           UE::Optionals::GetPtrOrNull;
    // clang-format on
}