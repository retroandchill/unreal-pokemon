// "Unreal Pokémon" created by Retro & Chill.


#include "Editor/Tilemap3DEditorViewport.h"

#include "SlateOptMacros.h"
#include "Ranges/Optional/Filter.h"
#include "Ranges/Optional/FlatMap.h"
#include "Ranges/Optional/Map.h"
#include "Ranges/Optional/Or.h"
#include "Ranges/Optional/OrElse.h"
#include "Ranges/Views/Map.h"
#include "Tilemap/Tilemap3D.h"
#include "Tileset/Tile3D.h"

constexpr double GridSize = 24.f; 

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void STilemap3DEditorViewport::Construct(const FArguments &InArgs) {
    /*
	ChildSlot
	[
		// Populate the widget
	];
	*/
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void STilemap3DEditorViewport::SetTilemap(ATilemap3D *Tilemap3D) {
    Tilemap = Tilemap3D;
}

void STilemap3DEditorViewport::SetTileHandle(const FTileHandle& InTileHandle) {
    PaintTile = InTileHandle;
}

int32 STilemap3DEditorViewport::OnPaint(const FPaintArgs &Args, const FGeometry &AllottedGeometry,
                                        const FSlateRect &MyCullingRect, FSlateWindowElementList &OutDrawElements,
                                        int32 LayerId,
                                        const FWidgetStyle &InWidgetStyle, bool bParentEnabled) const {
    int32 DrawLayerId = SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId,
                                                 InWidgetStyle,
                                                 bParentEnabled);

    if (!Tilemap.IsValid()) {
        return DrawLayerId;
    }

    FSlateBrush Brush;
    Brush.SetImageSize(FVector2D(GridSize, GridSize));

    const FVector2D TileSize = Brush.ImageSize / AllottedGeometry.Scale;
    int32 SizeX = Tilemap->GetSizeX();
    int32 SizeY = Tilemap->GetSizeY();
    for (int32 i = 0; i < SizeX; i++) {
        for (int32 j = 0; j < SizeY; j++) {
            auto LayerContents = Tilemap->GetTile(i, j).GetTile();
            DrawLayerId++;
            FSlateDrawElement::MakeBox(
                OutDrawElements,
                DrawLayerId,
                AllottedGeometry.ToPaintGeometry(TileSize, FSlateLayoutTransform(
                                                     FVector2D(GridSize * i, GridSize * j)
                                                        / AllottedGeometry.Scale)),
                &Brush,
                ESlateDrawEffect::None,
                LayerContents.IsSet() ? FLinearColor::Green : FLinearColor::White);
        }
    }

    if (CurrentMousePosition.IsSet()) {
        auto &Pos = *CurrentMousePosition;
        auto CursorSize = PaintTile.GetTile() |
            UE::Optionals::Map([&TileSize](const FTile3D& Tile)
                { return FVector2D(TileSize.X * Tile.SizeX, TileSize.Y * Tile.SizeY); }) |
            UE::Optionals::OrElse(TileSize);
        DrawLayerId++;
        FSlateDrawElement::MakeBox(
        OutDrawElements,
        DrawLayerId,
        AllottedGeometry.ToPaintGeometry(CursorSize, FSlateLayoutTransform(
                                             FVector2D(GridSize * Pos.X, GridSize * Pos.Y)
                                                / AllottedGeometry.Scale)),
        &Brush,
        ESlateDrawEffect::None,
        FLinearColor::Red);
    }

    return DrawLayerId;
}

FVector2D STilemap3DEditorViewport::ComputeDesiredSize(float LayoutScaleMultiplier) const {
    if (!Tilemap.IsValid()) {
        return SCompoundWidget::ComputeDesiredSize(LayoutScaleMultiplier);
    }

    return FVector2D(Tilemap->GetSizeX() * GridSize, Tilemap->GetSizeY() * GridSize);
}

void STilemap3DEditorViewport::OnMouseLeave(const FPointerEvent &MouseEvent) {
    SCompoundWidget::OnMouseLeave(MouseEvent);
    CurrentMousePosition.Reset();
    bIsHoldingMouse = false;
}

FReply STilemap3DEditorViewport::OnMouseMove(const FGeometry &SenderGeometry, const FPointerEvent &MouseEvent) {
    auto MousePosition = SenderGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
    if (auto NewGridPos = GetGridPosition(MousePosition, SenderGeometry.Scale); CurrentMousePosition != NewGridPos) {
        CurrentMousePosition = NewGridPos |
            UE::Optionals::Filter([this](const FIntVector2&) { return Tilemap.IsValid(); }) |
            UE::Optionals::Map([this](const FIntVector2&) -> auto& { return PaintTile; }) |
                UE::Optionals::FlatMap(&FTileHandle::GetTile) |
                UE::Optionals::Map([this, &NewGridPos](const FTile3D& I) {
                    return FIntVector2(
                        FMath::Min(NewGridPos->X, Tilemap->GetSizeX() - I.SizeX),
                        FMath::Min(NewGridPos->Y, Tilemap->GetSizeY() - I.SizeY));
                }) | UE::Optionals::Or([&NewGridPos] { return NewGridPos; });

        if (bIsHoldingMouse && Tilemap.IsValid() && CurrentMousePosition.IsSet()) {
            if (auto TileData = Tilemap->GetTile(CurrentMousePosition->X, CurrentMousePosition->Y);
                    TileData.GetTileOrigin() == *CurrentMousePosition || TileData.GetTileHandle() != PaintTile) {
                SetTile(*CurrentMousePosition);
                return FReply::Handled();
            }
        }
    }
    
    return FReply::Unhandled();
}

FReply STilemap3DEditorViewport::OnMouseButtonDown(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent) {
    FReply Reply = FReply::Unhandled();
    if (MouseEvent.GetEffectingButton() != EKeys::LeftMouseButton || !Tilemap.IsValid() || !CurrentMousePosition.IsSet()) {
        return FReply::Unhandled();
    }

    bIsHoldingMouse = true;
    SetTile(*CurrentMousePosition);
    return FReply::Handled();
}

FReply STilemap3DEditorViewport::OnMouseButtonUp(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent) {
    if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
        bIsHoldingMouse = false;
    }
    
    return FReply::Unhandled();
}

TOptional<FIntVector2> STilemap3DEditorViewport::GetGridPosition(const FVector2D &Geometry, double Scale) const {
    if (!Tilemap.IsValid()) {
        return TOptional<FIntVector2>();
    }

    int32 SizeX = Tilemap->GetSizeX();
    int32 SizeY = Tilemap->GetSizeY();

    double Divisor = GridSize / Scale;

    int32 CoordX = FMath::FloorToInt32(Geometry.X / Divisor);
    int32 CoordY = FMath::FloorToInt32(Geometry.Y / Divisor);

    if (CoordX < 0 || CoordX >= SizeX || CoordY < 0 || CoordY >= SizeY) {
        return TOptional<FIntVector2>();
    }

    return FIntVector2(CoordX, CoordY);
}

void STilemap3DEditorViewport::SetTile(const FIntVector2& Position, int32 Layer) const {
    if (PaintTile.IsValidTile()) {
        Tilemap->AddTile(PaintTile, Position.X, Position.Y, Layer);
    } else {
        Tilemap->RemoveTile(Position.X, Position.Y, Layer);
    }
}
