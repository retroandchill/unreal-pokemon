// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "Tileset/Tile3D.h"
#include "Ranges/Optional/OptionalRef.h"

#include "TileSelectorWidget.generated.h"

struct FTile3D;
class UTileset3D;
class STileSelector;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTileSelectionChanged, TOptional<const FTile3D&>);

/**
 * 
 */
UCLASS()
class TILEMAP3DEDITOR_API UTileSelectorWidget : public UWidget {
    GENERATED_BODY()

public:
    TSharedRef<SWidget> RebuildWidget() override;
    void SynchronizeProperties() override;
    void ReleaseSlateResources(bool bReleaseChildren) override;

    void SetTileset(UTileset3D* Tileset3D);

    FDelegateHandle BindToTileSelectionChanged(FOnTileSelectionChanged::FDelegate&& Delegate);
    void RemoveFromTileSelectionChanged(FDelegateHandle Handle);

private:
    void TileSelectionChanged(TOptional<const FTile3D&> Tile) const;
    
    UPROPERTY()
    TObjectPtr<UTileset3D> Tileset;
    
    TSharedPtr<STileSelector> TileSelector;

    FOnTileSelectionChanged OnTileSelectionChanged;
};
