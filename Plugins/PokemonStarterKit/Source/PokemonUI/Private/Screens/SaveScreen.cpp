// "Unreal Pokémon" created by Retro & Chill.

#include "Screens/SaveScreen.h"
#include "Components/DisplayText.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/PokemonSubsystem.h"
#include "Saving/PokemonSaveGame.h"
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
            auto PokemonSaveGame = Cast<UPokemonSaveGame>(SaveGameIn);
            SetSaveGame(PokemonSaveGame);
            PromptToSaveGame();
        }));
}

void USaveScreen::NativeTick(const FGeometry &MyGeometry, float InDeltaTime) {
    Super::NativeTick(MyGeometry, InDeltaTime);
    auto &Settings = *GetDefault<UPokemonSaveGameSettings>();
    if (!SaveGameCreationFuture.IsSet() || !SaveGameCreationFuture->IsReady()) {
        return;
    }

    auto SaveGame = SaveGameCreationFuture->Consume();
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

void USaveScreen::SetSaveGame(UPokemonSaveGame *SaveGame) {
    CurrentSaveGame = SaveGame;
    if (SaveGame != nullptr) {
        LastSavedText->SetText(FText::FormatNamed(
            LastSavedFormat, TEXT("Date"), FText::FromString(SaveGame->SaveDate.ToFormattedString(*DateFormat)),
            TEXT("Time"), FText::FromString(SaveGame->SaveDate.ToFormattedString(TEXT("%H:%M")))));
    } else {
        LastSavedText->SetText(FText::GetEmpty());
    }
}

void USaveScreen::SaveGame(FOnSaveComplete &&OnComplete) {
    check(!SaveGameCreationFuture.IsSet())
    OnSaveCompleteDelegate = std::move(OnComplete);
    SaveGameCreationFuture.Emplace(AsyncThread([this] {
        auto &Settings = *GetDefault<UPokemonSaveGameSettings>();
        auto SaveGame = UPokemonSubsystem::GetInstance(this).CreateSaveGame(
            Settings.SaveGameClass.TryLoadClass<UPokemonSaveGame>());
        AddCustomSaveProperties(SaveGame);
        return SaveGame;
    }));
}

void USaveScreen::ExitSaveScreen(bool bSuccess) {
    CloseScreen();
    OnExitSaveScreen.Broadcast(bSuccess);
    OnExitSaveScreen.Clear();
}