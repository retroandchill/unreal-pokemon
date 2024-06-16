// "Unreal Pokémon" created by Retro & Chill.

#include "Graphics/GraphicsLoadingSubsystem.h"
#include "RangeHelpers.h"
#include "Pokemon/Pokemon.h"
#include "Settings/BaseSettings.h"
#include "Species/SpeciesData.h"
#include "Trainers/Trainer.h"
#include "Trainers/TrainerType.h"
#include <cmath>

template <typename T>
    requires std::is_base_of_v<UObject, T>
static T *LookupAssetByName(FStringView BasePackageName, FStringView AssetName) {
    FStringView Prefix;
    if (int32 CharIndex; AssetName.FindLastChar('/', CharIndex)) {
        int32 PrefixLength = CharIndex + 1;
        Prefix = AssetName.SubStr(0, PrefixLength);
        AssetName = AssetName.RightChop(PrefixLength);
    }
    auto SearchKey = FString::Format(TEXT("{0}/{1}{2}.{2}"), {BasePackageName, Prefix, AssetName});
    return Cast<T>(StaticLoadObject(T::StaticClass(), nullptr, *SearchKey));
}

template <typename T>
    requires std::is_base_of_v<UObject, T>
static T *LookupAssetByName(FStringView BasePackageName, FName AssetName) {
    return LookupAssetByName<T>(BasePackageName, AssetName.ToString());
}

static FString GetFullAssetName(FStringView Prefix, FName Identifier) {
    return FString::Format(TEXT("{0}{1}"), {Prefix, Identifier.ToString()});
}

/**
 * Fetch the first matching asset for the provided keys
 * @tparam T The type of asset we're looking for
 * @param BasePackageName The base package to search for when resolving
 * @param Keys The keys to use when resolving
 * @return The found asset, if it exists
 */
template <typename T>
    requires std::is_base_of_v<UObject, T>
static T *ResolveAsset(FStringView BasePackageName, const TArray<FString> &Keys) {
    for (const auto &Key : Keys) {
        auto Lookup = LookupAssetByName<T>(BasePackageName, Key);
        if (Lookup == nullptr) {
            continue;
        }

        return Lookup;
    }

    return nullptr;
}

static TArray<FString> CreatePokemonSpriteResolutionList(FName Species, const FPokemonAssetParams &Params,
                                                         FStringView Subfolder);

void UGraphicsLoadingSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
    Super::Initialize(Collection);

    const auto &Settings = Pokemon::FBaseSettings::Get();
    PokemonSpriteMaterials = Settings.GetPokemonSpriteSettings();
    TrainerSpriteMaterials = Settings.GetTrainerSpriteSettings();
}

FMaterialInstanceWithSize UGraphicsLoadingSubsystem::GetPokemonBattleSprite(const IPokemon &Pokemon, UObject *Outer,
                                                                            bool bBack) const {
    return GetPokemonBattleSprite(Pokemon.GetSpecies().ID, Outer, bBack,
                                  {.Gender = Pokemon.GetGender(), .bShiny = Pokemon.IsShiny()});
}

FMaterialInstanceWithSize
UGraphicsLoadingSubsystem::GetPokemonBattleSprite(FName Species, UObject *Outer, bool bBack,
                                                  const FPokemonAssetParams &AdditionalParams) const {
    auto &[AssetPath] = Pokemon::FBaseSettings::Get().GetDynamicAssetPaths().PokemonSpritePackageName;
    auto SpriteResolutionList =
        CreatePokemonSpriteResolutionList(Species, AdditionalParams, bBack ? TEXT("Back") : TEXT("Front"));
    auto Texture = ResolveAsset<UTexture2D>(AssetPath, SpriteResolutionList);
    if (Texture == nullptr) {
        return {nullptr, FVector2D()};
    }

    static FName SourceTexture = "SourceTexture";
    auto Material =
        UMaterialInstanceDynamic::Create(PokemonSpriteMaterials.BattleSpritesMaterial.LoadSynchronous(), Outer);
    Material->SetTextureParameterValue(SourceTexture, Texture);
    return {Material, FVector2D(Texture->GetSizeY(), Texture->GetSizeY())};
}

