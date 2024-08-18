// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Summary/TrainerMemoPage.h"
#include "CommonRichTextBlock.h"
#include "Components/DisplayText.h"
#include "Components/Party/PokemonSelectionPane.h"
#include "DataTypes/OptionalUtilities.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/Stats/StatBlock.h"
#include "Pokemon/TrainerMemo/ObtainedBlock.h"
#include "Species/Nature.h"
#include "Species/Stat.h"
#include <range/v3/view/transform.hpp>

FCharacteristicList::FCharacteristicList() = default;

FCharacteristicList::FCharacteristicList(TArray<FText> &&Characteristics) : Characteristics(MoveTemp(Characteristics)) {
}

void UTrainerMemoPage::RefreshInfo_Implementation(const TScriptInterface<IPokemon> &Pokemon) {
    Super::RefreshInfo_Implementation(Pokemon);

    TArray<FText> Lines;
    auto StatBlock = Pokemon->GetStatBlock();
    bool bShowNature = ShowNature();
    if (bShowNature) {
        auto NatureName = StatBlock->GetNature().RealName;
        Lines.Emplace(FText::Format(NatureLineFormat, {{TEXT("Nature"), NatureName}}));
    }

    auto ObtainedInformation = Pokemon->GetObtainedInformation();
    if (auto TimeReceived = ObtainedInformation->GetTimeReceived(); TimeReceived != nullptr) {
        Lines.Emplace(FormatDate(*TimeReceived));
    }

    TFunctionRef<FText(const FText &Text)> TextCheck = [this](const FText &Text) {
        return Text.IsEmptyOrWhitespace() ? UnknownObtainLocation : Text;
    };
    auto ObtainedLocation =
        OptionalUtilities::Map(ObtainedInformation->GetObtainText(), TextCheck).Get(UnknownObtainLocation);
    Lines.Emplace(FormatLocation(ObtainedLocation));

    auto ObtainMethod = ObtainedInformation->GetObtainMethod();
    if (ObtainLocationFormats.Contains(ObtainMethod)) {
        Lines.Emplace(FText::Format(ObtainLocationFormats[ObtainMethod], {{TEXT("Level"), StatBlock->GetLevel()}}));
    }

    if (ObtainMethod == EObtainMethod::Egg) {
        if (auto TimeHatched = ObtainedInformation->GetTimeHatched(); TimeHatched != nullptr) {
            Lines.Emplace(FormatDate(*TimeHatched));
        }

        auto HatchedLocation =
            OptionalUtilities::Map(ObtainedInformation->GetHatchedMap(), TextCheck).Get(UnknownObtainLocation);
        Lines.Emplace(FormatLocation(HatchedLocation));
        Lines.Emplace(EggHatchedText);
    } else {
        Lines.Emplace(FText::FromStringView(TEXT("")));
    }

    if (bShowNature) {
        FName BestStat;
        int32 BestIV = 0;
        auto StartPoint = static_cast<int32>(Pokemon->GetPersonalityValue() % StatsOrder.Num());
        for (int i = 0; i < StatsOrder.Num(); i++) {
            if (auto Stat = StatsOrder[(i + StartPoint) % StatsOrder.Num()];
                BestStat.IsNone() || StatBlock->GetStat(Stat)->GetIV() > StatBlock->GetStat(BestStat)->GetIV()) {
                BestStat = Stat;
                BestIV = StatBlock->GetStat(Stat)->GetIV();
            }
        }

        const auto &CharacteristicList = Characteristics[BestStat].Characteristics;
        Lines.Emplace(CharacteristicList[BestIV % CharacteristicList.Num()]);
    }

    auto JoinedString = FString::Join(
        Lines |
            ranges::views::transform([](const FText &Text) -> const FString & { return Text.ToString(); }),
        TEXT("\n"));
    MemoBlock->SetText(FText::FromString(MoveTemp(JoinedString)));
}

FText UTrainerMemoPage::FormatDate(const FDateTime &DateTime) const {
    return FText::FromString(DateTime.ToFormattedString(*DateLineFormat));
}

FText UTrainerMemoPage::FormatLocation(FText Location) const {
    return FText::Format(LocationFormatting, {{TEXT("Location"), Location}});
}

bool UTrainerMemoPage::ShowNature_Implementation() const {
    return true;
}