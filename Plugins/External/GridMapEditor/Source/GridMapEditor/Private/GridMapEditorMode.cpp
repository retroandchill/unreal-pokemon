// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GridMapEditorMode.h"
#include "DrawDebugHelpers.h"
#include "EditorModeManager.h"
#include "EditorViewportClient.h"
#include "Engine/CollisionProfile.h"
#include "Engine/Engine.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Framework/Commands/UICommandList.h"
#include "GridMapEditCommands.h"
#include "GridMapEditorModeToolkit.h"
#include "GridMapStaticMeshActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "TileSet.h"
#include "Toolkits/ToolkitManager.h"

static FName GridMapBrushHighlightColorParamName("HighlightColor");

const FEditorModeID FGridMapEditorMode::EM_GridMapEditorModeId = TEXT("EM_GridMapEditorMode");

FGridMapEditorMode::FGridMapEditorMode() : FEdMode(), ActiveTileSet(nullptr) {
    BrushDefaultHighlightColor = FColor(127, 127, 255, 255);
    BrushWarningHighlightColor = FColor::Red;

    // Load resources and construct brush component
    UStaticMesh *StaticMesh = nullptr;
    if (!IsRunningCommandlet()) {
        UMaterial *BrushMaterial = LoadObject<UMaterial>(
            nullptr, TEXT("/GridMapEditor/M_TileBrushCube.M_TileBrushCube"), nullptr, LOAD_None, nullptr);
        BrushMID = UMaterialInstanceDynamic::Create(BrushMaterial, GetTransientPackage());
        check(BrushMID != nullptr)
        FLinearColor DefaultColor;
        BrushMID->GetVectorParameterDefaultValue(GridMapBrushHighlightColorParamName, DefaultColor);
        BrushDefaultHighlightColor = DefaultColor.ToFColor(false);
        StaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/GridMapEditor/SM_TileBrushCube.SM_TileBrushCube"), nullptr,
                                             LOAD_None, nullptr);
    }

    BrushCurrentHighlightColor = BrushDefaultHighlightColor;
    TileBrushComponent = NewObject<UStaticMeshComponent>(GetTransientPackage(), TEXT("TileBrushComponent"));
    TileBrushComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
    TileBrushComponent->SetCollisionObjectType(ECC_WorldDynamic);
    TileBrushComponent->SetStaticMesh(StaticMesh);
    TileBrushComponent->SetMaterial(0, BrushMID);
    TileBrushComponent->SetAbsolute(true, true, true);
    TileBrushComponent->CastShadow = false;

    bBrushTraceValid = false;
    BrushLocation = FVector::ZeroVector;

    // Setup and bind commands
    UICommandList = MakeShareable(new FUICommandList);
    BindCommandList();
}

void FGridMapEditorMode::BindCommandList() {
    const FGridMapEditCommands &Commands = FGridMapEditCommands::Get();

    UICommandList->MapAction(Commands.SetPaintTiles,
                             FExecuteAction::CreateRaw(this, &FGridMapEditorMode::OnSetPaintTiles), FCanExecuteAction(),
                             FIsActionChecked::CreateLambda([this] { return UISettings.GetPaintToolSelected(); }));

    UICommandList->MapAction(
        Commands.SetSelectTiles, FExecuteAction::CreateRaw(this, &FGridMapEditorMode::OnSetSelectTiles),
        FCanExecuteAction(), FIsActionChecked::CreateLambda([this] { return UISettings.GetSelectToolSelected(); }));

    UICommandList->MapAction(
        Commands.SetTileSettings, FExecuteAction::CreateRaw(this, &FGridMapEditorMode::OnSetTileSettings),
        FCanExecuteAction(), FIsActionChecked::CreateLambda([this] { return UISettings.GetSettingsToolSelected(); }));
}

void FGridMapEditorMode::Enter() {
    FEdMode::Enter();

    // Clear any selections
    const bool bNoteSelectionChange(false);
    const bool bDeslectBSPSurfs(true);
    GEditor->SelectNone(bNoteSelectionChange, bDeslectBSPSurfs);

    if (!Toolkit.IsValid() && UsesToolkits()) {
        Toolkit = MakeShareable(new FGridMapEditorModeToolkit);
        Toolkit->Init(Owner->GetToolkitHost());
    }

    const bool bNewVisibility(true);
    TileBrushComponent->SetVisibility(bNewVisibility);
}

