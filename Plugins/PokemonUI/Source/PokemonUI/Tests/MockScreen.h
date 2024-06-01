#pragma once

#if WITH_TESTS && HAS_AUTOMATION_HELPERS
#include "RPGPlayerController.h"
#include "Screens/PartyScreen.h"

class FMockScreen : public IPartyScreen {
  public:
    FMockScreen() = default;

    explicit FMockScreen(APlayerController *PlayerController) : PlayerController(PlayerController) {
    }

    void BeginSwitch(int32 Index) override {
        SwitchIndex.Emplace(Index);
    }

    void SetHelpText(const FText &Text) override {
    }

    APlayerController &GetPlayerController() override {
        return *PlayerController;
    }

    TOptional<int32> SwitchIndex;

    APlayerController *PlayerController = nullptr;
};
#endif