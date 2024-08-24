// "Unreal Pokémon" created by Retro & Chill.


#include "Editor/STilemap3DEditorViewport.h"

#include "SlateOptMacros.h"
#include "Tilemap/Tilemap3D.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void STilemap3DEditorViewport::Construct(const FArguments &InArgs) {
    /*
	ChildSlot
	[
		// Populate the widget
	];
	*/
}

void STilemap3DEditorViewport::SetTilemap(ATilemap3D *Tilemap3D) {
    Tilemap = Tilemap3D;
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
    Brush.SetImageSize(FVector2D(16, 16));

    const FVector2D CursorSize = Brush.ImageSize / AllottedGeometry.Scale;
    int32 SizeX = Tilemap->GetSizeX();
    int32 SizeY = Tilemap->GetSizeY();
    for (int32 i = 0; i < SizeX; i++) {
        for (int32 j = 0; j < SizeY; j++) {
            auto LayerContents = Tilemap->GetTile(i, j);
            DrawLayerId++;
            FSlateDrawElement::MakeBox(
                OutDrawElements,
                DrawLayerId,
                AllottedGeometry.ToPaintGeometry(CursorSize, FSlateLayoutTransform(
                                                     FVector2D(16 * i, 16 * j)
                                                        / AllottedGeometry.Scale)),
                &Brush,
                ESlateDrawEffect::None,
                LayerContents != nullptr ? FLinearColor::Green : FLinearColor::White);
        }
    }

    return DrawLayerId;
}

FVector2D STilemap3DEditorViewport::ComputeDesiredSize(float LayoutScaleMultiplier) const {
    if (!Tilemap.IsValid()) {
        return SCompoundWidget::ComputeDesiredSize(LayoutScaleMultiplier);
    }

    return FVector2D(Tilemap->GetSizeX() * 16, Tilemap->GetSizeY() * 16);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION