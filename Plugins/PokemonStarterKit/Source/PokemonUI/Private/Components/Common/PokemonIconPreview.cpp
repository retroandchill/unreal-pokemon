// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Common/PokemonIconPreview.h"
#include "Algo/ForEach.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"
#include "Graphics/AssetClasses.h"
#include "Graphics/SpriteLoader.h"
#include "PaperFlipbookUserWidget.h"
#include "Pokemon/Pokemon.h"
#include "Utilities/WidgetUtilities.h"

void UPokemonIconPreview::Refresh_Implementation(const TScriptInterface<IPokemon> &Pokemon) {
    Super::Refresh_Implementation(Pokemon);
    PokemonIcon->SetFlipbook(USpriteLoader::GetPokemonIcon(Pokemon));
    auto IconGraphics = Pokemon::Assets::Graphics::TypeIcons.LoadAssets(Pokemon->GetTypes());
    Algo::ForEach(TypeIcons, &UWidget::RemoveFromParent);
    TypeIcons.Empty();
    // clang-format off
    IconGraphics |
        UE::Ranges::Map([](TOptional<UObject&> Optional) { return Optional.GetPtrOrNull(); }) |
        UE::Ranges::ForEach(this, &UPokemonIconPreview::CreateTypeIcon);
    // clang-format on
}

void UPokemonIconPreview::CreateTypeIcon(UObject *Resource) {
    auto TypeIcon = WidgetTree->ConstructWidget<UImage>();
    UWidgetUtilities::SetBrushFromAsset(TypeIcon, Resource, true);
    SlotTypeIcon(TypeIcon);
    TypeIcons.Emplace(TypeIcon);
}