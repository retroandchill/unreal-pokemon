// "Unreal Pokémon" created by Retro & Chill.

#include "Screens/TrainerCardScreen.h"

#include "Components/Image.h"
#include "Managers/PokemonSubsystem.h"
#include "Player/PlayerMetadata.h"
#include "Primatives/DisplayText.h"
#include "Utilities/GraphicsLoadingSubsystem.h"

void UTrainerCardScreen::NativeConstruct() {
    Super::NativeConstruct();

    auto PokemonSubsystem = GetGameInstance()->GetSubsystem<UPokemonSubsystem>();
    check(PokemonSubsystem != nullptr);
    Trainer = PokemonSubsystem->GetPlayer();
    PlayerMetadata = PokemonSubsystem->GetPlayerMetadata();
    PlayerMetadata->GetOnTimeUpdated().AddDynamic(this, &UTrainerCardScreen::SetPlayerTimeInfo);

    SetTrainerSprite();
    SetTrainerInfo();
}

void UTrainerCardScreen::SetTrainerSprite() {
    check(TrainerImage != nullptr)

        auto GraphicsLoadingSubsystem = GetGameInstance()->GetSubsystem<UGraphicsLoadingSubsystem>();
    check(GraphicsLoadingSubsystem != nullptr) auto [Material, Size] =
        GraphicsLoadingSubsystem->GetTrainerSprite(*Trainer, this);
    TrainerImage->SetBrushFromMaterial(Material);
    TrainerImage->SetDesiredSizeOverride(Size);
}

void UTrainerCardScreen::SetTrainerInfo() {
    TrainerNameText->SetText(Trainer->GetTrainerName());
    IDText->SetText(FText::FromString(FString::FromInt(Trainer->GetIdNumber())));
    SetPlayerTimeInfo(PlayerMetadata->GetTotalPlaytime());
    AdventureStartedText->SetText(FText::AsDate(PlayerMetadata->GetStartDate()));

    // TODO: Pokédex, Money, and Hall of Fame
}

void UTrainerCardScreen::SetPlayerTimeInfo(float Playtime) {
    int32 Hours = FMath::FloorToInt32(Playtime / 3600);
    TimeText->SetText(FText::FromString(FTimespan::FromSeconds(Playtime).ToString(TEXT("%h:%m")).RightChop(1)));
}
