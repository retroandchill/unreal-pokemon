// "Unreal Pokémon" created by Retro & Chill.

#include "Simple2D/Assets/SimpleFlipbook.h"
#include "AlphaBitmap.h"
#include "Materials/MaterialInterface.h"
#include "Paper2DModule.h"
#include "PaperSprite.h"
#include "UObject/ConstructorHelpers.h"

USimpleFlipbook::USimpleFlipbook() {
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaskedMaterialRef(
        TEXT("/Simple2D/Materials/MaskedUnlitFlipbookMaterial"));
    DefaultMaterial = MaskedMaterialRef.Object;
}

int32 USimpleFlipbook::GetNumFrames() const {
    int32 SumFrames = 0;
    for (int32 KeyFrameIndex = 0; KeyFrameIndex < KeyFrames.Num(); ++KeyFrameIndex) {
        SumFrames += KeyFrames[KeyFrameIndex].FrameRun;
    }

    return SumFrames;
}

float USimpleFlipbook::GetTotalDuration() const {
    if (FramesPerSecond != 0) {
        return static_cast<float>(GetNumFrames()) / FramesPerSecond;
    }

    return 0.0f;
}

int32 USimpleFlipbook::GetKeyFrameIndexAtTime(float Time, bool bClampToEnds) const {
    if (Time < 0.0f && !bClampToEnds) {
        return INDEX_NONE;
    }

    if (FramesPerSecond > 0.0f) {
        float SumTime = 0.0f;

        for (int32 KeyFrameIndex = 0; KeyFrameIndex < KeyFrames.Num(); ++KeyFrameIndex) {
            SumTime += static_cast<float>(KeyFrames[KeyFrameIndex].FrameRun) / FramesPerSecond;

            if (Time <= SumTime) {
                return KeyFrameIndex;
            }
        }

        // Return the last frame (note: relies on INDEX_NONE = -1 if there are no key frames)
        return KeyFrames.Num() - 1;
    }

    return KeyFrames.Num() > 0 ? 0 : INDEX_NONE;
}

FBoxSphereBounds USimpleFlipbook::GetRenderBounds() const {
    FBox BoundingBox(ForceInit);

    for (int32 VertexIndex = 0; VertexIndex < BakedRenderData.Num(); ++VertexIndex) {
        const FVector4 &VertXYUV = BakedRenderData[VertexIndex];
        const FVector Vert((PaperAxisX * VertXYUV.X) + (PaperAxisY * VertXYUV.Y));
        BoundingBox += Vert;
    }

    // Make the whole thing a single unit 'deep'
    const FVector HalfThicknessVector = 0.5f * PaperAxisZ;
    BoundingBox += -HalfThicknessVector;
    BoundingBox += HalfThicknessVector;

    return FBoxSphereBounds(BoundingBox);
}

#if WITH_EDITOR
FVector2D USimpleFlipbook::GetRawPivotPosition() const {
    FVector2D TopLeftUV = FVector2D::ZeroVector;
    FVector2D Dimension = SourceTexture != nullptr
                              ? FVector2D(SourceTexture->GetSizeX() / Columns, SourceTexture->GetSizeY() / Rows)
                              : FVector2D::ZeroVector;
    switch (PivotMode) {
    case ESpritePivotMode::Top_Left:
        return TopLeftUV;
    case ESpritePivotMode::Top_Center:
        return FVector2D(TopLeftUV.X + Dimension.X * 0.5f, TopLeftUV.Y);
    case ESpritePivotMode::Top_Right:
        return FVector2D(TopLeftUV.X + Dimension.X, TopLeftUV.Y);
    case ESpritePivotMode::Center_Left:
        return FVector2D(TopLeftUV.X, TopLeftUV.Y + Dimension.Y * 0.5f);
    case ESpritePivotMode::Center_Center:
        return FVector2D(TopLeftUV.X + Dimension.X * 0.5f, TopLeftUV.Y + Dimension.Y * 0.5f);
    case ESpritePivotMode::Center_Right:
        return FVector2D(TopLeftUV.X + Dimension.X, TopLeftUV.Y + Dimension.Y * 0.5f);
    case ESpritePivotMode::Bottom_Left:
        return FVector2D(TopLeftUV.X, TopLeftUV.Y + Dimension.Y);
    case ESpritePivotMode::Bottom_Center:
        return FVector2D(TopLeftUV.X + Dimension.X * 0.5f, TopLeftUV.Y + Dimension.Y);
    case ESpritePivotMode::Bottom_Right:
        return TopLeftUV + Dimension;
    case ESpritePivotMode::Custom:
    default:
        return CustomPivotPoint;
    }
}

FVector2D USimpleFlipbook::GetPivotPosition() const {
    FVector2D RawPivot = GetRawPivotPosition();

    if (bSnapPivotToPixelGrid) {
        RawPivot.X = FMath::RoundToFloat(RawPivot.X);
        RawPivot.Y = FMath::RoundToFloat(RawPivot.Y);
    }

    return RawPivot;
}
#endif

