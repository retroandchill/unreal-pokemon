// "Unreal Pokémon" created by Retro & Chill.


#include "Editor/Tilemap3DEditorWidget.h"
#include "CommonButtonBase.h"
#include "Algo/ForEach.h"
#include "Blueprint/WidgetTree.h"

void UTilemap3DEditorWidget::NativePreConstruct() {
    Super::NativePreConstruct();
    if (TileButtonClass == nullptr) {
        return;
    }
    
    Algo::ForEach(TileButtons, &UWidget::RemoveFromParent);
    TileButtons.Reset();
    TileButtons.Reserve(SizeX * SizeY);
    for (int32 i = 0; i < SizeX; i++) {
        for (int32 j = 0; j < SizeY; j++) {
            auto Button = WidgetTree->ConstructWidget(TileButtonClass);
            SlotTileButton(Button, i, j);
            Button->OnClicked().AddUObject(this, &UTilemap3DEditorWidget::OnTileButtonClicked, i, j);
            TileButtons.Emplace(Button);
        }
    }
}