// "Unreal Pokémon" created by Retro & Chill.

#include "Components/PokemonIconPreview.h"
#include "Algo/ForEach.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"
#include "Graphics/GraphicsLoadingSubsystem.h"
#include "Pokemon/Pokemon.h"
#include "Utilities/WidgetUtilities.h"
#include <functional>

void UPokemonIconPreview::Refresh_Implementation(const TScriptInterface<IPokemon> &Pokemon) {
    Super::Refresh_Implementation(Pokemon);
    auto GraphicsLoadingSubsystem = GetGameInstance()->GetSubsystem<UGraphicsLoadingSubsystem>();
    UWidgetUtilities::SetBrushFromAsset(PokemonIcon, GraphicsLoadingSubsystem->GetPokemonIcon(Pokemon, this));
    auto IconGraphics = GraphicsLoadingSubsystem->GetTypeIconGraphics(Pokemon->GetTypes());
    Algo::ForEach(TypeIcons, &UWidget::RemoveFromParent);
    TypeIcons.Empty();
    Algo::ForEach(IconGraphics, std::bind_front(&UPokemonIconPreview::CreateTypeIcon, this));
}

void UPokemonIconPreview::CreateTypeIcon(UObject *Resource) {
    auto TypeIcon = WidgetTree->ConstructWidget<UImage>();
    UWidgetUtilities::SetBrushFromAsset(TypeIcon, Resource, true);
    SlotTypeIcon(TypeIcon);
    TypeIcons.Emplace(TypeIcon);
}