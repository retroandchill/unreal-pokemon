// "Unreal Pokémon" created by Retro & Chill.

#include "Simple2D/Assets/SimpleFlipbook/SimpleFlipbookThumbnailRenderer.h"
#include "CanvasItem.h"
#include "CanvasTypes.h"
#include "Paper2DModule.h"
#include "Simple2D/Assets/SimpleFlipbook.h"

void USimpleFlipbookThumbnailRenderer::Draw(UObject *Object, int32 X, int32 Y, uint32 Width, uint32 Height,
                                            FRenderTarget *RenderTarget, FCanvas *Canvas, bool bAdditionalViewFamily) {
    if (auto *Flipbook = Cast<USimpleFlipbook>(Object); Flipbook != nullptr) {
        const double DeltaTime = FApp::GetCurrentTime() - GStartTime;
        const double TotalDuration = Flipbook->GetTotalDuration();
        const float PlayTime = (TotalDuration > 0.0f) ? FMath::Fmod(DeltaTime, TotalDuration) : 0.0f;

        if (const int32 KeyFrameIndex = Flipbook->GetKeyFrameIndexAtTime(PlayTime);
            Flipbook->IsValidKeyFrameIndex(KeyFrameIndex)) {
            const auto &[Index, FrameRun] = Flipbook->GetKeyFrameChecked(KeyFrameIndex);
            DrawFrame(Flipbook, Index, X, Y, Width, Height, RenderTarget, Canvas);
            return;
        }

        // Fallback for empty frames or newly created flipbooks
        DrawGrid(X, Y, Width, Height, Canvas);

        if (TotalDuration == 0.0f) {
            // Warning text for no frames
            const FText ErrorText = NSLOCTEXT("SimpleFlipbookEditorApp", "ThumbnailWarningNoFrames", "No frames");
            FCanvasTextItem TextItem(FVector2D(5.0f, 5.0f), ErrorText, GEngine->GetLargeFont(), FLinearColor::Red);
            TextItem.EnableShadow(FLinearColor::Black);
            TextItem.Scale = FVector2D(Width / 128.0f, Height / 128.0f);
            TextItem.Draw(Canvas);
        }
    }
}

