// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/WidgetTestUtilities.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"

UWidget *UWidgetTestUtilities::FindChildWidget(UUserWidget *Parent, FName WidgetName) {
    auto Panel = Cast<UPanelWidget>(Parent->GetRootWidget());
    if (Panel == nullptr) {
        return nullptr;
    }

    int32 Index;
    return UWidgetTree::FindWidgetChild(Panel, WidgetName, Index);
}

TPair<TSharedRef<SOverlay>, UWorld *> UWidgetTestUtilities::CreateTestWorld() {
    auto GameInstance = NewObject<UGameInstance>(GEngine);
    GameInstance->InitializeStandalone(); // creates WorldContext, UWorld?
    auto World = GameInstance->GetWorld();
    auto WorldContext = GameInstance->GetWorldContext();
    WorldContext->GameViewport = NewObject<UGameViewportClient>(GEngine);
    TSharedRef<SOverlay> DudOverlay = SNew(SOverlay);
    WorldContext->GameViewport->SetViewportOverlayWidget(nullptr, DudOverlay);

    return {DudOverlay, World};
}
