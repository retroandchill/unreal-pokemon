#include "Asserts.h"
#include "Lookup/InjectionUtilities.h"
#include "Managers/PokemonSubsystem.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/GamePokemon.h"
#include "Settings/PokemonStorageSystemSettings.h"
#include "Storage/DefaultStorageBox.h"
#include "Storage/StorageBox.h"
#include "Storage/StorageSystem.h"
#include "Storage/StorageUtilities.h"
#include "Utilities/WidgetTestUtilities.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestStorageBox, "Unit Tests.Storage.TestStorageBox",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestStorageBox::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();

    UPokemonSubsystem::GetInstance(World.Get()).StartNewGame();
    auto Player = UPokemonSubsystem::GetInstance(World.Get()).GetPlayer();

    auto &StorageSystem = UPokemonSubsystem::GetInstance(World.Get()).GetStorageSystem();
    UE_CHECK_NOT_NULL(StorageSystem.GetObject());

    auto Settings = GetDefault<UPokemonStorageSystemSettings>();
    UE_ASSERT_EQUAL(Settings->TotalBoxes, StorageSystem->GetBoxCount());
    UE_CHECK_EQUAL(0, StorageSystem->GetCurrentBoxIndex());
    StorageSystem->SetCurrentBoxIndex(1);
    UE_CHECK_EQUAL(1, StorageSystem->GetCurrentBoxIndex());
    for (int32 i = 0; i < StorageSystem->GetBoxCount(); i++) {
        UE_CHECK_EQUAL(Settings->BoxCapacity, StorageSystem->GetBox(i)->GetCapacity());

        auto BoxName = FText::FormatNamed(Settings->BoxNameFormat, TEXT("Index"), i + 1);
        UE_CHECK_EQUAL(BoxName.ToString(), StorageSystem->GetBox(i)->GetDisplayName().ToString());
    }

    auto Box = StorageSystem->GetBox(0);
    Box->SetDisplayName(FText::FromStringView(TEXT("Test Box")));
    UE_CHECK_EQUAL(TEXT("Test Box"), Box->GetDisplayName().ToString());

    auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(), FPokemonDTO{.Species = TEXT("PIKACHU"), .Level = 20}, Player);
    int32 DepositIndex = INDEX_NONE;
    UE_ASSERT_EQUAL(EDepositResult::Deposited, UStorageUtilities::DepositToBox(Box, Pokemon1, DepositIndex));
    UE_CHECK_EQUAL(0, DepositIndex);

    for (int32 i = 1; i < 30; i++) {
        auto FillerPokemon = UnrealInjector::NewInjectedDependency<IPokemon>(
            World.Get(), FPokemonDTO{.Species = TEXT("MAGIKARP"), .Level = 5}, Player);
        DepositIndex = INDEX_NONE;
        UE_ASSERT_EQUAL(EDepositResult::Deposited, UStorageUtilities::DepositToBox(Box, FillerPokemon, DepositIndex));
        UE_CHECK_EQUAL(i, DepositIndex);
    }

    UE_CHECK_TRUE(Box->IsBoxFull());

    auto Pokemon2 = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(), FPokemonDTO{.Species = TEXT("RIOLU"), .Level = 20}, Player);
    DepositIndex = INDEX_NONE;
    UE_ASSERT_EQUAL(EDepositResult::BoxFull, UStorageUtilities::DepositToBox(Box, Pokemon2, DepositIndex));
    UE_CHECK_EQUAL(INDEX_NONE, DepositIndex);

    TScriptInterface<IPokemon> Withdrawn;
    UE_ASSERT_EQUAL(EWithdrawResult::PokemonPresent, UStorageUtilities::WithdrawFromBox(Box, 0, Withdrawn));
    UE_CHECK_TRUE(Withdrawn == Pokemon1);

    Withdrawn = nullptr;
    UE_ASSERT_EQUAL(EWithdrawResult::EmptySlot, UStorageUtilities::WithdrawFromBox(Box, 0, Withdrawn));
    UE_CHECK_TRUE(Withdrawn == nullptr);

    Withdrawn = nullptr;
    UE_ASSERT_EQUAL(EWithdrawResult::EmptySlot, UStorageUtilities::SwapWithPokemon(Box, 0, Pokemon1, Withdrawn));
    UE_CHECK_TRUE(Withdrawn == nullptr);

    Withdrawn = nullptr;
    UE_ASSERT_EQUAL(EWithdrawResult::PokemonPresent, UStorageUtilities::SwapWithPokemon(Box, 0, Pokemon2, Withdrawn));
    UE_CHECK_TRUE(Withdrawn == Pokemon1);

    return true;
}