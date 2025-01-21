// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "PaperSpriteAtlas.h"
#include "UObject/Object.h"
#include "SpriteEditorOnlyTypes.h"
#include "SimpleFlipbook.generated.h"

namespace EFlipbookCollisionMode {
    enum Type : int;
}

class UPaperSprite;

USTRUCT()
struct FSimpleFlipbookKeyFrame {
    GENERATED_BODY()

    UPROPERTY(Category=Sprite, EditAnywhere, meta=(ClampMin=0))
    int32 Index = 0;

    UPROPERTY(Category=Sprite, EditAnywhere, meta=(ClampMin=1))
    int32 FrameRun = 1;
    
};

/**
 * 
 */
UCLASS(BlueprintType)
class SIMPLE2D_API USimpleFlipbook : public UObject {
    GENERATED_BODY()

public:
    USimpleFlipbook();

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    UTexture2D* GetSourceTexture() const {
        return SourceTexture;
    }

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    TEnumAsByte<EFlipbookCollisionMode::Type> GetCollisionSource() const { return CollisionSource; }

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    int32 GetNumFrames() const;

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    int32 GetRows() const {
        return Rows;
    }

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    int32 GetColumns() const {
        return Columns;
    }

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    float GetFramesPerSecond() const {
        return FramesPerSecond;
    }

    UFUNCTION(BlueprintCallable, Category="Sprite")
    int32 GetNumKeyFrames() const {
        return KeyFrames.Num();
    }

    UFUNCTION(BlueprintCallable, Category="Sprite")
    float GetTotalDuration() const;

    UFUNCTION(BlueprintCallable, Category="Sprite")
    int32 GetKeyFrameIndexAtTime(float Time, bool bClampToEnds = false) const;

    const FSimpleFlipbookKeyFrame &GetKeyFrameChecked(int32 Index) const {
        return KeyFrames[Index];
    }

    UFUNCTION(BlueprintCallable, Category="Sprite")
    bool IsValidKeyFrameIndex(int32 Index) const {
        return KeyFrames.IsValidIndex(Index);
    }

    FBoxSphereBounds GetRenderBounds() const;

    // Return the scaling factor between pixels and Unreal units (cm)
    float GetPixelsPerUnrealUnit() const { return PixelsPerUnrealUnit; }

    // Return the scaling factor between Unreal units (cm) and pixels
    float GetUnrealUnitsPerPixel() const { return 1.0f / PixelsPerUnrealUnit; }

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    UMaterialInterface* GetDefaultMaterial() const {
        return DefaultMaterial;
    }
    
    // Returns the texture this should be rendered with
    UTexture2D* GetBakedTexture() const;

#if WITH_EDITOR
    FVector2D GetPivotPosition() const;

    FVector2D GetRawPivotPosition() const;
    
    void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

    void RebuildRenderData();

    FVector2D ConvertTextureSpaceToPivotSpace(FVector2D Input) const;
    
    void CreatePolygonFromBoundingBox(FSpriteGeometryCollection& GeomOwner) const;
    void BuildGeometryFromContours(FSpriteGeometryCollection& GeomOwner);
    void FindTextureBoundingBox(float AlphaThreshold, FVector2D& OutBoxPosition, FVector2D& OutBoxSize);

    static void FindContours(const FIntPoint& ScanPos, const FIntPoint& ScanSize, float AlphaThreshold, float Detail, UTexture2D* Texture, TArray< TArray<FIntPoint> >& OutPoints);
#endif

private:
    UPROPERTY(EditAnywhere, BlueprintGetter = GetSourceTexture, Category = Sprite)
    TObjectPtr<UTexture2D> SourceTexture;

    UPROPERTY(EditAnywhere, BlueprintGetter = GetCollisionSource, Category=Sprite, meta = (InvalidEnumValues = EachFrameCollision))
    TEnumAsByte<EFlipbookCollisionMode::Type> CollisionSource;

    UPROPERTY(EditAnywhere, Category = Sprite, meta = (ClamMin = 1, UIMin = 1))
    TArray<FSimpleFlipbookKeyFrame> KeyFrames;
    
    UPROPERTY(EditAnywhere, BlueprintGetter = GetRows, Category = Sprite, meta = (ClamMin = 1, UIMin = 1))
    int32 Rows = 1;

    UPROPERTY(EditAnywhere, BlueprintGetter = GetColumns, Category = Sprite, meta = (ClamMin = 1, UIMin = 1))
    int32 Columns = 1;

    UPROPERTY(EditAnywhere, BlueprintGetter = GetFramesPerSecond, Category = Sprite, meta=(ClampMin=0, ClampMax=1000))
    float FramesPerSecond = 15.f;
    
    UPROPERTY(EditAnywhere, BlueprintGetter = GetDefaultMaterial, Category = Sprite)
    TObjectPtr<UMaterialInterface> DefaultMaterial;

#if WITH_EDITORONLY_DATA
    // Pivot mode
    UPROPERTY(Category=Sprite, EditAnywhere)
    TEnumAsByte<ESpritePivotMode::Type> PivotMode;

    // Custom pivot point (relative to the sprite rectangle)
    UPROPERTY(Category=Sprite, EditAnywhere)
    FVector2D CustomPivotPoint;

    // Should the pivot be snapped to a pixel boundary?
    UPROPERTY(Category=Sprite, EditAnywhere, AdvancedDisplay)
    bool bSnapPivotToPixelGrid;

    // Custom collision geometry polygons (in texture space)
    UPROPERTY(Category=Collision, EditAnywhere)
    FSpriteGeometryCollection CollisionGeometry;

    // The extrusion thickness of collision geometry when using a 3D collision domain
    UPROPERTY(Category=Collision, EditAnywhere)
    float CollisionThickness;

    // Custom render geometry polygons (in texture space)
    UPROPERTY(Category=Rendering, EditAnywhere)
    FSpriteGeometryCollection RenderGeometry;

    // Spritesheet group that this sprite belongs to
    UPROPERTY(Category=Sprite, EditAnywhere, AssetRegistrySearchable)
    TObjectPtr<class UPaperSpriteAtlas> AtlasGroup;

    // The previous spritesheet group this belonged to
    // To make sure we remove ourselves from it if changed or nulled out
    TSoftObjectPtr<class UPaperSpriteAtlas> PreviousAtlasGroup;

#endif
    // The point at which the alternate material takes over in the baked render data (or INDEX_NONE)
    UPROPERTY()
    int32 AlternateMaterialSplitIndex;

    UPROPERTY()
    TObjectPtr<UTexture2D> BakedSourceTexture;
    
    UPROPERTY()
    FVector2D BakedSourceUV;

    // Baked render data (triangle vertices, stored as XY UV tuples)
    //   XY is the XZ position in world space, relative to the pivot
    //   UV is normalized (0..1)
    //   There should always be a multiple of three elements in this array
    UPROPERTY()
    TArray<FVector4> BakedRenderData;
};
