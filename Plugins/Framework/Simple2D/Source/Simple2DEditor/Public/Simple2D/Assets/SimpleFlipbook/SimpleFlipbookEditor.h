// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "Editor/Experimental/EditorInteractiveToolsFramework/Public/Behaviors/2DViewportBehaviorTargets.h"
#include "ITransportControl.h"
#include "Simple2D/Assets/SimpleFlipbook.h"
#include "Toolkits/AssetEditorToolkit.h"

class USimpleFlipbookComponent;

namespace Simple2D {
    class SSimpleFlipbookEditorViewport;

    /**
     *
     */
    class SIMPLE2DEDITOR_API FSimpleFlipbookEditor : public FAssetEditorToolkit, public FGCObject {
      public:
        void RegisterTabSpawners(const TSharedRef<FTabManager> &InTabManager) override;
        void UnregisterTabSpawners(const TSharedRef<FTabManager> &InTabManager) override;

        FName GetToolkitFName() const override;
        FText GetBaseToolkitName() const override;
        FText GetToolkitName() const override;
        FText GetToolkitToolTipText() const override;
        FLinearColor GetWorldCentricTabColorScale() const override;
        FString GetWorldCentricTabPrefix() const override;

        void AddReferencedObjects(FReferenceCollector &Collector) override;

        FString GetReferencerName() const override {
            return TEXT("Simple2D::FSimpleFlipbookEditor");
        }

        void InitFlipbookEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost> &InitToolkitHost,
                                USimpleFlipbook *InitFlipbook);

        USimpleFlipbook *GetFlipbookBeingEdited() const {
            return FlipbookBeingEdited;
        }

        USimpleFlipbookComponent *GetPreviewComponent() const;

      private:
        float GetFramesPerSecond() const {
            return FlipbookBeingEdited->GetFramesPerSecond();
        }

        int32 GetCurrentFrame() const {
            const int32 TotalLengthInFrames = GetTotalFrameCount();
            if (TotalLengthInFrames == 0) {
                return INDEX_NONE;
            }

            return FMath::Clamp<int32>((int32)(GetPlaybackPosition() * GetFramesPerSecond()), 0, TotalLengthInFrames);
        }

        void SetCurrentFrame(int32 NewIndex) {
            if (const int32 TotalLengthInFrames = GetTotalFrameCount(); TotalLengthInFrames > 0) {
                int32 ClampedIndex = FMath::Clamp<int32>(NewIndex, 0, TotalLengthInFrames);
                SetPlaybackPosition(ClampedIndex / GetFramesPerSecond());
            } else {
                SetPlaybackPosition(0.0f);
            }
        }

        void BindCommands();
        void ExtendMenu();
        void ExtendToolbar();

        TSharedRef<SDockTab> SpawnTab_Viewport(const FSpawnTabArgs &Args);
        TSharedRef<SDockTab> SpawnTab_Details(const FSpawnTabArgs &Args);

        void DeleteSelection();
        void DuplicateSelection();
        void SetSelection(int32 NewSelection);
        bool HasValidSelection() const;

        void AddKeyFrameAtCurrentTime();
        void AddNewKeyFrameAtEnd();
        void AddNewKeyFrameBefore();
        void AddNewKeyFrameAfter();

        FReply OnClick_Forward();
        FReply OnClick_Forward_Step();
        FReply OnClick_Forward_End();
        FReply OnClick_Backward();
        FReply OnClick_Backward_Step();
        FReply OnClick_Backward_End();
        FReply OnClick_ToggleLoop();

        uint32 GetTotalFrameCount() const;
        uint32 GetTotalFrameCountPlusOne() const;
        float GetTotalSequenceLength() const;
        float GetPlaybackPosition() const;
        void SetPlaybackPosition(float NewTime);
        bool IsLooping() const;
        EPlaybackMode::Type GetPlaybackMode() const;

        float GetViewRangeMin() const;
        float GetViewRangeMax() const;
        void SetViewRange(float NewMin, float NewMax);

        TObjectPtr<USimpleFlipbook> FlipbookBeingEdited;
        TSharedPtr<SSimpleFlipbookEditorViewport> ViewportPtr;

        int32 CurrentSelectedKeyframe = INDEX_NONE;

        mutable float ViewInputMin = 0.f;
        mutable float ViewInputMax = 0.f;
        mutable float LastObservedSequenceLength = 0.f;
    };
} // namespace Simple2D