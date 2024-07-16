// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/WidgetTestUtilities.h"
#include "CommonGameViewportClient.h"
#include "GameMapsSettings.h"
#include "GeneralEngineSettings.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Engine/GameEngine.h"
#include "Utilities/RAII.h"

class UGameMapsSettings;

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

TTuple<FDudWidgets, FWorldPtr, FGameInstancePtr> UWidgetTestUtilities::CreateTestWorld(bool bBeginPlay) {
    auto GameInstanceClassName = GetDefault<UGameMapsSettings>()->GameInstanceClass;
    auto GameInstanceClass = GameInstanceClassName.TryLoadClass<UGameInstance>();
    FGameInstancePtr GameInstance(NewObject<UGameInstance>(GEngine, GameInstanceClass));
    GameInstance->InitializeStandalone(); // creates WorldContext, UWorld?
    FWorldPtr World(GameInstance->GetWorld());
    auto WorldContext = GameInstance->GetWorldContext();
    WorldContext->GameViewport = NewObject<UCommonGameViewportClient>(GEngine, GEngine->GameViewportClientClass);
    WorldContext->GameViewport->Init(*WorldContext, GameInstance.Get(), false);
    FDudWidgets DudWidgets;
    WorldContext->GameViewport->SetViewportOverlayWidget(nullptr, DudWidgets.DudOverlay);
    WorldContext->GameViewport->SetGameLayerManager(DudWidgets.GameLayerManager);

    if (bBeginPlay) {
        World->InitializeActorsForPlay(World->URL);
        World->BeginPlay();
        World->SetBegunPlay(true);
    }
    
    return {DudWidgets, MoveTemp(World), MoveTemp(GameInstance)};
}
