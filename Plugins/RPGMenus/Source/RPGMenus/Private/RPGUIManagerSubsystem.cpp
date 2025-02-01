// "Unreal Pokémon" created by Retro & Chill.

#include "RPGUIManagerSubsystem.h"
#include "Data/RPGMenusSettings.h"
#include "EnhancedInputSubsystems.h"
#include "GameUIPolicy.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"
#include "PrimaryGameLayout.h"
#include "RPGMenus.h"
#include "Screens/Screen.h"

void URPGUIManagerSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
    Super::Initialize(Collection);

    if (!CurrentPolicy && !DefaultUIPolicyClass.IsNull()) {
        TSubclassOf<UGameUIPolicy> PolicyClass = DefaultUIPolicyClass.LoadSynchronous();
        SwitchToPolicy(NewObject<UGameUIPolicy>(this, PolicyClass));
    }

    MenuMappingContext =
        CastChecked<UInputMappingContext>(GetDefault<URPGMenusSettings>()->MenuMappingContext.TryLoad());
}

void URPGUIManagerSubsystem::Deinitialize() {
    Super::Deinitialize();
    SwitchToPolicy(nullptr);
}

bool URPGUIManagerSubsystem::ShouldCreateSubsystem(UObject *Outer) const {
    if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance()) {
        TArray<UClass *> ChildClasses;
        GetDerivedClasses(GetClass(), ChildClasses, false);

        // Only create an instance if there is no override implementation defined elsewhere
        return ChildClasses.Num() == 0;
    }

    return false;
}

void URPGUIManagerSubsystem::NotifyPlayerAdded(URPGLocalPlayer *LocalPlayer) {
    if (ensure(LocalPlayer != nullptr) && CurrentPolicy != nullptr) {
        CurrentPolicy->NotifyPlayerAdded(LocalPlayer);
    }
}

void URPGUIManagerSubsystem::NotifyPlayerRemoved(URPGLocalPlayer *LocalPlayer) {
    if (ensure(LocalPlayer != nullptr) && CurrentPolicy != nullptr) {
        CurrentPolicy->NotifyPlayerRemoved(LocalPlayer);
    }
}

void URPGUIManagerSubsystem::NotifyPlayerDestroyed(URPGLocalPlayer *LocalPlayer) {
    if (ensure(LocalPlayer != nullptr) && CurrentPolicy != nullptr) {
        CurrentPolicy->NotifyPlayerDestroyed(LocalPlayer);
    }
}

void URPGUIManagerSubsystem::SwitchToPolicy(UGameUIPolicy *InPolicy) {
    if (CurrentPolicy != InPolicy) {
        CurrentPolicy = InPolicy;
    }
}

URPGUIManagerSubsystem &URPGUIManagerSubsystem::Get(const UObject *WorldContext) {
    auto GameInstance = UGameplayStatics::GetGameInstance(WorldContext);
    check(GameInstance != nullptr)
    auto Subsystem = GameInstance->GetSubsystem<URPGUIManagerSubsystem>();
    check(Subsystem != nullptr)
    return *Subsystem;
}

UScreen *URPGUIManagerSubsystem::GetTopScreenOfStack() const {
    auto Layout = UPrimaryGameLayout::Get(this);
    auto Layer = Layout->GetLayerWidget(RPG::Menus::PrimaryMenuLayerTag);
    return Cast<UScreen>(Layer->GetActiveWidget());
}

UScreen *URPGUIManagerSubsystem::GetTopScreenOfOverlay() const {
    auto Layout = UPrimaryGameLayout::Get(this);
    auto Layer = Layout->GetLayerWidget(RPG::Menus::OverlayMenuLayerTag);
    return Cast<UScreen>(Layer->GetActiveWidget());
}

void URPGUIManagerSubsystem::OnScreenActivated(UScreen *Screen) {
    if (ActiveScreenCount == 0) {
        UE_LOG(LogRPGMenus, Display, TEXT("Adding menu input mapping context!"))
        auto Subsystem = Screen->GetOwningLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

#if WITH_EDITOR
        if (Subsystem != nullptr) {
#endif
            Subsystem->AddMappingContext(MenuMappingContext, GetDefault<URPGMenusSettings>()->MenuMappingPriority);
#if WITH_EDITOR
        }
#endif

        Screen->GetOwningPlayer()->SetInputMode(FInputModeUIOnly());
    }

    ActiveScreenCount++;
}

void URPGUIManagerSubsystem::OnScreenDeactivated(UScreen *Screen) {
    check(ActiveScreenCount > 0)
    ActiveScreenCount--;

    if (ActiveScreenCount == 0) {
        UE_LOG(LogRPGMenus, Display, TEXT("Removing menu input mapping context!"))
        auto Subsystem = Screen->GetOwningLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
#if WITH_EDITOR
        if (Subsystem == nullptr) {
            return;
        }
#endif

        Subsystem->RemoveMappingContext(MenuMappingContext);
        Screen->GetOwningPlayer()->SetInputMode(FInputModeGameOnly());
    }
}