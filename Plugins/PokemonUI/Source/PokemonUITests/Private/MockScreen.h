#pragma once

#include "Screens/PartyScreen.h"

class FMockScreen : public IPartyScreen {
  public:
    FMockScreen() = default;

    explicit FMockScreen(APlayerController *PlayerController) : PlayerController(PlayerController) {
    }

    void BeginSwitch(int32 Index) override {
        SwitchIndex.Emplace(Index);
    }

    void SetCommandHelpText(const FText &Text) override {
    }

    APlayerController &GetPlayerController() override {
        return *PlayerController;
    }

    FOnPokemonSelected &GetOnPokemonSelect() override {
        return OnPokemonSelected;
    }

    void RemoveFromStack() override {
    }

    virtual void RefreshScene() override {
    }

    TOptional<int32> SwitchIndex;

    APlayerController *PlayerController = nullptr;

    FOnPokemonSelected OnPokemonSelected;
};