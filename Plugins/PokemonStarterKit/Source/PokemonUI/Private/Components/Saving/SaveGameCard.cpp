// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Saving/SaveGameCard.h"
#include "Algo/ForEach.h"
#include "Blueprint/WidgetTree.h"
#include "Components/DisplayText.h"
#include "Graphics/SpriteLoader.h"
#include "Managers/PokemonSubsystem.h"
#include "PaperFlipbookUserWidget.h"
#include "Player/PlayerMetadata.h"
#include "Ranges/Algorithm/ToArray.h"
#include "Ranges/Views/Map.h"

void USaveGameCard::NativeConstruct() {
    Super::NativeConstruct();

    UpdateTimeLabels();

    auto &Subsystem = UPokemonSubsystem::GetInstance(this);
    auto PlayerMetadata = Subsystem.GetPlayerMetadata();
    PlayerMetadata->GetOnTimeUpdated().AddUniqueDynamic(this, &USaveGameCard::UpdatePlaytimeText);
    UpdateTimeLabels();

    LocationText->SetText(Subsystem.GetCurrentLocation());

    Algo::ForEach(Icons, &UWidget::RemoveFromParent);
    // clang-format off
    Icons = Subsystem.GetPlayer()->GetParty() |
            UE::Ranges::Map(this, &USaveGameCard::CreatePokemonIcon) |
            UE::Ranges::ToArray;
    // clang-format on

    // TODO: Badges and Pokédex info when it's ready

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

UPaperFlipbookUserWidget *USaveGameCard::CreatePokemonIcon(const TScriptInterface<IPokemon> &Pokemon) {
    auto Image = WidgetTree->ConstructWidget<UPaperFlipbookUserWidget>();
    auto Flipbook = USpriteLoader::GetPokemonIcon(Pokemon);
    Image->SetFlipbook(Flipbook);
    Image->SetPlayRate(0.f);
    SlotPokemonIcon(Image);
    return Image;
}

void USaveGameCard::UpdatePlaytimeText(float Playtime) {
    PlaytimeText->SetText(FText::FromString(FTimespan::FromSeconds(Playtime).ToString(TEXT("%h:%m")).RightChop(1)));
}