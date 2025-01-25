// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "SpriteDrawCall.h"
#include "UObject/Object.h"
#include "SpriteEditorOnlyTypes.h"
#include "SimpleFlipbook.generated.h"

namespace EFlipbookCollisionMode {
    enum Type : int;
}

USTRUCT()
struct SIMPLE2D_API FSimpleFlipbookDrawCall : public FSpriteDrawCallRecord {
    GENERATED_BODY()

    UPROPERTY()
    int32 Rows = 1;

    UPROPERTY()
    int32 Columns = 1;

    UPROPERTY()
    int32 FrameNumber = 0;
};

USTRUCT(BlueprintType)
struct SIMPLE2D_API FSimpleFlipbookKeyFrame {
    GENERATED_BODY()

    UPROPERTY(Category=Sprite, EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0))
    int32 Index = 0;

    UPROPERTY(Category=Sprite, EditAnywhere, BlueprintReadOnly, meta=(ClampMin=1))
    int32 FrameRun = 1;
    
};

namespace Simple2D {
    class FScopedSimpleFlipbookMutator;
#if WITH_EDITOR
    class FSimpleFlipbookDetailsCustomization;
#endif
}

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

    UFUNCTION(BlueprintCallable, Category="Sprite")
    const FSimpleFlipbookKeyFrame &GetKeyFrameChecked(int32 Index) const {
        return KeyFrames[Index];
    }

    UFUNCTION(BlueprintCallable, Category="Sprite")
    bool IsValidKeyFrameIndex(int32 Index) const {
        return KeyFrames.IsValidIndex(Index);
    }

    FBoxSphereBounds GetRenderBounds() const;

    float GetPixelsPerUnrealUnit() const { return PixelsPerUnrealUnit; }

    // Return the scaling factor between Unreal units (cm) and pixels
    float GetUnrealUnitsPerPixel() const { return 1.0f / PixelsPerUnrealUnit; }

    // Returns the raw pivot position (ignoring pixel snapping)
    FVector2D GetRawPivotPosition() const;

    // Returns the current pivot position in texture space
    FVector2D GetPivotPosition() const;

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    UMaterialInterface* GetDefaultMaterial() const {
        return DefaultMaterial;
    }

    FAdditionalSpriteTextureArray GetBakedAdditionalSourceTextures() const;

    FSimpleFlipbookDrawCall CreateDrawCallRecord(int32 FrameIndex) const;

    int32 GetAlternateMaterialSplitIndex() const {
        return AlternateMaterialSplitIndex;
    }

    void InvalidateCachedData();

#if WITH_EDITOR
    void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

    void RebuildRenderData();

    FVector2D ConvertTextureSpaceToPivotSpace(FVector2D Input) const;
    
    void CreatePolygonFromBoundingBox(FSpriteGeometryCollection& GeomOwner) const;
#endif

private:
#if WITH_EDITOR
    friend Simple2D::FSimpleFlipbookDetailsCustomization;
#endif
    friend Simple2D::FScopedSimpleFlipbookMutator;
    friend class USimpleFlipbookThumbnailRenderer;
    
    UPROPERTY(EditAnywhere, BlueprintGetter = GetSourceTexture, Category = Sprite)
    TObjectPtr<UTexture2D> SourceTexture;

    UPROPERTY(Category=Sprite, EditAnywhere, AssetRegistrySearchable, meta=(DisplayName="Additional Textures"))
    TArray<TObjectPtr<UTexture>> AdditionalSourceTextures;

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

    // The scaling factor between pixels and Unreal units (cm) (e.g., 0.64 would make a 64 pixel wide sprite take up 100 cm)
    UPROPERTY(Category=Sprite, EditAnywhere, meta = (DisplayName = "Pixels per unit"))
    float PixelsPerUnrealUnit = 1.f;

#if WITH_EDITORONLY_DATA
    // Pivot mode
    UPROPERTY(Category=Sprite, EditAnywhere)
    TEnumAsByte<ESpritePivotMode::Type> PivotMode = ESpritePivotMode::Center_Center;

    // Custom pivot point (relative to the sprite rectangle)
    UPROPERTY(Category=Sprite, EditAnywhere)
    FVector2D CustomPivotPoint;

    // Should the pivot be snapped to a pixel boundary?
    UPROPERTY(Category=Sprite, EditAnywhere, AdvancedDisplay)
    bool bSnapPivotToPixelGrid = true;

    UPROPERTY()
    FSpriteGeometryCollection RenderGeometry;
#endif

    UPROPERTY()
    int32 AlternateMaterialSplitIndex = INDEX_NONE;

    // Baked render data (triangle vertices, stored as XY UV tuples)
    //   XY is the XZ position in world space, relative to the pivot
    //   UV is normalized (0..1)
    //   There should always be a multiple of three elements in this array
    UPROPERTY()
    TArray<FVector4> BakedRenderData;

};

namespace Simple2D {
    class FScopedSimpleFlipbookMutator
    {
    public:
        float& FramesPerSecond;
        TArray<FSimpleFlipbookKeyFrame>& KeyFrames;

    private:
        USimpleFlipbook* SourceFlipbook;

    public:
        explicit FScopedSimpleFlipbookMutator(USimpleFlipbook* InFlipbook)
            : FramesPerSecond(InFlipbook->FramesPerSecond)
            , KeyFrames(InFlipbook->KeyFrames)
            , SourceFlipbook(InFlipbook)
        {
        }

        ~FScopedSimpleFlipbookMutator()
        {
            InvalidateCachedData();
        }

        UE_NONCOPYABLE(FScopedSimpleFlipbookMutator)

        void InvalidateCachedData()
        {
            SourceFlipbook->InvalidateCachedData();
        }

        USimpleFlipbook* GetSourceFlipbook() const
        {
            return SourceFlipbook;
        }
    };
}