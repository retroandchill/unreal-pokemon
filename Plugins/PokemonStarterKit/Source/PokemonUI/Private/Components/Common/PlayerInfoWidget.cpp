// "Unreal Pokémon" created by Retro & Chill.


#include "Components/Common/PlayerInfoWidget.h"
#include "Player/PlayerMetadata.h"

void UPlayerInfoWidget::SetPlayerInfo(const TScriptInterface<ITrainer> &NewTrainer, UPlayerMetadata *NewPlayerMetadata) {
    PlayerTrainer = NewTrainer;
    PlayerMetadata = NewPlayerMetadata;
    HandlePlayerInfoSet(NewTrainer, NewPlayerMetadata);
}

void UPlayerInfoWidget::HandlePlayerInfoSet(const TScriptInterface<ITrainer> &NewTrainer, UPlayerMetadata *NewPlayerMetadata) {
    PlayerMetadata->GetOnTimeUpdated().AddUniqueDynamic(this, &UPlayerInfoWidget::SetPlayerTimeInfo);
    OnPlayerInfoSet(NewTrainer, NewPlayerMetadata);
    SetPlayerTimeInfo(NewPlayerMetadata->GetTotalPlaytime());
}