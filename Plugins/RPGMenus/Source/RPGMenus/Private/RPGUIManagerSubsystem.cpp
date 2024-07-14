// "Unreal Pokémon" created by Retro & Chill.


#include "RPGUIManagerSubsystem.h"
#include "PrimaryGameLayout.h"
#include "Screens/Screen.h"

UScreen * URPGUIManagerSubsystem::GetTopScreenOfStack() const {
    auto Layout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(this);
    auto Layer = Layout->GetLayerWidget(RPG::Menus::PrimaryMenuLayerTag);
    return Cast<UScreen>(Layer->GetActiveWidget());
}