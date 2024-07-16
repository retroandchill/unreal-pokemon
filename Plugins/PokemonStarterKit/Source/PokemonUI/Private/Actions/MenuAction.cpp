// "Unreal Pokémon" created by Retro & Chill.
#include "Actions/MenuAction.h"
#include "PrimaryGameLayout.h"
#include "Screens/PauseMenuScreen.h"

void UMenuAction::PerformAction_Implementation(UObject *Owner) {
    Super::PerformAction_Implementation(Owner);
    auto Player = CastChecked<APawn>(Owner);
    auto Controller = CastChecked<APlayerController>(Player->GetController());
    UPrimaryGameLayout::GetPrimaryGameLayout(Controller)
        ->PushWidgetToLayerStack(RPG::Menus::PrimaryMenuLayerTag, MenuClass);
}
