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
    
private:
    TSharedPtr<FTileSelectorViewportClient> TypedViewportClient;
};
