// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "SCommonEditorViewportToolbarBase.h"
#include "SEditorViewport.h"

class FTile3DEditorViewportClient;

class TILEMAP3DEDITOR_API STile3DEditorViewport : public SEditorViewport, public ICommonEditorViewportToolbarInfoProvider {
public:
    SLATE_BEGIN_ARGS(STile3DEditorViewport) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs, TSharedPtr<FTile3DEditorViewportClient> InViewportClient);

    TSharedPtr<SWidget> MakeViewportToolbar() override;
    TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;
    void BindCommands() override;
    void OnFocusViewportToSelection() override;

    TSharedRef<SEditorViewport> GetViewportWidget() override;
    TSharedPtr<FExtender> GetExtenders() const override;
    void OnFloatingButtonClicked() override;

protected:
    FText GetTitleText() const;

private:
    TSharedPtr<FTile3DEditorViewportClient> TypedViewportClient;
};
