// "Unreal Pokémon" created by Retro & Chill.


#include "Nodes/PushMenuToStack.h"
#include "PrimaryGameLayout.h"
#include "PokemonMenuBase.h"

UE_DEFINE_GAMEPLAY_TAG(Pokemon::UI::PokemonMenuLayerTag, "UI.Layer.GameMenu");

UPushMenuToStack *UPushMenuToStack::PushMenuToStack(const UObject *WorldContextObject,
                                                    TSoftClassPtr<UPokemonMenuBase> MenuClass) {
    auto Node = NewObject<UPushMenuToStack>();
    Node->WorldContextObject = WorldContextObject;
    Node->MenuClass = MoveTemp(MenuClass);
    return Node;
}

void UPushMenuToStack::Activate() {
    auto Layout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(WorldContextObject);
    check(Layout != nullptr)
    TWeakObjectPtr<UPushMenuToStack> WeakThis = this;

    auto OnDestroyed = [this, WeakThis](EAsyncWidgetLayerState State, UPokemonMenuBase *Widget) {
        if (!WeakThis.IsValid()) {
            SetReadyToDestroy();
            return;
        }
        
        switch (State) {
        case EAsyncWidgetLayerState::Initialize:
            // Do nothing on initialize
                break;
        case EAsyncWidgetLayerState::AfterPush:
            MenuLoaded.Broadcast(Widget);
            SetReadyToDestroy();
            break;
        case EAsyncWidgetLayerState::Canceled:
            SetReadyToDestroy();
            break;
        }
    };
    LoadHandle = Layout->PushWidgetToLayerStackAsync<UPokemonMenuBase>(Pokemon::UI::PokemonMenuLayerTag, true,MenuClass, OnDestroyed);
}

void UPushMenuToStack::SetReadyToDestroy() {
    Super::SetReadyToDestroy();
    LoadHandle.Reset();
}