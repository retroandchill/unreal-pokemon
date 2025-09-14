// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "ThumbnailHelpers.h"

/**
 *
 */
class GRIDMAPEDITOR_API FTileMeshThumbnailScene : public FThumbnailPreviewScene
{
  public:
    /**
     * Constructor
     *
     */
    FTileMeshThumbnailScene();

    /** Sets the static mesh to use in the next CreateView() */
    void SetStaticMesh(class UStaticMesh *StaticMesh);

    /** Sets override materials for the static mesh  */
    void SetOverrideMaterials(const TArray<class UMaterialInterface *> &OverrideMaterials);

    void SetActorRotation(const FRotator &Rotation);

  protected:
    void GetViewMatrixParameters(const float InFOVDegrees, FVector &OutOrigin, float &OutOrbitPitch, float &OutOrbitYaw,
                                 float &OutOrbitZoom) const override;

  private:
    /** The static mesh actor used to display all static mesh thumbnails */
    TObjectPtr<AStaticMeshActor> PreviewActor;
};
