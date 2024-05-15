// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Summary/TrainerMemoPage.h"
#include "Mainpulation/RangeHelpers.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/Stats/StatBlock.h"
#include "Pokemon/TrainerMemo/ObtainedBlock.h"
#include "Species/Nature.h"
#include "Species/Stat.h"

#define MEMO_TEXT(Key, Text) NSLOCTEXT("UTrainerMemoPage", Key, Text)

FCharacteristicList::FCharacteristicList(TArray<FText> &&Characteristics) : Characteristics(MoveTemp(Characteristics)) {
}

UTrainerMemoPage::UTrainerMemoPage(const FObjectInitializer &Initializer)
    : USummaryScreenPage(Initializer), NatureLineFormat(MEMO_TEXT("NatureLineFormat", "<Blue>{Nature}</> nature.")),
      DateLineFormat(TEXT("%b %e, %Y")), UnknownObtainLocation(MEMO_TEXT("UnknownObtainLocation", "Faraway place")),
      LocationFormatting(MEMO_TEXT("LocationFormatting", "<Red>{Location}</>")),
      ObtainLocationFormats({{EObtainMethod::Default, MEMO_TEXT("ObtainLocationDefault", "Met at Lv. {Level}.")},
                             {EObtainMethod::Egg, MEMO_TEXT("ObtainLocationEgg", "Egg received.")},
                             {EObtainMethod::Trade, MEMO_TEXT("ObtainLocationTrade", "Traded at Lv. {Level}.")},
                             {EObtainMethod::FatefulEncounter,
                              MEMO_TEXT("ObtainLocationFatefulEncounter", "Had a fateful encounter at Lv. {Level}.")}}),
      EggHatchedText(MEMO_TEXT("EggHatchedText", "Egg hatched.")),
      StatsOrder({"HP", "ATTACK", "DEFENSE", "SPEED", "SPECIAL_ATTACK", "SPECIAL_DEFENSE"}),
      Characteristics({
          {"HP", FCharacteristicList({MEMO_TEXT("HP1", "Loves to eat."), MEMO_TEXT("HP2", "Takes plenty of siestas."),
                                      MEMO_TEXT("HP3", "Nods off a lot."), MEMO_TEXT("HP4", "Scatters things often."),
                                      MEMO_TEXT("HP5", "Likes to relax.")})},
          {"ATTACK", FCharacteristicList(
                         {MEMO_TEXT("ATTACK1", "Proud of its power."), MEMO_TEXT("ATTACK2", "Likes to thrash about."),
                          MEMO_TEXT("ATTACK3", "A little quick tempered."), MEMO_TEXT("ATTACK4", "Likes to fight."),
                          MEMO_TEXT("ATTACK5", "Quick tempered.")})},
          {"DEFENSE",
           FCharacteristicList({MEMO_TEXT("DEFENSE1", "Sturdy body."), MEMO_TEXT("DEFENSE2", "Capable of taking hits."),
                                MEMO_TEXT("DEFENSE3", "Highly persistent."), MEMO_TEXT("DEFENSE4", "Good endurance."),
                                MEMO_TEXT("DEFENSE5", "Good perseverance.")})},
          {"SPECIAL_ATTACK", FCharacteristicList({MEMO_TEXT("SPECIAL_ATTACK1", "Highly curious."),
                                                  MEMO_TEXT("SPECIAL_ATTACK2", "Mischievous."),
                                                  MEMO_TEXT("SPECIAL_ATTACK3", "Thoroughly cunning."),
                                                  MEMO_TEXT("SPECIAL_ATTACK4", "Often lost in thought."),
                                                  MEMO_TEXT("SPECIAL_ATTACK5", "Very finicky.")})},
          {"SPECIAL_DEFENSE",
           FCharacteristicList(
               {MEMO_TEXT("SPECIAL_DEFENSE1", "Strong willed."), MEMO_TEXT("SPECIAL_DEFENSE2", "Somewhat vain."),
                MEMO_TEXT("SPECIAL_DEFENSE3", "Strongly defiant."), MEMO_TEXT("SPECIAL_DEFENSE4", "Hates to lose."),
                MEMO_TEXT("SPECIAL_DEFENSE5", "Somewhat stubborn.")})},
          {"SPEED",
           FCharacteristicList({MEMO_TEXT("SPEED1", "Likes to run."), MEMO_TEXT("SPEED2", "Alert to sounds."),
                                MEMO_TEXT("SPEED3", "Impetuous and silly."),
                                MEMO_TEXT("SPEED4", "Somewhat of a clown."), MEMO_TEXT("SPEED5", "Quick to flee.")})},
      }) {
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
    Lines.Emplace(FormatDate(ObtainedInformation->GetTimeReceived()));
    auto ObtainedLocation = ObtainedInformation->GetObtainText().Get(UnknownObtainLocation);
    Lines.Emplace(FormatLocation(ObtainedLocation));

    auto ObtainMethod = ObtainedInformation->GetObtainMethod();
    if (ObtainLocationFormats.Contains(ObtainMethod)) {
        Lines.Emplace(FText::Format(ObtainLocationFormats[ObtainMethod], {{TEXT("Level"), StatBlock->GetLevel()}}));
    }

    if (ObtainMethod == EObtainMethod::Egg) {
        if (auto &TimeHatched = ObtainedInformation->GetTimeHatched(); TimeHatched.IsSet()) {
            Lines.Emplace(FormatDate(TimeHatched.GetValue()));
        }

        auto HatchedLocation = ObtainedInformation->GetHatchedMap().Get(UnknownObtainLocation);
        Lines.Emplace(HatchedLocation);
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

    auto JoinedString =
        FString::Join(RangeHelpers::CreateRange(Lines) |
                          std::views::transform([](const FText &Text) -> const FString & { return Text.ToString(); }),
                      TEXT("\n"));
    auto JoinedText = FText::FromString(MoveTemp(JoinedString));
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