void USimpleFlipbookThumbnailRenderer::DrawFrame(USimpleFlipbook *Flipbook, int32 Index, int32 X, int32 Y, uint32 Width,
                                                 uint32 Height, FRenderTarget *, FCanvas *Canvas) {
    const UTexture2D *SourceTexture = nullptr;
    if (Flipbook != nullptr) {
        SourceTexture = Flipbook->GetSourceTexture();
    }

    if (SourceTexture != nullptr) {
        const bool bUseTranslucentBlend = SourceTexture->HasAlphaChannel();

        // Draw the grid behind the sprite
        if (bUseTranslucentBlend) {
            DrawGrid(X, Y, Width, Height, Canvas);
        }

        // Draw the sprite itself
        // Use the baked render data, so we don't have to care about rotations and possibly
        // other sprites overlapping in source, UV region, etc.
        const auto &BakedRenderData = Flipbook->BakedRenderData;
        TArray<FVector2D> CanvasPositions;
        TArray<FVector2D> CanvasUVs;

        FVector2D ActualSize(static_cast<double>(SourceTexture->GetSizeX()) / Flipbook->GetColumns(),
                             static_cast<double>(SourceTexture->GetSizeY()) / Flipbook->GetRows());
        FVector2D ScaledSize(ActualSize.X / SourceTexture->GetSizeX(), ActualSize.Y / SourceTexture->GetSizeY());
        const int32 Column = Index % Flipbook->GetColumns();
        const int32 Row = Index / Flipbook->GetColumns();
        for (int Vertex = 0; Vertex < BakedRenderData.Num(); ++Vertex) {
            new (CanvasPositions) FVector2D(BakedRenderData[Vertex].X, BakedRenderData[Vertex].Y);
            new (CanvasUVs) FVector2D(BakedRenderData[Vertex].Z * ScaledSize.X + ScaledSize.X * Column,
                                      BakedRenderData[Vertex].W * ScaledSize.Y + ScaledSize.Y * Row);
        }

        // Determine the bounds to use
        auto RenderBounds = Flipbook->GetRenderBounds();

        const FVector MinPoint3D = RenderBounds.GetBoxExtrema(0);
        const FVector MaxPoint3D = RenderBounds.GetBoxExtrema(1);
        const FVector2D MinPoint(FVector::DotProduct(MinPoint3D, PaperAxisX),
                                 FVector::DotProduct(MinPoint3D, PaperAxisY));
        const FVector2D MaxPoint(FVector::DotProduct(MaxPoint3D, PaperAxisX),
                                 FVector::DotProduct(MaxPoint3D, PaperAxisY));

        const float UnscaledWidth = MaxPoint.X - MinPoint.X;
        const float UnscaledHeight = MaxPoint.Y - MinPoint.Y;
        const FVector2D Origin(X + Width * 0.5f, Y + Height * 0.5f);
        const bool bIsWider = (UnscaledWidth > 0.0f) && (UnscaledHeight > 0.0f) && (UnscaledWidth > UnscaledHeight);
        const float ScaleFactor = bIsWider ? (Width / UnscaledWidth) : (Height / UnscaledHeight);

        // Scale and recenter
        const FVector2D CanvasPositionCenter = (MaxPoint + MinPoint) * 0.5f;
        for (int Vertex = 0; Vertex < CanvasPositions.Num(); ++Vertex) {
            CanvasPositions[Vertex] = (CanvasPositions[Vertex] - CanvasPositionCenter) * ScaleFactor + Origin;
            CanvasPositions[Vertex].Y = Height - CanvasPositions[Vertex].Y;
        }

        // Draw triangles
        if (CanvasPositions.Num() > 0 && SourceTexture->GetResource() != nullptr) {
            TArray<FCanvasUVTri> Triangles;
            const FLinearColor SpriteColor(FLinearColor::White);
            for (int Vertex = 0; Vertex < CanvasPositions.Num(); Vertex += 3) {
                FCanvasUVTri *Triangle = new (Triangles) FCanvasUVTri();
                Triangle->V0_Pos = CanvasPositions[Vertex + 0];
                Triangle->V0_UV = CanvasUVs[Vertex + 0];
                Triangle->V0_Color = SpriteColor;
                Triangle->V1_Pos = CanvasPositions[Vertex + 1];
                Triangle->V1_UV = CanvasUVs[Vertex + 1];
                Triangle->V1_Color = SpriteColor;
                Triangle->V2_Pos = CanvasPositions[Vertex + 2];
                Triangle->V2_UV = CanvasUVs[Vertex + 2];
                Triangle->V2_Color = SpriteColor;
            }
            FCanvasTriangleItem CanvasTriangle(Triangles, SourceTexture->GetResource());
            CanvasTriangle.BlendMode = bUseTranslucentBlend ? SE_BLEND_Translucent : SE_BLEND_Opaque;
            Canvas->DrawItem(CanvasTriangle);
        }
    } else {
        // Fallback for a bogus sprite
        DrawGrid(X, Y, Width, Height, Canvas);
    }
}

void USimpleFlipbookThumbnailRenderer::DrawGrid(int32 X, int32 Y, uint32 Width, uint32 Height, FCanvas *Canvas) {
    static UTexture2D *GridTexture = nullptr;
    if (GridTexture == nullptr) {
        GridTexture = LoadObject<UTexture2D>(
            nullptr, TEXT("/Engine/EngineMaterials/DefaultDiffuse_TC_Masks.DefaultDiffuse_TC_Masks"), nullptr,
            LOAD_None, nullptr);
    }

    constexpr bool bAlphaBlend = false;

    Canvas->DrawTile(static_cast<float>(X), static_cast<float>(Y), static_cast<float>(Width),
                     static_cast<float>(Height), 0.0f, 0.0f, 4.0f, 4.0f, FLinearColor::White,
                     GridTexture->GetResource(), bAlphaBlend);
}