FMaterialInstanceWithSize UGraphicsLoadingSubsystem::GetPokemonUISprite(const IPokemon &Pokemon, UObject *Outer,
                                                                        bool bBack) const {
    return GetPokemonUISprite(Pokemon.GetSpecies().ID, Outer, bBack,
                              {.Gender = Pokemon.GetGender(), .bShiny = Pokemon.IsShiny()});
}

FMaterialInstanceWithSize
UGraphicsLoadingSubsystem::GetPokemonUISprite(FName Species, UObject *Outer, bool bBack,
                                              const FPokemonAssetParams &AdditionalParams) const {
    auto &[AssetPath] = Pokemon::FBaseSettings::Get().GetDynamicAssetPaths().PokemonSpritePackageName;
    auto SpriteResolutionList =
        CreatePokemonSpriteResolutionList(Species, AdditionalParams, bBack ? TEXT("Back") : TEXT("Front"));
    auto Texture = ResolveAsset<UTexture2D>(AssetPath, SpriteResolutionList);
    if (Texture == nullptr) {
        return {nullptr, FVector2D()};
    }

    static FName SourceTexture = "SourceTexture";
    auto Material = UMaterialInstanceDynamic::Create(PokemonSpriteMaterials.UISpritesMaterial.LoadSynchronous(), Outer);
    Material->SetTextureParameterValue(SourceTexture, Texture);
    return {Material, FVector2D(Texture->GetSizeY(), Texture->GetSizeY())};
}

UMaterialInstanceDynamic *UGraphicsLoadingSubsystem::GetPokemonIcon(const IPokemon &Pokemon, UObject *Outer) {
    return GetPokemonIcon(Pokemon.GetSpecies().ID, Outer, {.Gender = Pokemon.GetGender()});
}

UMaterialInstanceDynamic *UGraphicsLoadingSubsystem::GetPokemonIcon(FName Species, UObject *Outer,
                                                                    const FPokemonAssetParams &AdditionalParams) {
    auto &[AssetPath] = Pokemon::FBaseSettings::Get().GetDynamicAssetPaths().PokemonSpritePackageName;
    auto SpriteResolutionList = CreatePokemonSpriteResolutionList(Species, AdditionalParams, TEXT("Icons"));
    auto Texture = ResolveAsset<UTexture2D>(AssetPath, SpriteResolutionList);
    if (Texture == nullptr) {
        return nullptr;
    }

    static FName SourceTexture = "SourceTexture";
    auto Material = UMaterialInstanceDynamic::Create(PokemonSpriteMaterials.IconMaterial.LoadSynchronous(), Outer);
    Material->SetTextureParameterValue(SourceTexture, Texture);
    return Material;
}

FMaterialInstanceWithSize UGraphicsLoadingSubsystem::GetTrainerSprite(const ITrainer &Trainer, UObject *Outer) const {
    return GetTrainerSprite(Trainer.GetTrainerType().ID, Outer);
}

FMaterialInstanceWithSize UGraphicsLoadingSubsystem::GetTrainerSprite(FName TrainerType, UObject *Outer) const {
    auto &[AssetPath] = Pokemon::FBaseSettings::Get().GetDynamicAssetPaths().TrainerSpritesPackageName;
    auto Texture = LookupAssetByName<UTexture2D>(AssetPath, TrainerType.ToString());
    if (Texture == nullptr) {
        return {nullptr, FVector2D()};
    }

    static FName SourceTexture = "SourceTexture";
    auto Material =
        UMaterialInstanceDynamic::Create(TrainerSpriteMaterials.FrontSpriteBaseMaterialUI.LoadSynchronous(), Outer);
    Material->SetTextureParameterValue(SourceTexture, Texture);
    return {Material, FVector2D(Texture->GetSizeY(), Texture->GetSizeY())};
}

UObject *UGraphicsLoadingSubsystem::GetTypeIconGraphic(FName Type) const {
    auto &PathSettings = Pokemon::FBaseSettings::Get().GetDynamicAssetPaths();
    auto &[AssetPath] = PathSettings.TypeIconsPackageName;
    auto FullName = GetFullAssetName(PathSettings.TypeIconPrefix, Type);
    return LookupAssetByName<UObject>(AssetPath, FullName);
}

