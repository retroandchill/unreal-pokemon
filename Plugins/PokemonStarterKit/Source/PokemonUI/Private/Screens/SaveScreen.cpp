// "Unreal Pokémon" created by Retro & Chill.


#include "Screens/SaveScreen.h"
#include "Components/DisplayText.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/PokemonSubsystem.h"
#include "Saving/PokemonSaveGame.h"
#include "Settings/PokemonSaveGameSettings.h"

void USaveScreen::NativeConstruct() {
    auto &Settings = *GetDefault<UPokemonSaveGameSettings>();
    UGameplayStatics::AsyncLoadGameFromSlot(Settings.PrimarySaveSlotName, Settings.PrimarySaveIndex,
        FAsyncLoadGameFromSlotDelegate::CreateWeakLambda(this, [this](const FString&, int32, USaveGame* SaveGameIn) {
            auto PokemonSaveGame = Cast<UPokemonSaveGame>(SaveGameIn);
            SetSaveGame(PokemonSaveGame);
            PromptToSaveGame();
        }));
}

void USaveScreen::SetSaveGame(UPokemonSaveGame* SaveGame) {
    CurrentSaveGame = SaveGame;
    if (SaveGame != nullptr) {
        LastSavedText->SetText(FText::FormatNamed(LastSavedFormat,
            TEXT("Date"), FText::FromString(SaveGame->SaveDate.ToFormattedString(*DateFormat)),
            TEXT("Time"), FText::FromString(SaveGame->SaveDate.ToFormattedString(TEXT("%H:%M")))));
    } else {
        LastSavedText->SetText(FText::GetEmpty());
    }
}

void USaveScreen::SaveGame(FOnSaveComplete &&OnComplete) {
    auto &Settings = *GetDefault<UPokemonSaveGameSettings>();
    auto SaveGame = UPokemonSubsystem::GetInstance(this).CreateSaveGame(Settings.SaveGameClass.TryLoadClass<UPokemonSaveGame>());
    AddCustomSaveProperties(SaveGame);
    UGameplayStatics::AsyncSaveGameToSlot(SaveGame, Settings.PrimarySaveSlotName, Settings.PrimarySaveIndex,
        FAsyncSaveGameToSlotDelegate::CreateWeakLambda(this, [this, SaveGame, Callback = MoveTemp(OnComplete)](const FString&, const int32, bool bSuccess) {

            if (bSuccess) {
                SetSaveGame(SaveGame);
            }
            Callback.Execute(bSuccess);
        }));
}