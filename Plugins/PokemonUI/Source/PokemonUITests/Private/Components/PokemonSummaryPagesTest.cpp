
#include "Asserts.h"
#include "Components/Image.h"
#include "Components/Summary/HoldItemInfo.h"
#include "Components/Summary/PokemonInfoPage.h"
#include "Components/Summary/PokemonMovesPage.h"
#include "Components/Summary/PokemonSkillsPage.h"
#include "Components/Summary/PokemonStatRow.h"
#include "Components/Summary/SummaryNameInfo.h"
#include "Components/Summary/TrainerMemoPage.h"
#include "External/accessor.hpp"
#include "Lookup/InjectionUtilities.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/PokemonDTO.h"
#include "Pokemon/TrainerMemo/ObtainedBlock.h"
#include "Primatives/DisplayText.h"
#include "Trainers/BasicTrainer.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"

using namespace accessor;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PokemonSummaryPagesTest_NameInfo,
                                 "Unit Tests.UI.Summary.Components.PokemonSummaryPagesTest.NameInfo",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool PokemonSummaryPagesTest_NameInfo::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<USummaryNameInfo>();
    ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    auto Page = CreateWidget<USummaryNameInfo>(World.Get(), WidgetClass);
    Page->AddToViewport();

    auto ForeignTrainer = NewObject<UBasicTrainer>()->Initialize(TEXT("LASS"), FText::FromStringView(TEXT("Amy")));
    auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(),
        FPokemonDTO{
            .Species = "KABUTOPS", .Level = 30, .Gender = EPokemonGender::Female, .PokeBall = FName("ULTRABALL")},
        ForeignTrainer);

    Page->Refresh(Pokemon1);

    FIND_CHILD_WIDGET(Page, UDisplayText, PokemonNameText);
    ASSERT_NOT_NULL(PokemonNameText);
    CHECK_EQUAL(TEXT("Kabutops"), PokemonNameText->GetText().ToString());

    FIND_CHILD_WIDGET(Page, UDisplayText, PokemonLevelText);
    ASSERT_NOT_NULL(PokemonLevelText);
    CHECK_EQUAL(TEXT("30"), PokemonLevelText->GetText().ToString());

    FIND_CHILD_WIDGET(Page, UDisplayText, PokemonGenderText);
    ASSERT_NOT_NULL(PokemonGenderText);
    CHECK_EQUAL(TEXT("♀"), PokemonGenderText->GetText().ToString());

    FIND_CHILD_WIDGET(Page, UImage, PokemonStatusIcon);
    ASSERT_NOT_NULL(PokemonStatusIcon);
    CHECK_EQUAL(ESlateVisibility::Hidden, PokemonStatusIcon->GetVisibility());

    auto Pokemon2 = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(), FPokemonDTO{.Species = "OMASTAR", .Level = 60, .Gender = EPokemonGender::Male}, ForeignTrainer);
    Page->Refresh(Pokemon2);

    CHECK_EQUAL(TEXT("Omastar"), PokemonNameText->GetText().ToString());
    CHECK_EQUAL(TEXT("60"), PokemonLevelText->GetText().ToString());
    CHECK_EQUAL(TEXT("♂"), PokemonGenderText->GetText().ToString());
    CHECK_EQUAL(ESlateVisibility::Hidden, PokemonStatusIcon->GetVisibility());

    auto Pokemon3 = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(), FPokemonDTO{.Species = "VOLTORB", .Level = 10, .Nickname = FText::FromStringView(TEXT("Volty"))},
        ForeignTrainer);
    Page->Refresh(Pokemon3);

    CHECK_EQUAL(TEXT("Volty"), PokemonNameText->GetText().ToString());
    CHECK_EQUAL(TEXT("10"), PokemonLevelText->GetText().ToString());
    CHECK_EQUAL(TEXT(""), PokemonGenderText->GetText().ToString());
    CHECK_EQUAL(ESlateVisibility::Hidden, PokemonStatusIcon->GetVisibility());

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PokemonSummaryPagesTest_HoldItemInfo,
                                 "Unit Tests.UI.Summary.Components.PokemonSummaryPagesTest.HoldItemInfo",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool PokemonSummaryPagesTest_HoldItemInfo::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UHoldItemInfo>();
    ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    auto Page = CreateWidget<UHoldItemInfo>(World.Get(), WidgetClass);
    Page->AddToViewport();

    auto ForeignTrainer = NewObject<UBasicTrainer>()->Initialize(TEXT("LASS"), FText::FromStringView(TEXT("Amy")));
    auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(), FPokemonDTO{.Species = "KABUTOPS", .Shiny = true, .Item = FName("MYSTICWATER")}, ForeignTrainer);

    Page->Refresh(Pokemon1);

    FIND_CHILD_WIDGET(Page, UDisplayText, ItemNameText);
    ASSERT_NOT_NULL(ItemNameText);
    CHECK_EQUAL(TEXT("Mystic Water"), ItemNameText->GetText().ToString());

    FIND_CHILD_WIDGET(Page, UImage, ItemIcon);
    ASSERT_NOT_NULL(ItemIcon);
    CHECK_EQUAL(ESlateVisibility::SelfHitTestInvisible, ItemIcon->GetVisibility());

    FIND_CHILD_WIDGET(Page, UImage, ShinyIcon);
    ASSERT_NOT_NULL(ShinyIcon);
    CHECK_EQUAL(ESlateVisibility::SelfHitTestInvisible, ShinyIcon->GetVisibility());

    auto Pokemon2 = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(), FPokemonDTO{.Species = "OMASTAR", .Shiny = false}, ForeignTrainer);
    Page->Refresh(Pokemon2);

    CHECK_EQUAL(TEXT("None"), ItemNameText->GetText().ToString());
    CHECK_EQUAL(ESlateVisibility::Hidden, ItemIcon->GetVisibility());
    CHECK_EQUAL(ESlateVisibility::Hidden, ShinyIcon->GetVisibility());

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PokemonSummaryPagesTest_PokemonInfo,
                                 "Unit Tests.UI.Summary.Components.PokemonSummaryPagesTest.PokemonInfo",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool PokemonSummaryPagesTest_PokemonInfo::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UPokemonInfoPage>();
    ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    auto Page = CreateWidget<UPokemonInfoPage>(World.Get(), WidgetClass);
    Page->AddToViewport();

    auto ForeignTrainer = NewObject<UBasicTrainer>()->Initialize(TEXT("LASS"), FText::FromStringView(TEXT("Amy")));
    auto Pokemon = UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = "KABUTOPS"},
                                                                   ForeignTrainer);

    Page->RefreshInfo(Pokemon);

    FIND_CHILD_WIDGET(Page, UDisplayText, SpeciesNameText);
    ASSERT_NOT_NULL(SpeciesNameText);
    CHECK_EQUAL(TEXT("Kabutops"), SpeciesNameText->GetText().ToString());

    FIND_CHILD_WIDGET(Page, UDisplayText, OTNameText);
    ASSERT_NOT_NULL(OTNameText);
    CHECK_EQUAL(ForeignTrainer->GetTrainerName().ToString(), OTNameText->GetText().ToString());

    FIND_CHILD_WIDGET(Page, UDisplayText, PokemonIDText);
    ASSERT_NOT_NULL(PokemonIDText);
    CHECK_EQUAL(ForeignTrainer->GetIdNumber(), FCString::Atoi(*PokemonIDText->GetText().ToString()));

    CHECK_FALSE(Page->CanSelectOnPage());

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PokemonSummaryPagesTest_TrainerMemo,
                                 "Unit Tests.UI.Summary.Components.PokemonSummaryPagesTest.TrainerMemo",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool PokemonSummaryPagesTest_TrainerMemo::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UTrainerMemoPage>();
    ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    auto Page = CreateWidget<UTrainerMemoPage>(World.Get(), WidgetClass);
    Page->AddToViewport();

    auto ForeignTrainer = NewObject<UBasicTrainer>()->Initialize(TEXT("LASS"), FText::FromStringView(TEXT("Amy")));
    auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(),
                                                                    FPokemonDTO{.Species = "KABUTOPS",
                                                                                .Level = 40,
                                                                                .IVs = {{"HP", 30},
                                                                                        {"ATTACK", 31},
                                                                                        {"DEFENSE", 30},
                                                                                        {"SPECIAL_ATTACK", 30},
                                                                                        {"SPECIAL_DEFENSE", 30},
                                                                                        {"SPEED", 30}},
                                                                                .Nature = FName("Adamant")},
                                                                    ForeignTrainer);
    Page->RefreshInfo(Pokemon1);

    FIND_CHILD_WIDGET(Page, UDisplayText, MemoBlock);
    ASSERT_NOT_NULL(MemoBlock);
    TArray<FString> Lines;
    MemoBlock->GetText().ToString().ParseIntoArrayLines(Lines);
    ASSERT_EQUAL(5, Lines.Num());

    // Test what all the lines contain
    CHECK_EQUAL(TEXT("<Blue>Adamant</> nature."), Lines[0]); // Nature
    CHECK_EQUAL(TEXT("<Red>Faraway Place</>"), Lines[2]);    // Met Location
    CHECK_EQUAL(TEXT("Met at Lv. 40."), Lines[3]);           // Met Level
    CHECK_EQUAL(TEXT("Likes to thrash about."), Lines[4]);   // Characteristic

    auto Pokemon2 = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(),
        FPokemonDTO{.Species = "KABUTO",
                    .Level = 1,
                    .IVs = {{"HP", 30},
                            {"ATTACK", 31},
                            {"DEFENSE", 30},
                            {"SPECIAL_ATTACK", 30},
                            {"SPECIAL_DEFENSE", 30},
                            {"SPEED", 30}},
                    .Nature = FName("JOLLY"),
                    .ObtainMethod = EObtainMethod::Egg,
                    .MetLocation = FText::FromStringView(TEXT("Daycare Couple"))},
        ForeignTrainer);
    auto ObtainedBlock = Pokemon2->GetObtainedInformation();
    ObtainedBlock->SetTimeHatched(FDateTime::Now());
    ObtainedBlock->SetHatchedMap(FText::FromStringView(TEXT("Unit Test")));
    Page->RefreshInfo(Pokemon2);

    Lines.Empty();
    MemoBlock->GetText().ToString().ParseIntoArrayLines(Lines);
    ASSERT_EQUAL(8, Lines.Num());

    CHECK_EQUAL(TEXT("<Blue>Jolly</> nature."), Lines[0]); // Nature
    CHECK_EQUAL(TEXT("<Red>Daycare Couple</>"), Lines[2]); // Met Location
    CHECK_EQUAL(TEXT("Egg received."), Lines[3]);          // Met Level
    CHECK_EQUAL(TEXT("<Red>Unit Test</>"), Lines[5]);      // Hatched Location
    CHECK_EQUAL(TEXT("Egg hatched."), Lines[6]);           // Egg Hatched
    CHECK_EQUAL(TEXT("Likes to thrash about."), Lines[7]); // Characteristic

    CHECK_FALSE(Page->CanSelectOnPage());

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PokemonSummaryPagesTest_Skills,
                                 "Unit Tests.UI.Summary.Components.PokemonSummaryPagesTest.Skills",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool PokemonSummaryPagesTest_Skills::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UPokemonSkillsPage>();
    ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    auto Page = CreateWidget<UPokemonSkillsPage>(World.Get(), WidgetClass);
    Page->AddToViewport();

    auto ForeignTrainer = NewObject<UBasicTrainer>()->Initialize(TEXT("LASS"), FText::FromStringView(TEXT("Amy")));
    auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(),
                                                                    FPokemonDTO{.Species = "KABUTOPS",
                                                                                .Level = 40,
                                                                                .IVs = {{"HP", 30},
                                                                                        {"ATTACK", 31},
                                                                                        {"DEFENSE", 30},
                                                                                        {"SPECIAL_ATTACK", 30},
                                                                                        {"SPECIAL_DEFENSE", 30},
                                                                                        {"SPEED", 30}},
                                                                                .Nature = FName("Adamant"),
                                                                                .Ability = FName("BATTLEARMOR")},
                                                                    ForeignTrainer);
    Page->RefreshInfo(Pokemon1);

    FIND_CHILD_WIDGET(Page, UDisplayText, AbilityName);
    ASSERT_NOT_NULL(AbilityName);
    FIND_CHILD_WIDGET(Page, UDisplayText, AbilityDescription);
    ASSERT_NOT_NULL(AbilityDescription);

    CHECK_EQUAL(TEXT("Battle Armor"), AbilityName->GetText().ToString());
    CHECK_EQUAL(TEXT("The Pokémon is protected against critical hits."), AbilityDescription->GetText().ToString());

    auto &StatValues = UReflectionUtils::GetPropertyValue<TArray<TObjectPtr<UPokemonStatRow>>>(Page, TEXT("StatRows"));
    ASSERT_EQUAL(6, StatValues.Num());

    TArray<FString> Values = {
        TEXT("HP"), TEXT("<Boosted>Atk</>"), TEXT("Def"), TEXT("<Decreased>SpAtk</>"), TEXT("SpDef"), TEXT("Spd")};
    for (int i = 0; i < Values.Num(); i++) {
        FIND_CHILD_WIDGET(StatValues[i], UDisplayText, StatLabel);
        ASSERT_NOT_NULL(StatLabel);

        CHECK_EQUAL(Values[i], StatLabel->GetText().ToString());
    }

    CHECK_FALSE(Page->CanSelectOnPage());

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PokemonSummaryPagesTest_Moves,
                                 "Unit Tests.UI.Summary.Components.PokemonSummaryPagesTest.Moves",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool PokemonSummaryPagesTest_Moves::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UPokemonMovesPage>();
    ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    auto Page = CreateWidget<UPokemonMovesPage>(World.Get(), WidgetClass);
    Page->AddToViewport();

    auto ForeignTrainer = NewObject<UBasicTrainer>()->Initialize(TEXT("LASS"), FText::FromStringView(TEXT("Amy")));
    auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(), FPokemonDTO{.Species = "KABUTOPS", .Level = 40}, ForeignTrainer);
    Page->RefreshInfo(Pokemon1);

    CHECK_TRUE(Page->CanSelectOnPage());

    return true;
}