void FGridMapEditorMode::Exit() {
    if (Toolkit.IsValid()) {
        FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
        Toolkit.Reset();
    }

    // Remove the brush
    TileBrushComponent->UnregisterComponent();

    // Call base Exit method to ensure proper cleanup
    FEdMode::Exit();
}

void FGridMapEditorMode::Tick(FEditorViewportClient *ViewportClient, float DeltaTime) {
    if (!IsEditingEnabled())
        return;

    if (bBrushTraceValid) {
        auto BrushScaleCalc = [this](const UGridMapTileSet &TileSet) {
            auto BaseSize = TileBrushComponent->GetStaticMesh()->GetBounds().GetBox().GetSize();
            return FVector(TileSet.TileSize * TileSet.SizeX / BaseSize.X, TileSet.TileSize * TileSet.SizeY / BaseSize.Y,
                           TileSet.TileHeight * TileSet.SizeZ / BaseSize.Z);
        };
        auto BrushScaleOffset = [](const UGridMapTileSet &TileSet) {
            return FVector(TileSet.TileSize * (TileSet.SizeX - 1) / 2, TileSet.TileSize * (TileSet.SizeY - 1) / 2,
                           TileSet.TileHeight * (TileSet.SizeZ - 1) / 2);
        };

        auto BrushScale = Retro::Optionals::OfNullable(ActiveTileSet) | Retro::Optionals::Transform(BrushScaleCalc) |
                          Retro::Optionals::OrElseValue(FVector::OneVector);
        auto BrushOffset = Retro::Optionals::OfNullable(ActiveTileSet) | Retro::Optionals::Transform(BrushScaleOffset) |
                           Retro::Optionals::OrElseValue(FVector::ZeroVector);
        FTransform BrushTransform = FTransform(FQuat::Identity, BrushLocation + BrushOffset, BrushScale);
        TileBrushComponent->SetRelativeTransform(BrushTransform);

        // warning color if we're erasing tiles
        FColor BrushHighlightColor = BrushDefaultHighlightColor;
        if (UISettings.GetPaintMode() == EGridMapPaintMode::Erase) {
            BrushHighlightColor = BrushWarningHighlightColor;
        }

        // adjust color if needed
        if (BrushCurrentHighlightColor != BrushHighlightColor) {
            BrushCurrentHighlightColor = BrushHighlightColor;
            BrushMID->SetVectorParameterValue(GridMapBrushHighlightColorParamName, BrushHighlightColor);
        }

        if (!TileBrushComponent->IsRegistered()) {
            TileBrushComponent->RegisterComponentWithWorld(ViewportClient->GetWorld());
        }
    } else {
        if (TileBrushComponent->IsRegistered()) {
            TileBrushComponent->UnregisterComponent();
        }
    }
}

bool FGridMapEditorMode::UsesToolkits() const {
    return true;
}

void FGridMapEditorMode::AddReferencedObjects(FReferenceCollector &Collector) {
    FEdMode::AddReferencedObjects(Collector);
    Collector.AddReferencedObject(TileBrushComponent);
}

bool FGridMapEditorMode::StartTracking(FEditorViewportClient *InViewportClient, FViewport *InViewport) {
    return FEdMode::StartTracking(InViewportClient, InViewport);
}

bool FGridMapEditorMode::EndTracking(FEditorViewportClient *InViewportClient, FViewport *InViewport) {
    return FEdMode::EndTracking(InViewportClient, InViewport);
}

