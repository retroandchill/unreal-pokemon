// Fill out your copyright notice in the Description page of Project Settings.

#include "TestWorld.h"
#include "GameMapsSettings.h"
#include "UnrealSharpTestUtils.h"

FTestWorld::FTestWorld(const bool bBeginPlay) : GameInstance(CreateGameInstance()), World(GameInstance->GetWorld())
{
    const auto WorldContext = GameInstance->GetWorldContext();
    WorldContext->GameViewport = NewObject<UGameViewportClient>(GEngine, GEngine->GameViewportClientClass);
    WorldContext->GameViewport->Init(*WorldContext, GameInstance.Get(), false);

    WorldContext->GameViewport->SetViewportOverlayWidget(nullptr, Overlay);
    WorldContext->GameViewport->SetGameLayerManager(GameLayerManager);

    if (bBeginPlay)
    {
        BeginPlay();
    }
}

void FTestWorld::BeginPlay()
{
    if (bIsPlay)
    {
        UE_LOG(LogUnrealSharpTestUtils, Warning, TEXT("BeginPlay called twice for the same world"));
        return;
    }

    World->InitializeActorsForPlay(World->URL);
    World->BeginPlay();
    World->SetBegunPlay(true);
    bIsPlay = true;
}

FGameInstancePtr FTestWorld::CreateGameInstance()
{
    auto &GameInstanceClassName = GetDefault<UGameMapsSettings>()->GameInstanceClass;
    const auto GameInstanceClass = GameInstanceClassName.TryLoadClass<UGameInstance>();
    FGameInstancePtr GameInstance(NewObject<UGameInstance>(GEngine, GameInstanceClass));
    GameInstance->InitializeStandalone();
    return GameInstance;
}
