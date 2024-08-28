// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "Tilemap/Tilemap3D.h"

#include "Tilemap3DEditorViewportWidget.generated.h"

class ATilemap3D;
class STilemap3DEditorViewport;

/**
 * UMG widget that wraps the slate viewport widget.
 */
UCLASS(BlueprintType)
class TILEMAP3DEDITOR_API UTilemap3DEditorViewportWidget : public UWidget {
    GENERATED_BODY()

  public:
    TSharedRef<SWidget> RebuildWidget() override;
    void SynchronizeProperties() override;
    void ReleaseSlateResources(bool bReleaseChildren) override;

    void SetTilemap(ATilemap3D *Tilemap3D);
    void SetTile(const FTileHandle &Handle);

  private:
    UPROPERTY()
    TObjectPtr<ATilemap3D> TilemapActor;

    UPROPERTY()
    FTileHandle PaintTile;

    TSharedPtr<STilemap3DEditorViewport> TilemapWidget;
};
