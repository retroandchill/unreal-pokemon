// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/InteractiveUIUtilities.h"
#include "PrimaryGameLayout.h"
#include "Screens/Screen.h"

UCommonActivatableWidget *UInteractiveUIUtilities::RemoveTopScreenFromStackLayer(const UObject *WorldContextObject, FGameplayTag Tag)
{
    const auto *Layout = UPrimaryGameLayout::GetInstance(WorldContextObject);
    if (Layout == nullptr)
    {
        return nullptr;
    }
    auto *Layer = Layout->GetLayerWidget(Tag);

    if (auto *Screen = Cast<UCommonActivatableWidget>(Layer->GetActiveWidget()); Screen != nullptr)
    {
        Layer->RemoveWidget(*Screen);
    }

    return Cast<UCommonActivatableWidget>(Layer->GetActiveWidget());
}

UCommonActivatableWidget *UInteractiveUIUtilities::RemoveTopScreenFromStack(const UObject *WorldContextObject)
{
    return RemoveTopScreenFromStackLayer(WorldContextObject, RPG::Menus::PrimaryMenuLayerTag);
}

UCommonActivatableWidget *UInteractiveUIUtilities::RemoveTopScreenFromOverlay(const UObject *WorldContextObject)
{
    return RemoveTopScreenFromStackLayer(WorldContextObject, RPG::Menus::OverlayMenuLayerTag);
}