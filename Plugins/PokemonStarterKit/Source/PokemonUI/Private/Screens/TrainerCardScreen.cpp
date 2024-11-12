// "Unreal Pokémon" created by Retro & Chill.

#include "Screens/TrainerCardScreen.h"
#include "Managers/PokemonSubsystem.h"
#include "PaperFlipbookUserWidget.h"
#include "Components/Common/PlayerInfoWidget.h"
#include "Utilities/RPGMenuUtilities.h"

DEFINE_INJECTABLE_DEPENDENCY(UTrainerCardScreen)

UTrainerCardScreen *UTrainerCardScreen::AddTrainerCardScreenToStack(const UObject *WorldContextObject) {
    return URPGMenuUtilities::InjectScreenToStack<UTrainerCardScreen>(WorldContextObject).GetPtrOrNull();
}

void UTrainerCardScreen::NativeConstruct() {
    Super::NativeConstruct();

    auto PokemonSubsystem = GetGameInstance()->GetSubsystem<UPokemonSubsystem>();
    check(PokemonSubsystem != nullptr)
    PlayerTrainer = PokemonSubsystem->GetPlayer();
    PlayerMetadata = PokemonSubsystem->GetPlayerMetadata();
    OnPlayerInfoSet(PlayerTrainer, PlayerMetadata);
}

/*
void UTrainerCardScreen::SetTrainerSprite() {
    check(TrainerImage != nullptr)
    auto Flipbook = USpriteLoader::GetTrainerSprite(Trainer).TryGet<UPaperFlipbook>().GetPtrOrNull();
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
*/