// "Unreal Pokémon" created by Retro & Chill.


#include "Utilities/RPGMenuUtilities.h"
#include "PrimaryGameLayout.h"
#include "Screens/Screen.h"

UScreen * URPGMenuUtilities::PushScreenToStack(const UObject *WorldContextObject, TSubclassOf<UScreen> ScreenType) {
    auto Layout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(WorldContextObject);
    return Layout->PushWidgetToLayerStack<UScreen>(RPG::Menus::PrimaryMenuLayerTag, ScreenType);
}

UScreen * URPGMenuUtilities::RemoveTopScreenFromOverlay(const UObject *WorldContextObject) {
    auto Layout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(WorldContextObject);
    auto Layer = Layout->GetLayerWidget(RPG::Menus::OverlayMenuLayerTag);
    auto Screen = Cast<UScreen>(Layer->GetActiveWidget());

    if (Screen != nullptr) {
        Layer->RemoveWidget(*Screen);
    }

    return Screen;
}