bool FGridMapEditorMode::MouseMove(FEditorViewportClient *ViewportClient, FViewport *Viewport, int32 MouseX,
                                   int32 MouseY) {
    if (IsEditingEnabled()) {
        // Compute a world space ray from the screen space mouse coordinates
        FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(ViewportClient->Viewport,
                                                                                ViewportClient->GetScene(),
                                                                                ViewportClient->EngineShowFlags)
                                               .SetRealtimeUpdate(ViewportClient->IsRealtime()));

        FSceneView *View = ViewportClient->CalcSceneView(&ViewFamily);
        FViewportCursorLocation MouseViewportRay(View, ViewportClient, MouseX, MouseY);
        BrushTraceDirection = MouseViewportRay.GetDirection();

        FVector BrushTraceStart = MouseViewportRay.GetOrigin();
        if (ViewportClient->IsOrtho()) {
            BrushTraceStart += -WORLD_MAX * BrushTraceDirection;
        }

        GridMapBrushTrace(ViewportClient, BrushTraceStart, BrushTraceDirection);
    }
    return false;
}

bool FGridMapEditorMode::CapturedMouseMove(FEditorViewportClient *InViewportClient, FViewport *InViewport,
                                           int32 InMouseX, int32 InMouseY) {
    // Compute a world space ray from the screen space mouse coordinates
    FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(InViewportClient->Viewport,
                                                                            InViewportClient->GetScene(),
                                                                            InViewportClient->EngineShowFlags)
                                           .SetRealtimeUpdate(InViewportClient->IsRealtime()));

    FSceneView *View = InViewportClient->CalcSceneView(&ViewFamily);
    FViewportCursorLocation MouseViewportRay(View, InViewportClient, InMouseX, InMouseY);
    BrushTraceDirection = MouseViewportRay.GetDirection();

    FVector BrushTraceStart = MouseViewportRay.GetOrigin();
    if (InViewportClient->IsOrtho()) {
        BrushTraceStart += -WORLD_MAX * BrushTraceDirection;
    }

    GridMapBrushTrace(InViewportClient, BrushTraceStart, BrushTraceDirection);

    PaintTile();
    return true;
}

void FGridMapEditorMode::PaintTile() {
    // are we erasing?
    if (bIsPainting && bBrushTraceValid && UISettings.GetPaintMode() == EGridMapPaintMode::Erase &&
        UISettings.GetCurrentTileSet().IsValid()) {
        // and we're on top of something?
        if (BrushTraceHitActor.IsValid() && Cast<AGridMapStaticMeshActor>(BrushTraceHitActor.Get())) {
            EraseTile(Cast<AGridMapStaticMeshActor>(BrushTraceHitActor.Get()));
            BrushTraceHitActor.Reset();
        }
        return;
    }

    // if we're painting,
    if (bIsPainting && bBrushTraceValid && UISettings.GetCurrentTileSet().IsValid()) {
        UGridMapTileSet *TileSet = UISettings.GetCurrentTileSet().Get();

        // if it's the same tile set, don't do anything
        if (BrushTraceHitActor.IsValid()) {
            AGridMapStaticMeshActor *BrushTraceHitTile = Cast<AGridMapStaticMeshActor>(BrushTraceHitActor.Get());
            if (BrushTraceHitTile && BrushTraceHitTile->TileSet &&
                TileSet->TileTags.HasAllExact(BrushTraceHitTile->TileSet->TileTags))
                return;
        }

        // we're about to replace an actor
        if (BrushTraceHitActor.IsValid()) {
            EraseTile(Cast<AGridMapStaticMeshActor>(BrushTraceHitActor.Get()));
            BrushTraceHitActor.Reset();
        }

        // figure out the bitmask for this tile
        uint32 Adjacency = GetTileAdjacencyBitmask(GetWorld(), BrushLocation, TileSet, UISettings.GetPaintLayer());
        auto TileList = TileSet->FindTilesForAdjacency(Adjacency);
        if (TileList.IsSet()) {
            TSoftObjectPtr<UStaticMesh> StaticMeshAsset = TileList->GetRandomTile();

            FActorSpawnParameters SpawnParameters;
            AGridMapStaticMeshActor *MeshActor = GetWorld()->SpawnActor<AGridMapStaticMeshActor>(SpawnParameters);
            auto FolderPath = FString::Format(TEXT("Tiles/Layer{0}"), {UISettings.GetPaintLayer()});
            MeshActor->SetFolderPath(*FolderPath);
            MeshActor->TileSet = TileSet;
            MeshActor->CurrentLayer = UISettings.GetPaintLayer();

            // Rename the display name of the new actor in the editor to reflect the mesh that is being created from.
            FActorLabelUtilities::SetActorLabelUnique(MeshActor, CreateActorLabel(TileSet));

            UStaticMesh *StaticMesh = Cast<UStaticMesh>(StaticMeshAsset.LoadSynchronous());
            MeshActor->GetStaticMeshComponent()->SetStaticMesh(StaticMesh);
            MeshActor->ReregisterAllComponents();

            auto Offset = Retro::Optionals::OfNullable(ActiveTileSet) | Retro::Optionals::Transform<&UGridMapTileSet::BrushOffset>() |
                          Retro::Optionals::Transform([](const FVector2D &V) { return FVector(V, 0); }) |
                          Retro::Optionals::OrElseValue(FVector::ZeroVector);
            FTransform BrushTransform =
                FTransform(TileList->Rotation.Quaternion(), BrushLocation + Offset, FVector::OneVector);
            MeshActor->SetActorTransform(BrushTransform);

            // update adjacent tiles
            TArray<FAdjacentTile> AdjacentTiles;
            if (GetAdjacentTiles(GetWorld(), BrushLocation, AdjacentTiles, UISettings.GetPaintLayer())) {
                UpdateAdjacentTiles(GetWorld(), AdjacentTiles);
            }
        }
    }
}

