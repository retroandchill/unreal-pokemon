// "Unreal Pokémon" created by Retro & Chill.

#include "Editor/TileSelectorWidget.h"
#include "Editor/TileSelector.h"

TSharedRef<SWidget> UTileSelectorWidget::RebuildWidget() {
    return SAssignNew(TileSelector, STileSelector)
        .OnSelectedTileChanged_UObject(this, &UTileSelectorWidget::TileSelectionChanged);
}

void UTileSelectorWidget::SynchronizeProperties() {
    Super::SynchronizeProperties();

    if (TileSelector != nullptr) {
        TileSelector->SetTileset(Tileset.Get());
    }
}

void UTileSelectorWidget::ReleaseSlateResources(bool bReleaseChildren) {
    Super::ReleaseSlateResources(bReleaseChildren);
    TileSelector.Reset();
}

void UTileSelectorWidget::SetTileset(UTileset3D *Tileset3D) {
    Tileset = Tileset3D;
    if (TileSelector != nullptr) {
        TileSelector->SetTileset(Tileset3D);
    }
}

FDelegateHandle UTileSelectorWidget::BindToTileSelectionChanged(FOnTileSelectionChanged::FDelegate &&Delegate) {
    return OnTileSelectionChanged.Add(MoveTemp(Delegate));
}

void UTileSelectorWidget::RemoveFromTileSelectionChanged(FDelegateHandle Handle) {
    OnTileSelectionChanged.Remove(Handle);
}

void UTileSelectorWidget::TileSelectionChanged(const FTileHandle &Tile) const {
    OnTileSelectionChanged.Broadcast(Tile);
}