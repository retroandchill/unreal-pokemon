// "Unreal Pokémon" created by Retro & Chill.

#include "Graphics/GraphicsLoadingSubsystem.h"
#include "DynamicAssetLoadingSettings.h"
#include "Pokemon/Pokemon.h"
#include "PokemonDataSettings.h"
#include "range/v3/view/filter.hpp"
#include "range/v3/view/transform.hpp"
#include "Ranges/Algorithm/ToArray.h"
#include "Ranges/Optional/GetPtrOrNull.h"
#include "Ranges/Optional/Map.h"
#include "Ranges/Optional/Or.h"
#include "Ranges/Optional/OrElse.h"
#include "Ranges/Views/ContainerView.h"
#include "Ranges/Views/Extract.h"
#include "Ranges/Views/Map.h"
#include "Species/SpeciesData.h"
#include "TextureCompiler.h"
#include "Trainers/Trainer.h"
#include "Trainers/TrainerType.h"
#include "PaperFlipbook.h"
#include <cmath>

template <typename T>
    requires std::is_base_of_v<UObject, T>
static TOptional<T &> LookupAssetByName(FStringView BasePackageName, FStringView AssetName) {
    FStringView Prefix;
    if (int32 CharIndex; AssetName.FindLastChar('/', CharIndex)) {
        int32 PrefixLength = CharIndex + 1;
        Prefix = AssetName.SubStr(0, PrefixLength);
        AssetName = AssetName.RightChop(PrefixLength);
    }
    auto SearchKey = FString::Format(TEXT("{0}/{1}{2}.{2}"), {BasePackageName, Prefix, AssetName});
    return UE::Optionals::OfNullable<T>(
        Cast<T>(StaticLoadObject(T::StaticClass(), nullptr, *SearchKey, nullptr, LOAD_NoWarn)));
}

template <typename T>
    requires std::is_base_of_v<UObject, T>
static TOptional<T &> LookupAssetByName(FStringView BasePackageName, FName AssetName) {
    return LookupAssetByName<T>(BasePackageName, AssetName.ToString());
}

