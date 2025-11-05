// "Unreal Pokémon" created by Retro & Chill.

#include "Simple2D/Simple2DStyle.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"

#define IMAGE_PLUGIN_BRUSH(RelativePath, ...)                                                                          \
    FSlateImageBrush(FSimple2DStyle::InContent(RelativePath, ".png"), __VA_ARGS__)
#define IMAGE_PLUGIN_BRUSH_SVG(RelativePath, ...)                                                                      \
    FSlateVectorImageBrush(FSimple2DStyle::InContent(RelativePath, ".svg"), __VA_ARGS__)
#define IMAGE_BRUSH(RelativePath, ...)                                                                                 \
    FSlateImageBrush(StyleSet->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define BOX_BRUSH(RelativePath, ...) FSlateBoxBrush(StyleSet->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define DEFAULT_FONT(...) FCoreStyle::GetDefaultFontStyle(__VA_ARGS__)

namespace Simple2D {

    TSharedPtr<FSlateStyleSet> FSimple2DStyle::StyleSet;

    void FSimple2DStyle::Initialize() {
        // Const icon sizes
        const FVector2D Icon8x8(8.0f, 8.0f);
        const FVector2D Icon16x16(16.0f, 16.0f);
        const FVector2D Icon20x20(20.0f, 20.0f);
        const FVector2D Icon40x40(40.0f, 40.0f);
        const FVector2D Icon64x64(64.0f, 64.0f);

        // Only register once
        if (StyleSet.IsValid()) {
            return;
        }

        StyleSet = MakeShareable(new FSlateStyleSet(GetStyleSetName()));
        StyleSet->SetContentRoot(FPaths::EngineContentDir() / TEXT("Editor/Slate"));
        StyleSet->SetCoreContentRoot(FPaths::EngineContentDir() / TEXT("Slate"));

        const FTextBlockStyle &NormalText = FAppStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText");

        // Shared editors
        {
            StyleSet->Set("Paper2D.Common.ViewportZoomTextStyle",
                          FTextBlockStyle(NormalText).SetFont(DEFAULT_FONT("BoldCondensed", 16)));

            StyleSet->Set("Paper2D.Common.ViewportTitleTextStyle",
                          FTextBlockStyle(NormalText)
                              .SetFont(DEFAULT_FONT("Regular", 18))
                              .SetColorAndOpacity(FLinearColor(1.0, 1.0f, 1.0f, 0.5f)));

            StyleSet->Set("Paper2D.Common.ViewportTitleBackground",
                          new BOX_BRUSH("Old/Graph/GraphTitleBackground", FMargin(0)));
        }

        // Flipbook editor
        {
            StyleSet->Set("FlipbookEditor.SetShowGrid", new IMAGE_BRUSH(TEXT("Icons/icon_MatEd_Grid_40x"), Icon40x40));
            StyleSet->Set("FlipbookEditor.SetShowGrid.Small",
                          new IMAGE_BRUSH(TEXT("Icons/icon_MatEd_Grid_40x"), Icon20x20));
            StyleSet->Set("FlipbookEditor.SetShowBounds",
                          new IMAGE_BRUSH(TEXT("Icons/icon_StaticMeshEd_Bounds_40x"), Icon40x40));
            StyleSet->Set("FlipbookEditor.SetShowBounds.Small",
                          new IMAGE_BRUSH(TEXT("Icons/icon_StaticMeshEd_Bounds_40x"), Icon20x20));
            StyleSet->Set("FlipbookEditor.SetShowCollision",
                          new IMAGE_BRUSH(TEXT("Icons/icon_StaticMeshEd_Collision_40x"), Icon40x40));
            StyleSet->Set("FlipbookEditor.SetShowCollision.Small",
                          new IMAGE_BRUSH(TEXT("Icons/icon_StaticMeshEd_Collision_40x"), Icon20x20));
            StyleSet->Set("FlipbookEditor.SetShowPivot",
                          new IMAGE_BRUSH(TEXT("Icons/icon_StaticMeshEd_ShowPivot_40x"), Icon40x40));
            StyleSet->Set("FlipbookEditor.SetShowPivot.Small",
                          new IMAGE_BRUSH(TEXT("Icons/icon_StaticMeshEd_ShowPivot_40x"), Icon20x20));
            StyleSet->Set("FlipbookEditor.SetShowSockets",
                          new IMAGE_BRUSH(TEXT("Icons/icon_StaticMeshEd_ShowSockets_40x"), Icon40x40));
            StyleSet->Set("FlipbookEditor.SetShowSockets.Small",
                          new IMAGE_BRUSH(TEXT("Icons/icon_StaticMeshEd_ShowSockets_40x"), Icon20x20));

            StyleSet->Set("FlipbookEditor.AddKeyFrame",
                          new IMAGE_PLUGIN_BRUSH(TEXT("Icons/FlipbookEditor/icon_Flipbook_AddKey_40x"), Icon40x40));
            StyleSet->Set("FlipbookEditor.AddKeyFrame.Small",
                          new IMAGE_PLUGIN_BRUSH(TEXT("Icons/FlipbookEditor/icon_Flipbook_AddKey_40x"), Icon20x20));

            StyleSet->Set(
                "FlipbookEditor.RegionGrabHandle",
                new BOX_BRUSH("Sequencer/ScrubHandleWhole", FMargin(6.f / 13.f, 10 / 24.f, 6 / 13.f, 10 / 24.f)));
            StyleSet->Set("FlipbookEditor.RegionBody", new BOX_BRUSH("Common/Scrollbar_Thumb", FMargin(4.f / 16.f)));
            StyleSet->Set("FlipbookEditor.RegionBorder", new BOX_BRUSH("Common/CurrentCellBorder", FMargin(4.f / 16.f),
                                                                       FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)));
        }

        // Asset actions
        {
            StyleSet->Set("AssetActions.CreateSprite",
                          new IMAGE_PLUGIN_BRUSH(TEXT("Icons/icon_Texture_CreateSprite_16x"), Icon16x16));
            StyleSet->Set("AssetActions.ExtractSprites",
                          new IMAGE_PLUGIN_BRUSH(TEXT("Icons/icon_Texture_ExtractSprites_16x"), Icon16x16));
            StyleSet->Set("AssetActions.ConfigureForRetroSprites",
                          new IMAGE_PLUGIN_BRUSH(TEXT("Icons/icon_Texture_ConfigureForRetroSprites_16x"), Icon16x16));
            StyleSet->Set("AssetActions.CreateTileSet",
                          new IMAGE_PLUGIN_BRUSH(TEXT("Icons/icon_Texture_CreateTileSet_16x"), Icon16x16));
        }

        {
            // Flipbooks (asset, component, actor)
            StyleSet->Set("ClassIcon.SimpleFlipbook",
                          new IMAGE_PLUGIN_BRUSH_SVG("Icons/AssetIcons/PaperFlipbookActor_16", Icon16x16));
            StyleSet->Set("ClassThumbnail.SimpleFlipbook",
                          new IMAGE_PLUGIN_BRUSH_SVG("Icons/AssetIcons/PaperFlipbookActor_64", Icon64x64));
            StyleSet->Set("ClassIcon.SimpleFlipbookComponent",
                          new IMAGE_PLUGIN_BRUSH_SVG("Icons/AssetIcons/PaperFlipbookActor_16", Icon16x16));
            StyleSet->Set("ClassThumbnail.SimpleFlipbookComponent",
                          new IMAGE_PLUGIN_BRUSH_SVG("Icons/AssetIcons/PaperFlipbookActor_64", Icon64x64));
            StyleSet->Set("ClassIcon.SimpleFlipbookActor",
                          new IMAGE_PLUGIN_BRUSH_SVG("Icons/AssetIcons/PaperFlipbookActor_16", Icon16x16));
            StyleSet->Set("ClassThumbnailSimpleFlipbookActor",
                          new IMAGE_PLUGIN_BRUSH_SVG("Icons/AssetIcons/PaperFlipbookActor_64", Icon64x64));
        }

        FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
    }

    void FSimple2DStyle::Shutdown() {
        if (StyleSet.IsValid()) {
            FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
            ensure(StyleSet.IsUnique());
            StyleSet.Reset();
        }
    }

    TSharedPtr<ISlateStyle> FSimple2DStyle::Get() {
        return StyleSet;
    }

    FName FSimple2DStyle::GetStyleSetName() {
        static FName PaperStyleName(TEXT("Simple2DStyle"));
        return PaperStyleName;
    }

    FString FSimple2DStyle::InContent(const FString &RelativePath, const ANSICHAR *Extension) {
        static FString ContentDir = IPluginManager::Get().FindPlugin(TEXT("Paper2D"))->GetContentDir();
        return (ContentDir / RelativePath) + Extension;
    }
} // namespace Simple2D