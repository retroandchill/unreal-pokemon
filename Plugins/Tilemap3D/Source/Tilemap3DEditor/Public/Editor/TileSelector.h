// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Tileset/Tile3D.h"
#include "Widgets/SCompoundWidget.h"
#include "Ranges/Optional/OptionalRef.h"
#include "Tilemap/Tilemap3D.h"

class UTileset3D;

DECLARE_DELEGATE_OneParam(FOnSelectedTileChanged, const FTileHandle& Tile);

/**
 * 
 */
class TILEMAP3DEDITOR_API STileSelector : public SCompoundWidget {
public:
    SLATE_BEGIN_ARGS(STileSelector) {
    }
        SLATE_EVENT(FOnSelectedTileChanged, OnSelectedTileChanged)
    SLATE_END_ARGS()
        

    /**
     * Constructs this widget with the provided arguments
     * @param InArgs The arguments provided to slate
     */
    void Construct(const FArguments &InArgs);

    void SetTileset(UTileset3D* Tileset3D);
    
private:
    void OnTileSelectionChanged(FName Item, ESelectInfo::Type) const;
    
    TSharedPtr<SComboBox<FName>> TileComboBox;
    TSharedPtr<SOverlay> DetailsOverlay;
    TSharedPtr<IStructureDetailsView> DetailsView;
    TWeakObjectPtr<UTileset3D> Tileset;
    TArray<FName> TileOptions;
    FOnSelectedTileChanged OnSelectedTileChanged_Handler;
};
