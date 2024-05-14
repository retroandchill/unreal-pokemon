// "Unreal Pokémon" created by Retro & Chill.

#if WITH_TESTS && HAS_AUTOMATION_HELPERS
#include "Handlers/PartyMenu/PartySwitchHandler.h"
#include "Asserts.h"
#include "Handlers/PartyMenu/PartyMenuHandler.h"
#include "Managers/PokemonSubsystem.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/GamePokemon.h"
#include "Screens/PartyScreen.h"
#include "Trainers/BasicTrainer.h"
#include "Utilities/GCPointer.h"
#include "Utilities/RAII.h"

class FMockScreen : public IPartyScreen {
  public:
    void BeginSwitch(int32 Index) override {
        SwitchIndex.Emplace(Index);
    }

    void SetHelpText(const FText &Text) override {
    }

    ARPGPlayerController &GetPlayerController() override {
        return *PlayerController;
    }

    TOptional<int32> SwitchIndex;

    ARPGPlayerController *PlayerController = nullptr;
};

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartySwitchHandlerTest, "Unit Tests.UI.PartySwitchHandlerTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool PartySwitchHandlerTest::RunTest(const FString &Parameters) {
    FGameInstancePtr GameInstance;
    if (!UPokemonSubsystem::Exists()) {
        GameInstance.Reset(NewObject<UGameInstance>());
        GameInstance->Init();
    }

    FMockScreen Screen;

    auto Trainer = NewObject<UBasicTrainer>()->Initialize(TEXT("POKEMONRANGER_M"), FText::FromStringView(TEXT("Test")));
    Trainer->AddPokemonToParty(UGamePokemon::Create({.Species = TEXT("RIOLU"), .Level = 5}));

    TGCPointer<UPartyMenuHandler> Handler(NewObject<UPartySwitchHandler>());
    CHECK_FALSE(Handler->ShouldShow(Screen, *Trainer, 0));

    Trainer->AddPokemonToParty(UGamePokemon::Create({.Species = TEXT("OSHAWOTT"), .Level = 5}));
    CHECK_TRUE(Handler->ShouldShow(Screen, *Trainer, 0));

    Handler->Handle(Screen, *Trainer, 0);
    ASSERT_TRUE(Screen.SwitchIndex.IsSet());
    CHECK_EQUAL(0, Screen.SwitchIndex.GetValue());

    return true;
}
#endif