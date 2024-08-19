// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Summary/TrainerMemoPage.h"
#include "Components/DisplayText.h"
#include "Components/Party/PokemonSelectionPane.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/Stats/StatBlock.h"
#include "Pokemon/TrainerMemo/ObtainedBlock.h"
#include "Ranges/Algorithm/ToString.h"
#include "Ranges/Optional/IfPresent.h"
#include "Ranges/Optional/Map.h"
#include "Ranges/Optional/OptionalClosure.h"
#include "Ranges/Optional/OrElse.h"
#include "Species/Nature.h"

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

    auto EmplaceDate = [this, &Lines](const FDateTime& D) {
        Lines.Emplace(FormatDate(D));
    };

    auto ObtainedInformation = Pokemon->GetObtainedInformation();
    ObtainedInformation->GetTimeReceived() | UE::Optionals::IfPresent(EmplaceDate);

    auto TextCheck = [this](const FText &Text) {
        return Text.IsEmptyOrWhitespace() ? UnknownObtainLocation : Text;
    };
    auto ObtainedLocation = ObtainedInformation->GetObtainText() |
        UE::Optionals::Map(TextCheck) |
        UE::Optionals::OrElse(UnknownObtainLocation);
    Lines.Emplace(FormatLocation(ObtainedLocation));

    auto ObtainMethod = ObtainedInformation->GetObtainMethod();
    if (ObtainLocationFormats.Contains(ObtainMethod)) {
        Lines.Emplace(FText::Format(ObtainLocationFormats[ObtainMethod], {{TEXT("Level"), StatBlock->GetLevel()}}));
    }

    if (ObtainMethod == EObtainMethod::Egg) {
        ObtainedInformation->GetTimeHatched() | UE::Optionals::IfPresent(EmplaceDate);
        auto HatchedLocation = ObtainedInformation->GetHatchedMap() |
            UE::Optionals::Map(TextCheck) |
            UE::Optionals::OrElse(UnknownObtainLocation);
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

    MemoBlock->SetText(FText::FromString(Lines |
        UE::Ranges::Map(&FText::ToString) |
        UE::Ranges::ToString(TEXT("\n"))));
}

FText UTrainerMemoPage::FormatDate(const FDateTime &DateTime) const {
    return FText::FromString(DateTime.ToFormattedString(*DateLineFormat));
}

FText UTrainerMemoPage::FormatLocation(const FText& Location) const {
    return FText::Format(LocationFormatting, {{TEXT("Location"), Location}});
}

bool UTrainerMemoPage::ShowNature_Implementation() const {
    return true;
}