void FGridMapEditorMode::EraseTile(AGridMapStaticMeshActor *TileToErase) {
    TArray<FAdjacentTile> AdjacentTiles;
    bool hasAdjacentTiles =
        GetAdjacentTiles(GetWorld(), TileToErase->GetActorLocation(), AdjacentTiles, UISettings.GetPaintLayer());

    GetWorld()->DestroyActor(TileToErase);

    if (hasAdjacentTiles) {
        UpdateAdjacentTiles(GetWorld(), AdjacentTiles);
    }
}

bool FGridMapEditorMode::InputKey(FEditorViewportClient *InViewportClient, FViewport *InViewport, FKey InKey,
                                  EInputEvent InEvent) {
    bool bHandled = false;

    const bool bEventIsLeftButtonDown = (InKey == EKeys::LeftMouseButton && InEvent != IE_Released);
    const bool bKeystateIsLeftButtonDown = InViewport->KeyState(EKeys::LeftMouseButton);
    const bool bWantsToErase = InViewport->KeyState(EKeys::LeftControl) || InViewport->KeyState(EKeys::RightControl);

    const bool bIsLeftButtonDown = bEventIsLeftButtonDown || bKeystateIsLeftButtonDown;
    if (InViewportClient->EngineShowFlags.ModeWidgets) {
        const bool bUserWantsPaint = bIsLeftButtonDown;
        bIsPainting = bUserWantsPaint;

        // check paint mode?
        if (bWantsToErase) {
            UISettings.SetPaintMode(EGridMapPaintMode::Erase);
        } else {
            UISettings.SetPaintMode(EGridMapPaintMode::Paint);
        }

        if (bUserWantsPaint) {
            bHandled = true;
            PaintTile();
        }
    }

    if (!bHandled) {
        bHandled = FEdMode::InputKey(InViewportClient, InViewport, InKey, InEvent);
    }

    return bHandled;
}

bool FGridMapEditorMode::HandleClick(FEditorViewportClient *InViewportClient, HHitProxy *HitProxy,
                                     const FViewportClick &Click) {
    return true;
}

void FGridMapEditorMode::GridMapBrushTrace(FEditorViewportClient *ViewportClient, const FVector &InRayOrigin,
                                           const FVector &InRayDirection) {
    bBrushTraceValid = false;
    BrushTraceHitActor.Reset();

    if (ViewportClient == nullptr || (!ViewportClient->IsMovingCamera() && ViewportClient->IsVisible())) {
        if (UISettings.GetPaintToolSelected()) {
            const FPlane GroundPlane(UISettings.GetPaintOrigin() + FVector(0, 0, UISettings.GetPaintHeight()),
                                     FVector::UpVector);

            FVector IntersectionLocation = FMath::RayPlaneIntersection(InRayOrigin, InRayDirection, GroundPlane);
            BrushLocation = SnapLocation(IntersectionLocation);
            bBrushTraceValid = true;

            TArray<AGridMapStaticMeshActor *> HitTiles;
            TilesAt(GetWorld(), BrushLocation, UISettings.GetPaintLayer(), HitTiles);
            if (HitTiles.Num() > 0) {
                BrushTraceHitActor = HitTiles[0];
            }
        }
    }
}