TArray<UObject *> UGraphicsLoadingSubsystem::GetTypeIconGraphics(TConstArrayView<FName> Types) const {
    auto &PathSettings = Pokemon::FBaseSettings::Get().GetDynamicAssetPaths();
    auto &[AssetPath] = PathSettings.TypeIconsPackageName;
    return RangeHelpers::CreateRange(Types) | std::views::transform([&PathSettings](FName Type) {
               return GetFullAssetName(PathSettings.TypeIconPrefix, Type);
           }) |
           std::views::transform(
               [&AssetPath](FStringView Name) { return LookupAssetByName<UObject>(AssetPath, Name); }) |
           RangeHelpers::TToArray<UObject *>();
}

UObject *UGraphicsLoadingSubsystem::GetTypePanelGraphic(FName Type) const {
    auto &PathSettings = Pokemon::FBaseSettings::Get().GetDynamicAssetPaths();
    auto &[AssetPath] = PathSettings.TypePanelsPackageName;
    auto FullName = GetFullAssetName(PathSettings.TypePanelPrefix, Type);
    return LookupAssetByName<UObject>(AssetPath, FullName);
}

UObject *UGraphicsLoadingSubsystem::GetPokeBallIcon(FName PokeBall) const {
    const auto &Settings = Pokemon::FBaseSettings::Get();
    auto &PathSettings = Settings.GetDynamicAssetPaths();
    auto &[AssetPath] = PathSettings.SummaryBallPackageName;
    auto FullName = GetFullAssetName(PathSettings.SummaryBallPrefix, PokeBall);
    auto Asset = LookupAssetByName<UObject>(AssetPath, FullName);
    return Asset != nullptr ? Asset : LookupAssetByName<UObject>(AssetPath, Settings.GetDefaultPokeBall().ToString());
}

UObject *UGraphicsLoadingSubsystem::GetItemIcon(FName ItemID) const {
    static FName DefaultItem = "000";
    auto &[AssetPath] = Pokemon::FBaseSettings::Get().GetDynamicAssetPaths().ItemIconPackageName;
    auto Asset = LookupAssetByName<UObject>(AssetPath, ItemID);
    return Asset != nullptr ? Asset : LookupAssetByName<UObject>(AssetPath, DefaultItem);
}

static TArray<FString> CreatePokemonSpriteResolutionList(FName Species, const FPokemonAssetParams &Params,
                                                         FStringView Subfolder) {
    auto SubfolderString = FString::Format(TEXT("{0}/"), {Subfolder});
    auto ShinyExtension = FString::Format(TEXT("{0}Shiny/"), {Subfolder});
    auto FormExtension = FString::Format(TEXT("_{0}"), {{Params.Form}});
    auto SpeciesExtension = Species.ToString();
    TArray<std::tuple<int32, FStringView, FStringView>> Factors;
    if (Params.bShiny) {
        Factors.Emplace(4, ShinyExtension, SubfolderString);
    }
    if (Params.bShadow) {
        Factors.Emplace(3, TEXT("_shadow"), TEXT(""));
    }
    if (Params.Gender == EPokemonGender::Female) {
        Factors.Emplace(2, TEXT("_female"), TEXT(""));
    }
    if (Params.Form > 0) {
        Factors.Emplace(1, FormExtension, TEXT(""));
    }
    Factors.Emplace(0, SpeciesExtension, TEXT("000"));

    TArray<FString> FormattedStrings;
    auto FactorPower = static_cast<int32>(std::pow(2, Factors.Num()));
    for (int i = 0; i < FactorPower; i++) {
        FStringView TrySpecies;
        FStringView TryForm;
        FStringView TryGender;
        FStringView TryShadow;
        FStringView TrySubfolder = SubfolderString;
        for (int j = 0; j < Factors.Num(); j++) {
            const auto &[FactorIndex, Extension, Fallback] = Factors[j];
            auto TestValue = i / static_cast<int32>(std::pow(2, j)) % 2 == 0 ? Extension : Fallback;
            switch (FactorIndex) {
            case 0:
                TrySpecies = TestValue;
                break;
            case 1:
                TryForm = TestValue;
                break;
            case 2:
                TryGender = TestValue;
                break;
            case 3:
                TryShadow = TestValue;
                break;
            default:
                TrySubfolder = TestValue;
                break;
            }
        }
        auto FormattedName =
            FString::Format(TEXT("{0}{1}{2}{3}{4}"), {TrySubfolder, TrySpecies, TryForm, TryGender, TryShadow});
        FormattedStrings.Add(MoveTemp(FormattedName));
    }

    return FormattedStrings;
}