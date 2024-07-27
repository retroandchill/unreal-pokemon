// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/RPGMenuUtilities.h"
#include "PrimaryGameLayout.h"
#include "Screens/Screen.h"

UScreen *URPGMenuUtilities::PushScreenToStack(const UObject *WorldContextObject, TSubclassOf<UScreen> ScreenType) {
    auto Layout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(WorldContextObject);
    return Layout->PushWidgetToLayerStack<UScreen>(RPG::Menus::PrimaryMenuLayerTag, ScreenType);
}

UScreen * URPGMenuUtilities::LoadAndPushScreenToStack(const UObject *WorldContextObject,
    const TSoftClassPtr<UScreen> &ScreenType) {
    return PushScreenToStack(WorldContextObject, ScreenType.LoadSynchronous());
}

UScreen *URPGMenuUtilities::RemoveTopScreenFromStackLayer(const UObject *WorldContextObject, FGameplayTag Tag) {
    auto Layout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(WorldContextObject);
    if (Layout == nullptr) {
        return nullptr;
    }
    auto Layer = Layout->GetLayerWidget(Tag);

    if (auto Screen = Cast<UScreen>(Layer->GetActiveWidget()); Screen != nullptr) {
        Layer->RemoveWidget(*Screen);
    }

    return Cast<UScreen>(Layer->GetActiveWidget());
}

UScreen *URPGMenuUtilities::RemoveTopScreenFromStack(const UObject *WorldContextObject) {
    return RemoveTopScreenFromStackLayer(WorldContextObject, RPG::Menus::PrimaryMenuLayerTag);
}

UScreen *URPGMenuUtilities::RemoveTopScreenFromOverlay(const UObject *WorldContextObject) {
    return RemoveTopScreenFromStackLayer(WorldContextObject, RPG::Menus::OverlayMenuLayerTag);
}