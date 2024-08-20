// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Saving/SaveGameCard.h"
#include "Algo/ForEach.h"
#include "Blueprint/WidgetTree.h"
#include "Components/DisplayText.h"
#include "Components/Image.h"
#include "Components/Party/PokemonSelectionPane.h"
#include "Graphics/GraphicsLoadingSubsystem.h"
#include "Managers/PokemonSubsystem.h"
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
    Icons = Subsystem.GetPlayer()->GetParty() |
            UE::Ranges::Map(this, &USaveGameCard::CreatePokemonIcon) | UE::Ranges::ToArray;

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

UImage *USaveGameCard::CreatePokemonIcon(const TScriptInterface<IPokemon> &Pokemon) {
    auto Image = WidgetTree->ConstructWidget<UImage>();
    auto Subsystem = GetGameInstance()->GetSubsystem<UGraphicsLoadingSubsystem>();
    auto [Material, Size] = Subsystem->GetPokemonIcon(Pokemon, this);
    Material->SetScalarParameterValue("FrameRate", 0.f);
    Image->SetBrushFromMaterial(Material);
    Image->SetDesiredSizeOverride(Size);
    SlotPokemonIcon(Image);
    return Image;
}

void USaveGameCard::UpdatePlaytimeText(float Playtime) {
    PlaytimeText->SetText(FText::FromString(FTimespan::FromSeconds(Playtime).ToString(TEXT("%h:%m")).RightChop(1)));
}