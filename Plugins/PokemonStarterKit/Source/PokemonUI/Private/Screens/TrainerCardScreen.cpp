// "Unreal Pokémon" created by Retro & Chill.

#include "Screens/TrainerCardScreen.h"
#include "Components/DisplayText.h"
#include "Graphics/SpriteLoader.h"
#include "Managers/PokemonSubsystem.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookUserWidget.h"
#include "Player/PlayerMetadata.h"
#include "Utilities/PokemonUIUtils.h"

void UTrainerCardScreen::NativeConstruct() {
    Super::NativeConstruct();

    auto PokemonSubsystem = GetGameInstance()->GetSubsystem<UPokemonSubsystem>();
    check(PokemonSubsystem != nullptr)
    Trainer = PokemonSubsystem->GetPlayer();
    PlayerMetadata = PokemonSubsystem->GetPlayerMetadata();
    PlayerMetadata->GetOnTimeUpdated().AddUniqueDynamic(this, &UTrainerCardScreen::SetPlayerTimeInfo);

    SetTrainerSprite();
    SetTrainerInfo();
}

void UTrainerCardScreen::SetTrainerSprite() {
    check(TrainerImage != nullptr)
    auto Flipbook = USpriteLoader::GetTrainerSprite(Trainer);
    TrainerImage->SetFlipbook(Flipbook);
    if (Flipbook != nullptr) {
        TrainerImage->SetPlaybackPositionInFrames(Flipbook->GetNumFrames());
    }
}

void UTrainerCardScreen::SetTrainerInfo() {
    TrainerNameText->SetText(Trainer->GetTrainerName());
    IDText->SetText(FText::FromString(UPokemonUIUtils::ZeroPad(Trainer->GetIdNumber(), IdNumberLength)));
    SetPlayerTimeInfo(PlayerMetadata->GetTotalPlaytime());
    AdventureStartedText->SetText(FText::AsDate(PlayerMetadata->GetStartDate()));

    // TODO: Pokédex, Money, and Hall of Fame
}

void UTrainerCardScreen::SetPlayerTimeInfo(float Playtime) {
    TimeText->SetText(FText::FromString(FTimespan::FromSeconds(Playtime).ToString(TEXT("%h:%m")).RightChop(1)));
}
