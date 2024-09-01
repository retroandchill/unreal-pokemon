// "Unreal Pokémon" created by Retro & Chill.

#include "TileMeshThumbnailScene.h"
#include "Engine/StaticMeshActor.h"
#include "ThumbnailRendering/SceneThumbnailInfo.h"

FTileMeshThumbnailScene::FTileMeshThumbnailScene() {
    bForceAllUsedMipsResident = false;

    // Create preview actor
    // checked
    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnInfo.bNoFail = true;
    SpawnInfo.ObjectFlags = RF_Transient;
    PreviewActor = GetWorld()->SpawnActor<AStaticMeshActor>(SpawnInfo);

    PreviewActor->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
    PreviewActor->SetActorEnableCollision(false);
}

void FTileMeshThumbnailScene::SetStaticMesh(UStaticMesh *StaticMesh) {
    PreviewActor->GetStaticMeshComponent()->SetStaticMesh(StaticMesh);

    if (StaticMesh != nullptr) {
        PreviewActor->SetActorLocation(FVector(0, 0, 0), false);

        // Force LOD 0
        PreviewActor->GetStaticMeshComponent()->ForcedLodModel = 1;

        PreviewActor->GetStaticMeshComponent()->UpdateBounds();

        // Center the mesh at the world origin then offset to put it on top of the plane
        const float BoundsZOffset = GetBoundsZOffset(PreviewActor->GetStaticMeshComponent()->Bounds);
        PreviewActor->SetActorLocation(
            -PreviewActor->GetStaticMeshComponent()->Bounds.Origin + FVector(0, 0, BoundsZOffset), false);
    }

    PreviewActor->GetStaticMeshComponent()->RecreateRenderState_Concurrent();
}

void FTileMeshThumbnailScene::SetOverrideMaterials(const TArray<UMaterialInterface *> &OverrideMaterials) {
    PreviewActor->GetStaticMeshComponent()->OverrideMaterials = OverrideMaterials;
    PreviewActor->GetStaticMeshComponent()->MarkRenderStateDirty();
}

void FTileMeshThumbnailScene::SetActorRotation(const FRotator &Rotation) {
    PreviewActor->SetActorRotation(Rotation);
}

void FTileMeshThumbnailScene::GetViewMatrixParameters(const float InFOVDegrees, FVector &OutOrigin,
                                                      float &OutOrbitPitch, float &OutOrbitYaw,
                                                      float &OutOrbitZoom) const {
    check(PreviewActor)
    ;
    check(PreviewActor->GetStaticMeshComponent())
    ;
    check(PreviewActor->GetStaticMeshComponent()->GetStaticMesh())
    ;

    const float HalfFOVRadians = FMath::DegreesToRadians<float>(InFOVDegrees) * 0.5f;
    // Add extra size to view slightly outside of the sphere to compensate for perspective
    const float HalfMeshSize = static_cast<float>(PreviewActor->GetStaticMeshComponent()->Bounds.SphereRadius * 1.15);
    const float BoundsZOffset = GetBoundsZOffset(PreviewActor->GetStaticMeshComponent()->Bounds);
    const float TargetDistance = HalfMeshSize / FMath::Tan(HalfFOVRadians);

    auto ThumbnailInfo =
        Cast<USceneThumbnailInfo>(PreviewActor->GetStaticMeshComponent()->GetStaticMesh()->ThumbnailInfo);
    if (ThumbnailInfo != nullptr) {
        if (TargetDistance + ThumbnailInfo->OrbitZoom < 0) {
            ThumbnailInfo->OrbitZoom = -TargetDistance;
        }
    } else {
        ThumbnailInfo = USceneThumbnailInfo::StaticClass()->GetDefaultObject<USceneThumbnailInfo>();
    }

    OutOrigin = FVector(0, 0, -BoundsZOffset);
    OutOrbitPitch = -90;
    OutOrbitYaw = 180;
    OutOrbitZoom = TargetDistance + ThumbnailInfo->OrbitZoom;
}