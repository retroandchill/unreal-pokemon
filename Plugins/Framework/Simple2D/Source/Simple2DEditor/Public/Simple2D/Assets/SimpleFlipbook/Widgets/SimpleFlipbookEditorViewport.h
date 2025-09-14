// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "SCommonEditorViewportToolbarBase.h"
#include "SEditorViewport.h"
#include "Simple2D/Assets/SimpleFlipbook/Viewports/SimpleFlipbookEditorViewportClient.h"

class USimpleFlipbook;

namespace Simple2D
{

    /**
     *
     */
    class SIMPLE2DEDITOR_API SSimpleFlipbookEditorViewport : public SEditorViewport,
                                                             public ICommonEditorViewportToolbarInfoProvider
    {
      public:
        SLATE_BEGIN_ARGS(SSimpleFlipbookEditorViewport) : _FlipbookBeingEdited(nullptr)
            {
            }
            SLATE_ATTRIBUTE(USimpleFlipbook *, FlipbookBeingEdited)

            SLATE_END_ARGS()

            void Construct(const FArguments &InArgs);

            void BindCommands() override;
            TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;
            TSharedPtr<SWidget> MakeViewportToolbar() override;
            EVisibility GetTransformToolbarVisibility() const override;
            void OnFocusViewportToSelection() override;

            TSharedRef<SEditorViewport> GetViewportWidget() override;
            TSharedPtr<FExtender> GetExtenders() const override;
            void OnFloatingButtonClicked() override;

            auto *GetPreviewComponent() const
            {
                return EditorViewportClient->GetPreviewComponent();
            }

          private:
            TAttribute<USimpleFlipbook *> FlipbookBeingEdited;
            TSharedPtr<FSimpleFlipbookEditorViewportClient> EditorViewportClient;
        };
    }