static FString GetFullAssetName(FName Identifier, FStringView Prefix) {
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
static TOptional<T &> ResolveAsset(FStringView BasePackageName, const TArray<FString> &Keys) {
    for (const auto &Key : Keys) {
        auto Lookup = LookupAssetByName<T>(BasePackageName, Key);
        if (!Lookup.IsSet()) {
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

    auto Settings = GetDefault<UDynamicAssetLoadingSettings>();
    PokemonSpriteMaterials = Settings->PokemonSprites;
    TrainerSpriteMaterials = Settings->TrainerSprites;
}

UPaperFlipbook* UGraphicsLoadingSubsystem::GetPokemonBattleSprite(const TScriptInterface<IPokemon>& Pokemon,
                                                                  bool bBack) const {
    return GetSpeciesBattleSprite(Pokemon->GetSpecies().ID, bBack, {.Gender = Pokemon->GetGender(), .bShiny = Pokemon->IsShiny()});
}

UPaperFlipbook* UGraphicsLoadingSubsystem::GetSpeciesBattleSprite(FName Species, bool bBack,
                                                                  const FPokemonAssetParams& AdditionalParams) const {
    auto &[AssetPath] = GetDefault<UDynamicAssetLoadingSettings>()->PokemonSpritePackageName;
    auto SpriteResolutionList =
        CreatePokemonSpriteResolutionList(Species, AdditionalParams, bBack ? TEXT("Back") : TEXT("Front"));

    // clang-format off
    return ResolveAsset<UPaperFlipbook>(AssetPath, SpriteResolutionList).GetPtrOrNull();
    // clang-format on
}

UPaperFlipbook* UGraphicsLoadingSubsystem::GetPokemonIcon(const TScriptInterface<IPokemon>& Pokemon) {
    return GetSpeciesIcon(Pokemon->GetSpecies().ID, {.Gender = Pokemon->GetGender()});
}

UPaperFlipbook* UGraphicsLoadingSubsystem::GetSpeciesIcon(FName Species,
                                                          const FPokemonAssetParams& AdditionalParams) {
    auto &[AssetPath] = GetDefault<UDynamicAssetLoadingSettings>()->PokemonSpritePackageName;
    auto SpriteResolutionList = CreatePokemonSpriteResolutionList(Species, AdditionalParams, TEXT("Icons"));
    return ResolveAsset<UPaperFlipbook>(AssetPath, SpriteResolutionList).GetPtrOrNull();
}

UPaperFlipbook *UGraphicsLoadingSubsystem::GetTrainerSprite(const TScriptInterface<ITrainer>& Trainer) const {
    return GetTrainerTypeSprite(Trainer->GetTrainerType().ID);
}

UPaperFlipbook *UGraphicsLoadingSubsystem::GetTrainerTypeSprite(FName TrainerType) const {
    auto &[AssetPath] = GetDefault<UDynamicAssetLoadingSettings>()->TrainerSpritesPackageName;
    // clang-format off
    return LookupAssetByName<UPaperFlipbook>(AssetPath, TrainerType.ToString()).GetPtrOrNull();
    // clang-format on
}

UObject *UGraphicsLoadingSubsystem::GetTypeIconGraphic(FName Type) const {
    auto &PathSettings = *GetDefault<UDynamicAssetLoadingSettings>();
    auto &[AssetPath] = PathSettings.TypeIconsPackageName;
    auto FullName = GetFullAssetName(Type, PathSettings.TypeIconPrefix);
    return LookupAssetByName<UObject>(AssetPath, FullName).GetPtrOrNull();
}

TArray<UObject *> UGraphicsLoadingSubsystem::GetTypeIconGraphics(const TArray<FName> &Types) const {
    auto &PathSettings = *GetDefault<UDynamicAssetLoadingSettings>();
    auto &[AssetPath] = PathSettings.TypeIconsPackageName;
    static_assert(UE::Ranges::FunctionalType<decltype(&GetFullAssetName)>);
    // clang-format off
    return Types |
           UE::Ranges::Map(&GetFullAssetName, PathSettings.TypeIconPrefix) |
           UE::Ranges::Map([&AssetPath](FStringView Name) {
               return LookupAssetByName<UObject>(AssetPath, Name);
           }) |
           UE::Ranges::Extract |
           UE::Ranges::ToArray;
    // clang-format on
}

UObject *UGraphicsLoadingSubsystem::GetStatusIconGraphic(FName Status) const {
    auto &PathSettings = *GetDefault<UDynamicAssetLoadingSettings>();
    auto &[AssetPath] = PathSettings.StatusIconsPackageName;
    auto FullName = GetFullAssetName(Status, PathSettings.StatusIconPrefix);
    return LookupAssetByName<UObject>(AssetPath, FullName).GetPtrOrNull();
}

UObject *UGraphicsLoadingSubsystem::GetTypePanelGraphic(FName Type) const {
    auto &PathSettings = *GetDefault<UDynamicAssetLoadingSettings>();
    auto &[AssetPath] = PathSettings.TypePanelsPackageName;
    auto FullName = GetFullAssetName(Type, PathSettings.TypePanelPrefix);
    return LookupAssetByName<UObject>(AssetPath, FullName).GetPtrOrNull();
}

UObject *UGraphicsLoadingSubsystem::GetPokeBallIcon(FName PokeBall) const {
    auto &PathSettings = *GetDefault<UDynamicAssetLoadingSettings>();
    auto &[AssetPath] = PathSettings.SummaryBallPackageName;
    auto FullName = GetFullAssetName(PokeBall, PathSettings.SummaryBallPrefix);
    // clang-format off
    return LookupAssetByName<UObject>(AssetPath, FullName) |
           UE::Optionals::Or([&AssetPath] {
               return LookupAssetByName<UObject>(AssetPath,
                                                 GetDefault<UPokemonDataSettings>()->DefaultPokeBall.ToString());
           }) |
           UE::Optionals::GetPtrOrNull;
    // clang-format on
}

UObject *UGraphicsLoadingSubsystem::GetItemIcon(FName ItemID) const {
    static const FName DefaultItem = "000";
    auto &[AssetPath] = GetDefault<UDynamicAssetLoadingSettings>()->ItemIconPackageName;
    // clang-format off
    return LookupAssetByName<UObject>(AssetPath, ItemID) |
           UE::Optionals::Or([&AssetPath] {
               return LookupAssetByName<UObject>(AssetPath, DefaultItem);
           }) |
           UE::Optionals::GetPtrOrNull;
    // clang-format on
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