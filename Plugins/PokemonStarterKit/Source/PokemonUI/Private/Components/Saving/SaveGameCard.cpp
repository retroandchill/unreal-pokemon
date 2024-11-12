// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Saving/SaveGameCard.h"
#include "Blueprint/WidgetTree.h"
#include "CommonTextBlock.h"
#include "Managers/PokemonSubsystem.h"
#include "Player/PlayerMetadata.h"

void USaveGameCard::NativeConstruct() {
    Super::NativeConstruct();

    UpdateTimeLabels();

    auto &Subsystem = UPokemonSubsystem::GetInstance(this);
    auto PlayerMetadata = Subsystem.GetPlayerMetadata();
    PlayerMetadata->GetOnTimeUpdated().AddUniqueDynamic(this, &USaveGameCard::UpdatePlaytimeText);
    UpdateTimeLabels();
    SetContents();

    auto Playtime = PlayerMetadata->GetTotalPlaytime();
    UpdatePlaytimeText(Playtime);
}

void USaveGameCard::UpdateTimeLabels() {
    auto Now = FDateTime::Now();
    DateText->SetText(FText::FromString(Now.ToFormattedString(*DateFormat)));
    TimeText->SetText(FText::FromString(Now.ToFormattedString(TEXT("%H:%M"))));

    auto Seconds = static_cast<float>(60 - Now.GetSecond());
    auto &TimerManager = GetGameInstance()->GetTimerManager();
    TimerManager.SetTimer(TimeUpdateTimer, FTimerDelegate::CreateUObject(this, &USaveGameCard::UpdateTimeLabels),
                          Seconds, false);
}

void USaveGameCard::UpdatePlaytimeText(float Playtime) {
    PlaytimeText->SetText(FText::FromString(FTimespan::FromSeconds(Playtime).ToString(TEXT("%h:%m")).RightChop(1)));
}