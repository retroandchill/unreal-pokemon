// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Common/PokemonIconPreview.h"
#include "Algo/ForEach.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"
#include "Graphics/AssetClasses.h"
#include "Graphics/SpriteLoader.h"
#include "PaperFlipbookUserWidget.h"
#include "Pokemon/Pokemon.h"
#include "Ranges/Optional/GetPtrOrNull.h"
#include "Utilities/WidgetUtilities.h"

void UPokemonIconPreview::Refresh_Implementation(const TScriptInterface<IPokemon> &NewPokemon) {
    Super::Refresh_Implementation(NewPokemon);
    PokemonIcon->SetFlipbook(USpriteLoader::GetPokemonIcon(NewPokemon).TryGet<UPaperFlipbook>().GetPtrOrNull());
    auto IconGraphics = Pokemon::Assets::Graphics::TypeIcons.LoadAssets(NewPokemon->GetTypes());
    Algo::ForEach(TypeIcons, &UWidget::RemoveFromParent);
    TypeIcons.Empty();
    // clang-format off
    IconGraphics |
        UE::Ranges::Map([](const TOptional<FImageAsset> &Optional) {
            return Optional |
                UE::Optionals::FlatMap([](const FImageAsset& Opt) { return Opt.TryGet(); }) |
                UE::Optionals::GetPtrOrNull;
        }) |
        UE::Ranges::ForEach(this, &UPokemonIconPreview::CreateTypeIcon);
    // clang-format on
}

void UPokemonIconPreview::CreateTypeIcon(UObject *Resource) {
    auto TypeIcon = WidgetTree->ConstructWidget<UImage>();
    UWidgetUtilities::SetBrushFromAsset(TypeIcon, Resource, true);
    SlotTypeIcon(TypeIcon);
    TypeIcons.Emplace(TypeIcon);
}