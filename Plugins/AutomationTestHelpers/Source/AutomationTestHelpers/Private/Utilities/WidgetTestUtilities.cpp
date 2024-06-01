// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/WidgetTestUtilities.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Utilities/RAII.h"

UWidget *UWidgetTestUtilities::FindChildWidget(UUserWidget *Parent, FName WidgetName) {
    auto RootWidget = Parent->GetRootWidget();
    if (RootWidget->GetFName() == WidgetName) {
        return RootWidget;
    }

    auto Panel = Cast<UPanelWidget>(RootWidget);
    if (Panel == nullptr) {
        return nullptr;
    }

    int32 Index;
    return UWidgetTree::FindWidgetChild(Panel, WidgetName, Index);
}

std::tuple<TSharedRef<SOverlay>, FWorldPtr, FGameInstancePtr> UWidgetTestUtilities::CreateTestWorld() {
    FGameInstancePtr GameInstance(NewObject<UGameInstance>(GEngine));
    GameInstance->InitializeStandalone(); // creates WorldContext, UWorld?
    FWorldPtr World(GameInstance->GetWorld());
    auto WorldContext = GameInstance->GetWorldContext();
    WorldContext->GameViewport = NewObject<UGameViewportClient>(GEngine);
    WorldContext->GameViewport->Init(*WorldContext, GameInstance.Get(), false);
    TSharedRef<SOverlay> DudOverlay = SNew(SOverlay);
    WorldContext->GameViewport->SetViewportOverlayWidget(nullptr, DudOverlay);

    return {DudOverlay, MoveTemp(World), MoveTemp(GameInstance)};
}