FVector FGridMapEditorMode::SnapLocation(const FVector &InLocation) {
    int32 SnapWidth = GetTileSize();
    if (SnapWidth <= 0) {
        return InLocation;
    }
    float X = InLocation.X / SnapWidth;
    float Y = InLocation.Y / SnapWidth;
    float Z = InLocation.Z / SnapWidth;

    X = FMath::RoundToInt(X) * SnapWidth;
    Y = FMath::RoundToInt(Y) * SnapWidth;
    Z = FMath::RoundToInt(Z) * SnapWidth;
    return FVector(X, Y, Z);
}

bool FGridMapEditorMode::Select(AActor *InActor, bool bInSelected) {
    // return true if you filter that selection
    // however - return false if we are trying to deselect so that it will infact do the deselection
    if (bInSelected == false) {
        return false;
    }
    return true;
}

bool FGridMapEditorMode::IsSelectionAllowed(AActor *InActor, bool bInSelection) const {
    return false;
}

void FGridMapEditorMode::ClearAllToolSelection() {
    UISettings.SetPaintToolSelected(false);
    UISettings.SetSelectToolSelected(false);
    UISettings.SetSettingsToolSelected(false);
}

void FGridMapEditorMode::OnSetPaintTiles() {
    ClearAllToolSelection();
    UISettings.SetPaintToolSelected(true);
    // HandleToolChanged();
}

void FGridMapEditorMode::OnSetSelectTiles() {
    ClearAllToolSelection();
    UISettings.SetSelectToolSelected(true);
}

void FGridMapEditorMode::OnSetTileSettings() {
    ClearAllToolSelection();
    UISettings.SetSettingsToolSelected(true);
}

EGridMapEditingState FGridMapEditorMode::GetEditingState() const {
    UWorld *World = GetWorld();

    if (GEditor->bIsSimulatingInEditor) {
        return EGridMapEditingState::SIEWorld;
    } else if (GEditor->PlayWorld != NULL) {
        return EGridMapEditingState::PIEWorld;
    } else if (World == nullptr) {
        return EGridMapEditingState::Unknown;
    }

    return EGridMapEditingState::Enabled;
}

uint32 FGridMapEditorMode::GetTileAdjacencyBitmask(UWorld *World, const FVector &Origin, UGridMapTileSet *TileSet,
                                                   int32 Layer) const {
    uint32 bitmask = 0;

    TArray<FAdjacentTile> AdjacentTiles;
    if (GetAdjacentTiles(World, Origin, AdjacentTiles, Layer, TileSet->bMatchesEmpty)) {
        for (const FAdjacentTile &AdjacentTile : AdjacentTiles) {
            if (AdjacentTile.Key &&
                TileSet->AdjacencyTagRequirements.RequirementsMet(AdjacentTile.Key->TileSet->TileTags))
                bitmask |= AdjacentTile.Value;
            else if (AdjacentTile.Key == nullptr) {
                bitmask |= AdjacentTile.Value;
            }
        }
    }

    return bitmask;
}

