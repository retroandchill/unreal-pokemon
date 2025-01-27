// "Unreal Pokémon" created by Retro & Chill.

#include "Simple2D/Assets/SimpleFlipbook/Widgets/SimpleFlipbookPropertiesTabsBody.h"
#include "Simple2D/Assets/SimpleFlipbook/SimpleFlipbookEditor.h"
#include "SlateOptMacros.h"

namespace Simple2D {
    void SSimpleFlipbookPropertiesTabsBody::Construct(const FArguments &,
                                                      const TSharedPtr<FSimpleFlipbookEditor> &InFlipbookEditor) {
        FlipbookEditorPtr = InFlipbookEditor;

        SSingleObjectDetailsPanel::Construct(SSingleObjectDetailsPanel::FArguments()
                                                 .HostCommandList(InFlipbookEditor->GetToolkitCommands())
                                                 .HostTabManager(InFlipbookEditor->GetTabManager()),
                                             true, true);
    }

    UObject *SSimpleFlipbookPropertiesTabsBody::GetObjectToObserve() const {
        return FlipbookEditorPtr.Pin()->GetFlipbookBeingEdited();
    }

    TSharedRef<SWidget> SSimpleFlipbookPropertiesTabsBody::PopulateSlot(TSharedRef<SWidget> PropertyEditorWidget) {
        return SNew(SVerticalBox) + SVerticalBox::Slot().FillHeight(1)[PropertyEditorWidget];
    }
} // namespace Simple2D