// "Unreal Pokémon" created by Retro & Chill.

#include "Screens/SaveScreen.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/PokemonSubsystem.h"
#include "PokemonUI.h"
#include "Saving/EnhancedSaveGameSubsystem.h"
#include "Saving/PokemonSaveGame.h"
#include "Saving/Serialization/EnhancedSaveGame.h"
#include "Settings/PokemonSaveGameSettings.h"
#include "Utilities/RPGMenuUtilities.h"

DEFINE_INJECTABLE_DEPENDENCY(USaveScreen)

USaveScreen *USaveScreen::AddSaveScreenToStack(const UObject *WorldContextObject) {
    return URPGMenuUtilities::InjectScreenToStack<USaveScreen>(WorldContextObject).GetPtrOrNull();
}

void USaveScreen::NativeOnActivated() {
    Super::NativeOnActivated();
    auto &Settings = *GetDefault<UPokemonSaveGameSettings>();
    UGameplayStatics::AsyncLoadGameFromSlot(
        Settings.PrimarySaveSlotName, Settings.PrimarySaveIndex,
        FAsyncLoadGameFromSlotDelegate::CreateWeakLambda(this, [this](const FString &, int32, USaveGame *SaveGameIn) {
            auto EnhancedSaveGame = Cast<UEnhancedSaveGame>(SaveGameIn);
            SetSaveGame(EnhancedSaveGame);
            PromptToSaveGame();
        }));
}

void USaveScreen::SetSaveGame(UEnhancedSaveGame *SaveGame) {
    CurrentSaveGame = SaveGame;
    if (SaveGame != nullptr) {
        auto PokemonSaveData = SaveGame->LoadObjectFromSaveGame<UPokemonSaveGame>(Pokemon::Saving::PokemonCoreSaveData);
        check(PokemonSaveData.IsSet())
        OnSaveGameSet(PokemonSaveData.GetPtrOrNull());
    } else {
        OnFirstSaveAttempt();
    }
}

void USaveScreen::SaveGame(FOnSaveComplete &&OnComplete) {
    UE_LOG(LogPokemonUI, Display, TEXT("Creating the save game!"));
    check(!SaveGameCreationFuture.IsSet())
    OnSaveCompleteDelegate = std::move(OnComplete);
    SaveGameCreationFuture.Emplace(AsyncThread([this] {
        auto SaveGame = UEnhancedSaveGameSubsystem::Get(this).CreateSaveGame();
        return SaveGame;
    }));
    SaveGameCreationFuture->AddOnCompleteTask(this, &USaveScreen::CommitSaveGame);
}

void USaveScreen::ExitSaveScreen(bool bSuccess) {
    CloseScreen();
    OnExitSaveScreen.Broadcast(bSuccess);
    OnExitSaveScreen.Clear();
}

void USaveScreen::CommitSaveGame(UEnhancedSaveGame *SaveGame) {
    UE_LOG(LogPokemonUI, Display, TEXT("Saving game!"));
    auto &Settings = *GetDefault<UPokemonSaveGameSettings>();
    UGameplayStatics::AsyncSaveGameToSlot(SaveGame, Settings.PrimarySaveSlotName, Settings.PrimarySaveIndex,
                                          FAsyncSaveGameToSlotDelegate::CreateWeakLambda(
                                              this, [this, SaveGame](const FString &, const int32, bool bSuccess) {
                                                  if (bSuccess) {
                                                      SetSaveGame(SaveGame);
                                                  }
                                                  OnSaveCompleteDelegate.ExecuteIfBound(bSuccess);
                                                  OnSaveCompleteDelegate.Unbind();
                                              }));
    SaveGameCreationFuture.Reset();
}