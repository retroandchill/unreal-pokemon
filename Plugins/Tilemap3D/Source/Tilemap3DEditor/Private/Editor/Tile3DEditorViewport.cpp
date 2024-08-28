// "Unreal Pokémon" created by Retro & Chill.

#include "Editor/Tile3DEditorViewport.h"
#include "Editor/Tile3DEditorViewportClient.h"
#include "Styling/SlateStyleRegistry.h"

class STileset3DEditorViewportToolbar : public SCommonEditorViewportToolbarBase {
  public:
    SLATE_BEGIN_ARGS(STileset3DEditorViewportToolbar) {
    }
    SLATE_END_ARGS()

    void Construct(const FArguments &InArgs,
                   TSharedPtr<class ICommonEditorViewportToolbarInfoProvider> InInfoProvider) {
        SCommonEditorViewportToolbarBase::Construct(SCommonEditorViewportToolbarBase::FArguments(), InInfoProvider);
    }

    TSharedRef<SWidget> GenerateShowMenu() const override {
        GetInfoProvider().OnFloatingButtonClicked();
        auto ViewportRef = GetInfoProvider().GetViewportWidget();
        const bool bInShouldCloseWindowAfterMenuSelection = true;
        FMenuBuilder ShowMenuBuilder(bInShouldCloseWindowAfterMenuSelection, ViewportRef->GetCommandList());
        return ShowMenuBuilder.MakeWidget();
    }
};

void STile3DEditorViewport::Construct(const FArguments &InArgs,
                                      TSharedPtr<FTile3DEditorViewportClient> InViewportClient) {
    TypedViewportClient = InViewportClient;

    SEditorViewport::Construct(SEditorViewport::FArguments());

    auto ParentContents = ChildSlot.GetWidget();

    auto Style = FSlateStyleRegistry::FindSlateStyle("PaperStyle");
    ChildSlot[SNew(SOverlay) + SOverlay::Slot()[ParentContents]

              + SOverlay::Slot().VAlign(VAlign_Bottom)
                    [SNew(SBorder)
                         .BorderImage(Style->GetBrush("Paper2D.Common.ViewportTitleBackground"))
                         .HAlign(HAlign_Fill)
                         .Visibility(EVisibility::HitTestInvisible)
                             [SNew(SVerticalBox)
                              // Title text/icon
                              + SVerticalBox::Slot().AutoHeight()
                                    [SNew(SHorizontalBox) +
                                     SHorizontalBox::Slot()
                                         .HAlign(HAlign_Center)
                                         .FillWidth(1.f)[SNew(STextBlock)
                                                             .TextStyle(Style, "Paper2D.Common.ViewportTitleTextStyle")
                                                             .Text(this, &STile3DEditorViewport::GetTitleText)]]]]];
}

TSharedPtr<SWidget> STile3DEditorViewport::MakeViewportToolbar() {
    return SNew(STileset3DEditorViewportToolbar, SharedThis(this));
}

TSharedRef<FEditorViewportClient> STile3DEditorViewport::MakeEditorViewportClient() {
    return TypedViewportClient.ToSharedRef();
}

void STile3DEditorViewport::OnFocusViewportToSelection() {
    TypedViewportClient->RequestFocusOnSelection(/*bInstant=*/false);
}

TSharedRef<SEditorViewport> STile3DEditorViewport::GetViewportWidget() {
    return SharedThis(this);
}

TSharedPtr<FExtender> STile3DEditorViewport::GetExtenders() const {
    return MakeShared<FExtender>();
}

void STile3DEditorViewport::OnFloatingButtonClicked() {
    // Do nothing
}

FText STile3DEditorViewport::GetTitleText() const {
    const int32 CurrentTileIndex = TypedViewportClient->GetTileIndex();
    if (CurrentTileIndex != INDEX_NONE) {
        FNumberFormattingOptions NoGroupingFormat;
        NoGroupingFormat.SetUseGrouping(false);

        return FText::Format(NSLOCTEXT("Tilemap3D", "STile3DEditorViewportTitle", "Editing tile #{0}"),
                             FText::AsNumber(CurrentTileIndex, &NoGroupingFormat));
    }

    return NSLOCTEXT("Tilemap3D", "STile3DEditorViewportTitle_NoTile", "Tile Editor - Select a tile");
}