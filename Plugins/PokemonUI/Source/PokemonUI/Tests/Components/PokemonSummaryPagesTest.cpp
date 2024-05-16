#include "Asserts.h"
#include "RPGPlayerController.h"
#include "Components/Image.h"
#include "Components/Summary/PokemonInfoPage.h"
#include "Components/Summary/SummaryNameInfo.h"
#include "Misc/AutomationTest.h"
#include "Trainers/BasicTrainer.h"
#include "Utilities/ConstructionUtilities.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"
#include "Pokemon/PokemonDTO.h"
#include "Primatives/DisplayText.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PokemonSummaryPagesTest_NameInfo, "Unit Tests.UI.Summary.Components.PokemonSummaryPagesTest.NameInfo",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool PokemonSummaryPagesTest_NameInfo::RunTest(const FString &Parameters) {
    auto [DudOverlay, World] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<USummaryNameInfo>();
    ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    auto Page = CreateWidget<USummaryNameInfo>(World, WidgetClass);
    Page->AddToViewport();

    auto ForeignTrainer = NewObject<UBasicTrainer>()->Initialize(TEXT("LASS"), FText::FromStringView(TEXT("Amy")));
    auto Pokemon1 = UConstructionUtilities::CreateForeignPokemon({
        .Species = "KABUTOPS",
        .Level = 30,
        .Gender = EPokemonGender::Female,
        .PokeBall = FName("ULTRABALL")
    }, ForeignTrainer);

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

    auto Pokemon2 = UConstructionUtilities::CreateForeignPokemon({
        .Species = "OMASTAR",
        .Level = 60,
        .Gender = EPokemonGender::Male
    }, ForeignTrainer);
    Page->Refresh(Pokemon2);
    
    CHECK_EQUAL(TEXT("Omastar"), PokemonNameText->GetText().ToString());
    CHECK_EQUAL(TEXT("60"), PokemonLevelText->GetText().ToString());
    CHECK_EQUAL(TEXT("♂"), PokemonGenderText->GetText().ToString());
    CHECK_EQUAL(ESlateVisibility::Hidden, PokemonStatusIcon->GetVisibility());

    auto Pokemon3 = UConstructionUtilities::CreateForeignPokemon({
        .Species = "VOLTORB",
        .Level = 10,
        .Nickname = FText::FromStringView(TEXT("Volty"))
    }, ForeignTrainer);
    Page->Refresh(Pokemon3);
    
    CHECK_EQUAL(TEXT("Volty"), PokemonNameText->GetText().ToString());
    CHECK_EQUAL(TEXT("10"), PokemonLevelText->GetText().ToString());
    CHECK_EQUAL(TEXT(""), PokemonGenderText->GetText().ToString());
    CHECK_EQUAL(ESlateVisibility::Hidden, PokemonStatusIcon->GetVisibility());
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PokemonSummaryPagesTest_PokemonInfo, "Unit Tests.UI.Summary.Components.PokemonSummaryPagesTest.PokemonInfo",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool PokemonSummaryPagesTest_PokemonInfo::RunTest(const FString &Parameters) {
    auto [DudOverlay, World] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UPokemonInfoPage>();
    ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    auto Page = CreateWidget<UPokemonInfoPage>(World, WidgetClass);
    Page->AddToViewport();

    auto ForeignTrainer = NewObject<UBasicTrainer>()->Initialize(TEXT("LASS"), FText::FromStringView(TEXT("Amy")));
    auto Pokemon = UConstructionUtilities::CreateForeignPokemon({
        .Species = "KABUTOPS"
    }, ForeignTrainer);

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
    
    return true;
}