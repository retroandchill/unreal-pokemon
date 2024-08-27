// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "SEditorViewport.h"

class FTileSelectorViewportClient;
/**
 * 
 */
class TILEMAP3DEDITOR_API STileSelectorViewport : public SEditorViewport {
public:
    SLATE_BEGIN_ARGS(STileSelectorViewport) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs, TSharedPtr<FTileSelectorViewportClient> InViewportClient);
    TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;
    
    FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
    FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
    FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
    FReply OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
    FCursorReply OnCursorQuery( const FGeometry& MyGeometry, const FPointerEvent& CursorEvent ) const override;
    
private:
    TSharedPtr<FTileSelectorViewportClient> TypedViewportClient;
};
