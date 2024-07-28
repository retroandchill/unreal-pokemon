// "Unreal Pokémon" created by Retro & Chill.

#include "RPGUIManagerSubsystem.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "PrimaryGameLayout.h"
#include "RPGMenus.h"
#include "Data/RPGMenusSettings.h"
#include "Screens/Screen.h"

void URPGUIManagerSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
    Super::Initialize(Collection);
    MenuMappingContext = CastChecked<UInputMappingContext>(GetDefault<URPGMenusSettings>()->MenuMappingContext.TryLoad());
}

UScreen *URPGUIManagerSubsystem::GetTopScreenOfStack() const {
    auto Layout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(this);
    auto Layer = Layout->GetLayerWidget(RPG::Menus::PrimaryMenuLayerTag);
    return Cast<UScreen>(Layer->GetActiveWidget());
}

UScreen *URPGUIManagerSubsystem::GetTopScreenOfOverlay() const {
    auto Layout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(this);
    auto Layer = Layout->GetLayerWidget(RPG::Menus::OverlayMenuLayerTag);
    return Cast<UScreen>(Layer->GetActiveWidget());
}

void URPGUIManagerSubsystem::OnScreenActivated(UScreen *Screen) {
    if (ActiveScreenCount == 0) {
        UE_LOG(LogRPGMenus, Display, TEXT("Adding menu input mapping context!"))
        auto Subsystem = Screen->GetOwningLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
        Subsystem->AddMappingContext(MenuMappingContext, GetDefault<URPGMenusSettings>()->MenuMappingPriority);
    }

    ActiveScreenCount++;
}

void URPGUIManagerSubsystem::OnScreenDeactivated(UScreen *Screen) {
    check(ActiveScreenCount > 0)
    ActiveScreenCount--;

    if (ActiveScreenCount == 0) {
        UE_LOG(LogRPGMenus, Display, TEXT("Removing menu input mapping context!"))
        auto Subsystem = Screen->GetOwningLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
        Subsystem->RemoveMappingContext(MenuMappingContext);
    }
}