FAdditionalSpriteTextureArray USimpleFlipbook::GetBakedAdditionalSourceTextures() const {
    return FAdditionalSpriteTextureArray(AdditionalSourceTextures);
}

FSimpleFlipbookDrawCall USimpleFlipbook::CreateDrawCallRecord(int32 FrameIndex) const {
    FSimpleFlipbookDrawCall DrawCall;

    DrawCall.Destination = FVector::ZeroVector;
    DrawCall.BaseTexture = GetSourceTexture();
    DrawCall.AdditionalTextures = GetBakedAdditionalSourceTextures();
    DrawCall.Color = FColor::White;
    DrawCall.RenderVerts = BakedRenderData;
    DrawCall.Columns = Columns;
    DrawCall.Rows = Rows;
    DrawCall.FrameNumber = GetKeyFrameChecked(FrameIndex).Index;

    return DrawCall;
}

void USimpleFlipbook::InvalidateCachedData() {
    // No cached data yet, but the functions that currently have to iterate over all frames can use cached data in the
    // future
}

#if WITH_EDITOR
void USimpleFlipbook::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) {
    if (PixelsPerUnrealUnit <= 0.0f) {
        PixelsPerUnrealUnit = 1.0f;
    }

    if (FramesPerSecond < 0.0f) {
        FramesPerSecond = 0.0f;
    }

    CustomPivotPoint = GetPivotPosition();

    UObject::PostEditChangeProperty(PropertyChangedEvent);

    RebuildRenderData();
}

void USimpleFlipbook::RebuildRenderData() {
    FSpriteGeometryCollection AlternateGeometry;

    CreatePolygonFromBoundingBox(RenderGeometry);

    // Determine the texture size
    UTexture2D *EffectiveTexture = GetSourceTexture();

    FVector2D TextureSize(1.0f, 1.0f);
    if (EffectiveTexture) {
        EffectiveTexture->ConditionalPostLoad();
        TextureSize = FVector2D(EffectiveTexture->GetImportedSize());
    }
    double InverseWidth = static_cast<double>(Columns) / TextureSize.X;
    double InverseHeight = static_cast<double>(Rows) / TextureSize.Y;

    // Adjust for the pivot and store in the baked geometry buffer
    const float UnitsPerPixel = GetUnrealUnitsPerPixel();

    // Triangulate the render geometry
    TArray<FVector2D> TriangluatedPoints;
    RenderGeometry.Triangulate(/*out*/ TriangluatedPoints, /*bIncludeBoxes=*/true);

    // Triangulate the alternate render geometry, if present
    if (AlternateGeometry.Shapes.Num() > 0) {
        TArray<FVector2D> AlternateTriangluatedPoints;
        AlternateGeometry.Triangulate(/*out*/ AlternateTriangluatedPoints, /*bIncludeBoxes=*/true);

        AlternateMaterialSplitIndex = TriangluatedPoints.Num();
        TriangluatedPoints.Append(AlternateTriangluatedPoints);
        RenderGeometry.Shapes.Append(AlternateGeometry.Shapes);
    } else {
        AlternateMaterialSplitIndex = INDEX_NONE;
    }

    // Bake the verts
    BakedRenderData.Empty(TriangluatedPoints.Num());
    for (int32 PointIndex = 0; PointIndex < TriangluatedPoints.Num(); ++PointIndex) {
        const FVector2D &SourcePos = TriangluatedPoints[PointIndex];

        const FVector2D PivotSpacePos = ConvertTextureSpaceToPivotSpace(SourcePos);
        const FVector2D UV(SourcePos + FVector2D::ZeroVector);

        new (BakedRenderData) FVector4(PivotSpacePos.X * UnitsPerPixel, PivotSpacePos.Y * UnitsPerPixel,
                                       UV.X * InverseWidth, UV.Y * InverseHeight);
    }

    check(BakedRenderData.Num() % 3 == 0);

    // Swap the generated vertices so they end up in counterclockwise order
    for (int32 SVT = 0; SVT < TriangluatedPoints.Num(); SVT += 3) {
        Swap(BakedRenderData[SVT + 2], BakedRenderData[SVT + 0]);
    }
}

FVector2D USimpleFlipbook::ConvertTextureSpaceToPivotSpace(FVector2D Input) const {
    const FVector2D Pivot = GetPivotPosition();

    const float X = Input.X - Pivot.X;
    const float Y = -Input.Y + Pivot.Y;

    return FVector2D(X, Y);
}

void USimpleFlipbook::CreatePolygonFromBoundingBox(FSpriteGeometryCollection &GeomOwner) const {
    FVector2D BoxSize = SourceTexture != nullptr
                            ? FVector2D(SourceTexture->GetSizeX() / Columns, SourceTexture->GetSizeY() / Rows)
                            : FVector2D(0, 0);
    FVector2D BoxPosition(0, 0);

    // Recenter the box
    BoxPosition += BoxSize * 0.5f;

    // Put the bounding box into the geometry array
    GeomOwner.Shapes.Empty();
    GeomOwner.AddRectangleShape(BoxPosition, BoxSize);
}
#endif