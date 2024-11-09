// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Summary/PokemonInfoPage.h"
#include "Blueprint/WidgetTree.h"
#include "Components/DisplayText.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Graphics/AssetClasses.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/Stats/StatBlock.h"
#include "Ranges/Optional/GetPtrOrNull.h"
#include "Ranges/Optional/IfPresent.h"
#include "Species/SpeciesData.h"
#include "Trainers/OwnerInfo.h"
#include "Utilities/PokemonUIUtils.h"
#include "Utilities/WidgetUtilities.h"

void UPokemonInfoPage::OnPokemonSet_Implementation(const TScriptInterface<IPokemon> &NewPokemon) {
    // TODO: Handle the regional Pokédex number
    auto &Species = NewPokemon->GetSpecies();
    PokemonNumberText->SetText(FText::FromString(UPokemonUIUtils::ZeroPad(Species.RowID, PokedexNumberLength)));
    SpeciesNameText->SetText(Species.RealName);

    ClearTypeIcons();

    for (auto Types = Pokemon::Assets::Graphics::TypeIcons.LoadAssets(NewPokemon->GetTypes()); auto Asset : Types) {
        // clang-format off
        auto AssetObject = Asset |
                           UE::Optionals::FlatMap([](const FImageAsset &ImageAsset) { return ImageAsset.TryGet();}) |
                           UE::Optionals::GetPtrOrNull;
        // clang-format on
        auto Icon = WidgetTree->ConstructWidget<UImage>();
        UWidgetUtilities::SetBrushFromAsset(Icon, AssetObject, true);
        SlotTypeIcon(Icon);
    }

    auto &OwnerInfo = NewPokemon->GetOwnerInfo();
    OTNameText->SetText(OwnerInfo.OriginalTrainerName);
    if (auto Color = GenderTextColors.Find(OwnerInfo.OriginalTrainerGender); Color != nullptr) {
        OTNameText->SetTextStyle(*Color);
    }
    PokemonIDText->SetText(FText::FromString(UPokemonUIUtils::ZeroPad(OwnerInfo.ID, IdNumberLength)));

    auto StatBlock = NewPokemon->GetStatBlock();
    ExpTotalText->SetText(FText::FromString(FString::FromInt(StatBlock->GetExp())));
    NextLevelUpCountText->SetText(FText::FromString(FString::FromInt(StatBlock->GetExpForNextLevel())));
    ExpBar->SetPercent(StatBlock->GetExpPercent());

    using enum ESlateVisibility;
    if (auto HoldItem = NewPokemon->GetHoldItem(); HoldItem.IsSet()) {
        ItemNameText->SetText(HoldItem->RealName);
        ItemDescriptionText->SetText(HoldItem->Description);
        // clang-format off
        auto Item = Pokemon::Assets::Graphics::ItemIcons.LoadAsset(HoldItem->ID) |
                           UE::Optionals::Map([](const FImageAsset &Asset) -> auto &{ return Asset.Get(); }) |
                           UE::Optionals::GetPtrOrNull;
        // clang-format on
        UWidgetUtilities::SetBrushFromAsset(ItemIcon, Item, true);
        ItemIcon->SetVisibility(SelfHitTestInvisible);
    } else {
        ItemNameText->SetText(NoItemText);
        ItemIcon->SetVisibility(Hidden);
        ItemDescriptionText->SetText(FText::GetEmpty());
    }

    ShinyIcon->SetVisibility(NewPokemon->IsShiny() ? SelfHitTestInvisible : Hidden);
}