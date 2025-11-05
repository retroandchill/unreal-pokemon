#pragma once

#include "CoreMinimal.h"
#include "Input/Reply.h"
#include "Layout/Visibility.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/SWidget.h"

class FGridMapEditorMode;

class GRIDMAPEDITOR_API SGridMapEditorToolkitWidget : public SCompoundWidget {
  public:
    SLATE_BEGIN_ARGS(SGridMapEditorToolkitWidget) {
        }
        SLATE_END_ARGS()

      public:
        void Construct(const FArguments &InArgs);

      private: // construction helpers
        TSharedRef<SWidget> BuildToolBar();
        TSharedRef<SWidget> BuildPaintOptions();

        TSharedPtr<class STileSetPalette> TileSetPalette;

      private:
        bool IsGridMapEditorEnabled() const;
        bool IsPaintTool() const;
        bool IsSelectTool() const;
        bool IsSettingsTool() const;

        FText GetActiveToolName() const;
        FText GetGridMapEditorErrorText() const;

        // helpers for getting/setting ui
        void SetPaintOrigin(FVector InHeight);
        TOptional<FVector> GetPaintOrigin() const;

        TOptional<float> GetPaintHeight() const;
        void SetPaintHeight(float InPaintHeight);

        TOptional<int32> GetPaintLayer() const;
        void SetPaintLayer(int32 InPaintLayer);

        void OnChangeTileSet(UObject *NewAsset);
        UObject *GetCurrentTileSet() const;

        // helper for visibilities
        EVisibility GetVisibility_PaintOptions() const;
        EVisibility GetVisibility_SelectOptions() const;
        EVisibility GetVisibility_SettingsOptions() const;

      private:
        /** Current error message */
        TSharedPtr<class SErrorText> ErrorText;
        FGridMapEditorMode *GridMapEditorMode;
    };