bool FGridMapEditorMode::TilesAt(UWorld *World, const FVector &Origin, int32 Layer,
                                 TArray<AGridMapStaticMeshActor *> &OutTiles) const {
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    TArray<AActor *> ActorsToIgnore;
    TArray<AActor *> OutActors;

    auto TileSize = GetTileSize();
    auto TileHeight = GetTileHeight();
    auto Scaled = Retro::Optionals::OfNullable(ActiveTileSet) |
                  Retro::Optionals::Transform([](const UGridMapTileSet &M) { return FUintVector(M.SizeX, M.SizeY, M.SizeZ); }) |
                  Retro::Optionals::OrElseValue(FUintVector(1));
    auto ScaledGridWidth = static_cast<int32>(TileSize * Scaled.X * .95f / 2.f);
    auto ScaledGridLength = static_cast<int32>(TileSize * Scaled.Y * .95f / 2.f);
    auto ScaledGridHeight = static_cast<int32>(TileHeight * Scaled.Z / 2.f);

    FVector OriginOffset((Scaled.X - 1) * TileSize / 2, (Scaled.Y - 1) * TileSize / 2, (Scaled.Z - 1) * TileHeight / 2);
    if (UKismetSystemLibrary::BoxOverlapActors(
            World, Origin + OriginOffset, FVector(ScaledGridWidth, ScaledGridLength, ScaledGridHeight), ObjectTypes,
            AGridMapStaticMeshActor::StaticClass(), ActorsToIgnore, OutActors)) {
        OutTiles.Reserve(OutActors.Num());
        for (AActor *OutActor : OutActors) {
            if (auto TileMesh = Cast<AGridMapStaticMeshActor>(OutActor);
                TileMesh != nullptr && TileMesh->CurrentLayer == Layer)
                OutTiles.Add(TileMesh);
        }
    }

    return OutTiles.Num() > 0;
}

void FGridMapEditorMode::UpdateAllTiles() {
    TArray<FAdjacentTile> AllTiles;

    for (TActorIterator<AGridMapStaticMeshActor> It(GetWorld(), AGridMapStaticMeshActor::StaticClass()); It; ++It) {
        AGridMapStaticMeshActor *Actor = *It;
        if (IsValid(Actor)) {
            AllTiles.Add(FAdjacentTile(Actor, 0));
        }
    }

    UpdateAdjacentTiles(GetWorld(), AllTiles);
}

void FGridMapEditorMode::UpdateAdjacentTiles(UWorld *World, const TArray<FAdjacentTile> &RootActors) {
    TArray<FAdjacentTile> AdjacentTiles;
    TArray<AGridMapStaticMeshActor *> ProcessedActors;
    TQueue<AGridMapStaticMeshActor *> QueuedActors;

    for (const FAdjacentTile &RootTile : RootActors) {
        QueuedActors.Enqueue(RootTile.Key);
    }

    while (!QueuedActors.IsEmpty()) {
        AGridMapStaticMeshActor *CurrentActor = nullptr;
        QueuedActors.Dequeue(CurrentActor);

        // have we already been processed?
        if (ProcessedActors.Contains(CurrentActor))
            continue;

        UGridMapTileSet *TileSet = CurrentActor->TileSet;
        uint32 Adjacency =
            GetTileAdjacencyBitmask(GetWorld(), CurrentActor->GetActorLocation(), TileSet, UISettings.GetPaintLayer());
        auto TileList = TileSet->FindTilesForAdjacency(Adjacency);
        if (!TileList.IsSet()) {
            GEngine->AddOnScreenDebugMessage(INDEX_NONE, 4.0f, FColor::Red, TEXT("Failed to find tile!"), true,
                                             FVector2D::UnitVector);
            ::DrawDebugPoint(GetWorld(), CurrentActor->GetActorLocation(), 12, FColor::Red, false, 10.f);
            continue;
        }
        UStaticMesh *ExpectedStaticMesh = TileList->GetRandomTile().LoadSynchronous();
        UStaticMesh *CurrentStaticMesh = CurrentActor->GetStaticMeshComponent()->GetStaticMesh();

        // it hasn't changed
        if (CurrentStaticMesh == ExpectedStaticMesh)
            continue;

        if (UISettings.GetDebugDrawTiles()) {
            DrawDebugPoint(GetWorld(), CurrentActor->GetActorLocation(), 10.f, FColor::Yellow, false, 5.0f, 255);
        }

        // it has changed, so let's update it
        CurrentActor->GetStaticMeshComponent()->SetStaticMesh(ExpectedStaticMesh);
        CurrentActor->SetActorRotation(TileList->Rotation);
        ProcessedActors.Add(CurrentActor);

        // find any neighbors that haven't been updated, and queue them for an update
        if (GetAdjacentTiles(World, CurrentActor->GetActorLocation(), AdjacentTiles, UISettings.GetPaintLayer())) {
            for (const FAdjacentTile &AdjacentTile : AdjacentTiles) {
                if (AdjacentTile.Key && !ProcessedActors.Contains(AdjacentTile.Key)) {
                    QueuedActors.Enqueue(AdjacentTile.Key);
                }
            }
        }
    }
}

