// "Unreal Pokémon" created by Retro & Chill.


#include "Utilities/PokemonAssetParams.h"
#include <cmath>

TArray<FString> FPokemonAssetParams::CreateResolutionList(FName Species, FStringView Subfolder) const {
    auto SubfolderString = !Subfolder.IsEmpty() ? FString::Format(TEXT("{0}/"), {Subfolder}) : FString();
    auto ShinyExtension = !Subfolder.IsEmpty() ? FString::Format(TEXT("{0}Shiny/"), {Subfolder}) : TEXT("_shiny");
    auto FormExtension = FString::Format(TEXT("_{0}"), {{Form.ToString()}});
    auto SpeciesExtension = Species.ToString();
    TArray<std::tuple<int32, FStringView, FStringView>> Factors;
    if (bShiny) {
        Factors.Emplace(4, ShinyExtension, SubfolderString);
    }
    if (bShadow) {
        Factors.Emplace(3, TEXT("_shadow"), TEXT(""));
    }
    if (Gender == EPokemonGender::Female) {
        Factors.Emplace(2, TEXT("_female"), TEXT(""));
    }
    if (!Form.IsNone()) {
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
        FormattedStrings.Add(std::move(FormattedName));
    }

    return FormattedStrings;
}