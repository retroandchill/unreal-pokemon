#pragma once

#if WITH_TESTS && HAS_AUTOMATION_HELPERS
#include "RPGPlayerController.h"
#include "Screens/PartyScreen.h"

class FMockScreen : public IPartyScreen {
public:
    FMockScreen() = default;

    explicit FMockScreen(ARPGPlayerController *PlayerController) : PlayerController(PlayerController) {
        
    }
    
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
#endif