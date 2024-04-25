
#include "Primatives/SelectableContainer.h"
#include "Utilities/RAII.h"
#if WITH_TESTS && HAS_AUTOMATION_HELPERS
#include "AssetRegistry/AssetRegistryModule.h"
#include "Misc/AutomationTest.h"
#include "Asserts.h"
#include "Blueprint/UserWidget.h"
#include "Windows/SelectableWidget.h"
#include "Components/Button.h"
#include "Misc/OutputDeviceNull.h"
#include "Tests/AutomationEditorCommon.h"
#include "Utilities/WidgetChildUtilities.h"
#include "Primatives/SelectableOption.h"
#include "External/accessor.hpp"

using namespace accessor;

FUNCTION_ACCESSOR(UserWidgetConstruct, UUserWidget, NativeConstruct, void);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(SelectableWidgetMouseTest, "UnrealPokemon.RPGMenus.SelectableWidgetMouseTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool SelectableWidgetMouseTest::RunTest(const FString &Parameters) {
    auto WidgetBlueprintAsset = FAssetRegistryModule::GetRegistry()
        .GetAssetByObjectPath(FSoftObjectPath(TEXT("/RPGMenus/Tests/Resources/OptionTestWidget.OptionTestWidget")));
    auto WidgetBlueprint = Cast<UBlueprint>(WidgetBlueprintAsset.GetAsset());
    ASSERT_NOT_NULL(WidgetBlueprint);
    
    auto World = FAutomationEditorCommonUtils::CreateNewMap();
    auto WidgetClass = WidgetBlueprint->GeneratedClass.Get();
    ASSERT_EQUAL(USelectableWidget::StaticClass(), WidgetClass->GetSuperClass());
    auto NewWidget = CreateWidget<USelectableWidget>(World, WidgetClass);
    ASSERT_NOT_NULL(NewWidget);
    callFunction<UserWidgetConstruct>(*NewWidget);
    
    auto Options = ISelectableContainer::Execute_GetOptions(NewWidget);
    ASSERT_EQUAL(3, Options.Num());
    for (auto Option : Options) {
        callFunction<UserWidgetConstruct>(*Option);
    }

    auto Button1 = Cast<UButton>(UWidgetChildUtilities::FindChildWidget(Options[1], TEXT("PrimaryButton")));
    ASSERT_NOT_NULL(Button1);
    Button1->OnHovered.Broadcast();
    ASSERT_EQUAL(-1, NewWidget->GetIndex());

    NewWidget->SetActive(true);
    Button1->OnHovered.Broadcast();
    ASSERT_EQUAL(1, NewWidget->GetIndex());

    auto Button2 = Cast<UButton>(UWidgetChildUtilities::FindChildWidget(Options[2], TEXT("PrimaryButton")));
    ASSERT_NOT_NULL(Button2);

    AddExpectedError(FString::Format(TEXT("Confirm handled on index {0}"), FStringFormatOrderedArguments({2})));
    NewWidget->SetActive(false);
    Button2->OnClicked.Broadcast();

    NewWidget->SetActive(true);
    Button2->OnClicked.Broadcast();
    
    return true;
}
#endif