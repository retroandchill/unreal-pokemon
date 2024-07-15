// "Unreal Pokémon" created by Retro & Chill.


#include "Info/TrainerCardWidgetBase.h"
#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "Graphics/GraphicsLoadingSubsystem.h"
#include "Managers/PokemonSubsystem.h"
#include "Player/PlayerMetadata.h"
#include "Utilities/PokemonUIUtils.h"

class UGraphicsLoadingSubsystem;
class UPokemonSubsystem;

void UTrainerCardWidgetBase::NativeConstruct() {
    Super::NativeConstruct();

    auto PokemonSubsystem = GetGameInstance()->GetSubsystem<UPokemonSubsystem>();
    check(PokemonSubsystem != nullptr)
    Trainer = PokemonSubsystem->GetPlayer();
    PlayerMetadata = PokemonSubsystem->GetPlayerMetadata();
    PlayerMetadata->GetOnTimeUpdated().AddUniqueDynamic(this, &UTrainerCardWidgetBase::SetPlayerTimeInfo);

    SetTrainerSprite();
    SetTrainerInfo();
}

void UTrainerCardWidgetBase::SetTrainerSprite() {
    check(TrainerImage != nullptr)

    auto GraphicsLoadingSubsystem = GetGameInstance()->GetSubsystem<UGraphicsLoadingSubsystem>();
    check(GraphicsLoadingSubsystem != nullptr)
    auto [Material, Size] = GraphicsLoadingSubsystem->GetTrainerSprite(Trainer, this);
    TrainerImage->SetBrushFromMaterial(Material);
    TrainerImage->SetDesiredSizeOverride(Size);
}

void UTrainerCardWidgetBase::SetTrainerInfo() {
    TrainerNameText->SetText(Trainer->GetTrainerName());
    IDText->SetText(FText::FromString(UPokemonUIUtils::ZeroPad(Trainer->GetIdNumber(), IdNumberLength)));
    SetPlayerTimeInfo(PlayerMetadata->GetTotalPlaytime());
    AdventureStartedText->SetText(FText::AsDate(PlayerMetadata->GetStartDate()));

    // TODO: Pokédex, Money, and Hall of Fame
}

void UTrainerCardWidgetBase::SetPlayerTimeInfo(float Playtime) {
    TimeText->SetText(FText::FromString(FTimespan::FromSeconds(Playtime).ToString(TEXT("%h:%m")).RightChop(1)));
}