bool FGridMapEditorMode::GetAdjacentTiles(UWorld *World, const FVector &Origin,
                                          TArray<TPair<AGridMapStaticMeshActor *, uint32>> &OutAdjacentTiles,
                                          int32 Layer, bool bIncludeEmptyTiles) const {
    static const int TileCount = 8;

    static FVector Offsets[TileCount]{
        FVector(0, -GetTileSize(), 0), // top-center
        FVector(-GetTileSize(), 0, 0), // center-left
        FVector(GetTileSize(), 0, 0),  // center-right
        FVector(0, GetTileSize(), 0),  // botton-center

        FVector(-GetTileSize(), -GetTileSize(), 0), // top left
        FVector(GetTileSize(), -GetTileSize(), 0),  // top right
        FVector(-GetTileSize(), GetTileSize(), 0),  // bottom left
        FVector(GetTileSize(), GetTileSize(), 0),   // bottom right
    };

    static uint32 Bits[TileCount]{
        1 << 0, // top
        1 << 1, // left
        1 << 2, // right
        1 << 3, // bottom

        1 << 4, // top left
        1 << 5, // top right
        1 << 6, // bottom left
        1 << 7, // bottom right
    };

    TArray<AGridMapStaticMeshActor *> Tiles;
    for (int i = 0; i < TileCount; ++i) {
        if (TilesAt(World, Origin + Offsets[i], Layer, Tiles)) {
            for (AGridMapStaticMeshActor *Tile : Tiles) {
                OutAdjacentTiles.Add(TPair<AGridMapStaticMeshActor *, uint32>(Tile, Bits[i]));
            }
            Tiles.Empty();
        } else if (bIncludeEmptyTiles) {
            OutAdjacentTiles.Add(TPair<AGridMapStaticMeshActor *, uint32>(nullptr, Bits[i]));
        }
    }

    return OutAdjacentTiles.Num() > 0;
}

int32 FGridMapEditorMode::GetTileSize() const {
    if (ActiveTileSet)
        return ActiveTileSet->TileSize;

    // fallback on the editor grid size
    return GEditor->GetGridSize();
}

int32 FGridMapEditorMode::GetTileHeight() const {
    if (ActiveTileSet)
        return ActiveTileSet->TileHeight;

    return GEditor->GetGridSize();
}

void FGridMapEditorMode::AddActiveTileSet(UGridMapTileSet *TileSet) {
    // don't add duplicates
    for (const UGridMapTileSet *ExistingTileSet : ActiveTileSets) {
        if (ExistingTileSet->GetName() == TileSet->GetName())
            return;
    }

    ActiveTileSets.Add(TileSet);
}

const TArray<UGridMapTileSet *> &FGridMapEditorMode::GetActiveTileSets() const {
    return ActiveTileSets;
}

void FGridMapEditorMode::SetActiveTileSet(UGridMapTileSet *TileSet) {
    ActiveTileSet = TileSet;
    for (UGridMapTileSet *ExistingTileSet : ActiveTileSets) {
        if (TileSet != nullptr && ExistingTileSet->GetName() == TileSet->GetName()) {
            ActiveTileSet = ExistingTileSet;
        }
    }

    UISettings.SetCurrentTileSet(ActiveTileSet);
}

FString FGridMapEditorMode::CreateActorLabel(const class UGridMapTileSet *TileSet) const {
    FString TileSetName = TileSet->GetName();
    FString CleanTileSetName = TileSetName.StartsWith("TS_") ? TileSetName.RightChop(3) : TileSetName;
    return FString::Printf(TEXT("SM_%s"), *CleanTileSetName);
}