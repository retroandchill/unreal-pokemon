// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Summary/PokemonInfoPage.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "DataManager.h"
#include "Graphics/GraphicsLoadingSubsystem.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/Stats/StatBlock.h"
#include "Primatives/DisplayText.h"
#include "Species/SpeciesData.h"
#include "Trainers/OwnerInfo.h"
#include "Utilities/PokemonUIUtils.h"
#include "Utilities/WidgetUtilities.h"

void UPokemonInfoPage::RefreshInfo_Implementation(const TScriptInterface<IPokemon> &Pokemon) {
    Super::RefreshInfo_Implementation(Pokemon);

    // TODO: Handle the regional Pokédex number
    auto &Species = Pokemon->GetSpecies();
    PokemonNumberText->SetText(
        FText::FromString(UPokemonUIUtils::ZeroPad(Species.RowNumber, PokedexNumberLength)));
    SpeciesNameText->SetText(Species.RealName);

    ClearTypeIcons();
    auto GraphicsLoadingSubsystem = GetGameInstance()->GetSubsystem<UGraphicsLoadingSubsystem>();
    for (auto Types = GraphicsLoadingSubsystem->GetTypeIconGraphics(Pokemon->GetTypes()); auto Asset : Types) {
        auto Icon = WidgetTree->ConstructWidget<UImage>();
        UWidgetUtilities::SetBrushFromAsset(Icon, Asset, true);
        SlotTypeIcon(Icon);
    }

    auto &OwnerInfo = Pokemon->GetOwnerInfo();
    OTNameText->SetText(OwnerInfo.OriginalTrainerName);
    if (auto Color = GenderTextColors.Find(OwnerInfo.OriginalTrainerGender); Color != nullptr) {
        UPokemonUIUtils::SetItemTextColor(OTNameText, *Color);
    }
    PokemonIDText->SetText(FText::FromString(UPokemonUIUtils::ZeroPad(OwnerInfo.ID, IdNumberLength)));

    auto StatBlock = Pokemon->GetStatBlock();
    ExpTotalText->SetText(FText::FromString(FString::FromInt(StatBlock->GetExp())));
    NextLevelUpCountText->SetText(FText::FromString(FString::FromInt(StatBlock->GetExpForNextLevel())));
    ExpBar->SetPercent(StatBlock->GetExpPercent());
}