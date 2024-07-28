
#include "Asserts.h"
#include "Components/DisplayText.h"
#include "Components/Image.h"
#include "Components/Summary/PokemonInfoPage.h"
#include "Components/Summary/PokemonMovesPage.h"
#include "Components/Summary/PokemonSkillsPage.h"
#include "Components/Summary/PokemonStatRow.h"
#include "Components/Summary/SummaryNameInfo.h"
#include "Components/Summary/TrainerMemoPage.h"
#include "Lookup/InjectionUtilities.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/PokemonDTO.h"
#include "Pokemon/TrainerMemo/ObtainedBlock.h"
#include "Trainers/BasicTrainer.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PokemonSummaryPagesTest_NameInfo,
                                 "Unit Tests.UI.Summary.Components.PokemonSummaryPagesTest.NameInfo",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool PokemonSummaryPagesTest_NameInfo::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<USummaryNameInfo>();
    UE_ASSERT_NOT_EQUAL(0, Subclasses.Num());
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
    UE_ASSERT_NOT_NULL(PokemonNameText);
    UE_CHECK_EQUAL(TEXT("Kabutops"), PokemonNameText->GetText().ToString());

    FIND_CHILD_WIDGET(Page, UDisplayText, PokemonLevelText);
    UE_ASSERT_NOT_NULL(PokemonLevelText);
    UE_CHECK_EQUAL(TEXT("30"), PokemonLevelText->GetText().ToString());

    FIND_CHILD_WIDGET(Page, UDisplayText, PokemonGenderText);
    UE_ASSERT_NOT_NULL(PokemonGenderText);
    UE_CHECK_EQUAL(TEXT("♀"), PokemonGenderText->GetText().ToString());

    FIND_CHILD_WIDGET(Page, UImage, PokemonStatusIcon);
    UE_ASSERT_NOT_NULL(PokemonStatusIcon);
    UE_CHECK_EQUAL(ESlateVisibility::Hidden, PokemonStatusIcon->GetVisibility());

    auto Pokemon2 = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(), FPokemonDTO{.Species = "OMASTAR", .Level = 60, .Gender = EPokemonGender::Male}, ForeignTrainer);
    Page->Refresh(Pokemon2);

    UE_CHECK_EQUAL(TEXT("Omastar"), PokemonNameText->GetText().ToString());
    UE_CHECK_EQUAL(TEXT("60"), PokemonLevelText->GetText().ToString());
    UE_CHECK_EQUAL(TEXT("♂"), PokemonGenderText->GetText().ToString());
    UE_CHECK_EQUAL(ESlateVisibility::Hidden, PokemonStatusIcon->GetVisibility());

    auto Pokemon3 = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(), FPokemonDTO{.Species = "VOLTORB", .Level = 10, .Nickname = FText::FromStringView(TEXT("Volty"))},
        ForeignTrainer);
    Page->Refresh(Pokemon3);

    UE_CHECK_EQUAL(TEXT("Volty"), PokemonNameText->GetText().ToString());
    UE_CHECK_EQUAL(TEXT("10"), PokemonLevelText->GetText().ToString());
    UE_CHECK_EQUAL(TEXT(""), PokemonGenderText->GetText().ToString());
    UE_CHECK_EQUAL(ESlateVisibility::Hidden, PokemonStatusIcon->GetVisibility());

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PokemonSummaryPagesTest_PokemonInfo,
                                 "Unit Tests.UI.Summary.Components.PokemonSummaryPagesTest.PokemonInfo",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool PokemonSummaryPagesTest_PokemonInfo::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UPokemonInfoPage>();
    UE_ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    auto Page = CreateWidget<UPokemonInfoPage>(World.Get(), WidgetClass);
    Page->AddToViewport();

    auto ForeignTrainer = NewObject<UBasicTrainer>()->Initialize(TEXT("LASS"), FText::FromStringView(TEXT("Amy")));
    auto Pokemon = UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = "KABUTOPS"},
                                                                   ForeignTrainer);

    Page->RefreshInfo(Pokemon);

    FIND_CHILD_WIDGET(Page, UDisplayText, SpeciesNameText);
    UE_ASSERT_NOT_NULL(SpeciesNameText);
    UE_CHECK_EQUAL(TEXT("Kabutops"), SpeciesNameText->GetText().ToString());

    FIND_CHILD_WIDGET(Page, UDisplayText, OTNameText);
    UE_ASSERT_NOT_NULL(OTNameText);
    UE_CHECK_EQUAL(ForeignTrainer->GetTrainerName().ToString(), OTNameText->GetText().ToString());

    FIND_CHILD_WIDGET(Page, UDisplayText, PokemonIDText);
    UE_ASSERT_NOT_NULL(PokemonIDText);
    UE_CHECK_EQUAL(ForeignTrainer->GetIdNumber(), FCString::Atoi(*PokemonIDText->GetText().ToString()));

    UE_CHECK_FALSE(Page->CanSelectOnPage());

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PokemonSummaryPagesTest_TrainerMemo,
                                 "Unit Tests.UI.Summary.Components.PokemonSummaryPagesTest.TrainerMemo",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool PokemonSummaryPagesTest_TrainerMemo::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UTrainerMemoPage>();
    UE_ASSERT_NOT_EQUAL(0, Subclasses.Num());
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
    UE_ASSERT_NOT_NULL(MemoBlock);
    TArray<FString> Lines;
    MemoBlock->GetText().ToString().ParseIntoArrayLines(Lines);
    UE_ASSERT_EQUAL(5, Lines.Num());

    // Test what all the lines contain
    UE_CHECK_EQUAL(TEXT("<Blue>Adamant</> nature."), Lines[0]); // Nature
    UE_CHECK_EQUAL(TEXT("<Red>Faraway Place</>"), Lines[2]);    // Met Location
    UE_CHECK_EQUAL(TEXT("Met at Lv. 40."), Lines[3]);           // Met Level
    UE_CHECK_EQUAL(TEXT("Likes to thrash about."), Lines[4]);   // Characteristic

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
    UE_ASSERT_EQUAL(8, Lines.Num());

    UE_CHECK_EQUAL(TEXT("<Blue>Jolly</> nature."), Lines[0]); // Nature
    UE_CHECK_EQUAL(TEXT("<Red>Daycare Couple</>"), Lines[2]); // Met Location
    UE_CHECK_EQUAL(TEXT("Egg received."), Lines[3]);          // Met Level
    UE_CHECK_EQUAL(TEXT("<Red>Unit Test</>"), Lines[5]);      // Hatched Location
    UE_CHECK_EQUAL(TEXT("Egg hatched."), Lines[6]);           // Egg Hatched
    UE_CHECK_EQUAL(TEXT("Likes to thrash about."), Lines[7]); // Characteristic

    UE_CHECK_FALSE(Page->CanSelectOnPage());

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PokemonSummaryPagesTest_Skills,
                                 "Unit Tests.UI.Summary.Components.PokemonSummaryPagesTest.Skills",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool PokemonSummaryPagesTest_Skills::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UPokemonSkillsPage>();
    UE_ASSERT_NOT_EQUAL(0, Subclasses.Num());
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
    UE_ASSERT_NOT_NULL(AbilityName);
    FIND_CHILD_WIDGET(Page, UDisplayText, AbilityDescription);
    UE_ASSERT_NOT_NULL(AbilityDescription);

    UE_CHECK_EQUAL(TEXT("Battle Armor"), AbilityName->GetText().ToString());
    UE_CHECK_EQUAL(TEXT("The Pokémon is protected against critical hits."), AbilityDescription->GetText().ToString());

    auto &StatValues = UReflectionUtils::GetPropertyValue<TArray<TObjectPtr<UPokemonStatRow>>>(Page, TEXT("StatRows"));
    UE_ASSERT_EQUAL(6, StatValues.Num());

    TArray<FString> Values = {
        TEXT("HP"), TEXT("Atk"), TEXT("Def"), TEXT("SpAtk"), TEXT("SpDef"), TEXT("Spd")};
    for (int i = 0; i < Values.Num(); i++) {
        FIND_CHILD_WIDGET(StatValues[i], UDisplayText, StatLabel);
        UE_ASSERT_NOT_NULL(StatLabel);

        UE_CHECK_EQUAL(Values[i], StatLabel->GetText().ToString());
    }

    UE_CHECK_FALSE(Page->CanSelectOnPage());

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PokemonSummaryPagesTest_Moves,
                                 "Unit Tests.UI.Summary.Components.PokemonSummaryPagesTest.Moves",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool PokemonSummaryPagesTest_Moves::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UPokemonMovesPage>();
    UE_ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    auto Page = CreateWidget<UPokemonMovesPage>(World.Get(), WidgetClass);
    Page->AddToViewport();

    auto ForeignTrainer = NewObject<UBasicTrainer>()->Initialize(TEXT("LASS"), FText::FromStringView(TEXT("Amy")));
    auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(), FPokemonDTO{.Species = "KABUTOPS", .Level = 40}, ForeignTrainer);
    Page->RefreshInfo(Pokemon1);

    UE_CHECK_TRUE(Page->CanSelectOnPage());

    return true;
}