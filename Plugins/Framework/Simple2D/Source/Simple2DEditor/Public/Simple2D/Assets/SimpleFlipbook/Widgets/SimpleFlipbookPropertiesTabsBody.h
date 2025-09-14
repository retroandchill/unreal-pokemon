// "Unreal Pokémon" created by Retro & Chill.

#pragma once
#include "SSingleObjectDetailsPanel.h"

namespace Simple2D
{
    class FSimpleFlipbookEditor;
    /**
     *
     */
    class SIMPLE2DEDITOR_API SSimpleFlipbookPropertiesTabsBody : public SSingleObjectDetailsPanel
    {
      public:
        SLATE_BEGIN_ARGS(SSimpleFlipbookPropertiesTabsBody)
            {
            }

            SLATE_END_ARGS()

            /** Constructs this widget with InArgs */
            void Construct(const FArguments &InArgs, const TSharedPtr<FSimpleFlipbookEditor> &InFlipbookEditor);

            UObject *GetObjectToObserve() const override;
            TSharedRef<SWidget> PopulateSlot(TSharedRef<SWidget> PropertyEditorWidget) override;

          private:
            // Pointer back to owning sprite editor instance (the keeper of state)
            TWeakPtr<FSimpleFlipbookEditor> FlipbookEditorPtr;
        };
    }
