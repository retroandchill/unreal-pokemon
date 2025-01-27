// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "SCommonEditorViewportToolbarBase.h"
#include "Widgets/SCompoundWidget.h"

namespace Simple2D {
    /**
     *
     */
    class SIMPLE2DEDITOR_API SSimpleFlipbookEditorViewportToolbar : public SCommonEditorViewportToolbarBase {
      public:
        SLATE_BEGIN_ARGS(SSimpleFlipbookEditorViewportToolbar) {
            }

            SLATE_END_ARGS()

            void Construct(const FArguments &InArgs,
                           const TSharedPtr<ICommonEditorViewportToolbarInfoProvider> &InInfoProvider);

            TSharedRef<SWidget> GenerateShowMenu() const override;
        };
    }