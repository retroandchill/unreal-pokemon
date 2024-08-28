// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "Ranges/Optional/OptionalRef.h"
#include "Tileset/Tile3D.h"

#include "TileSelectorWidget.generated.h"

struct FTileHandle;
struct FTile3D;
class UTileset3D;
class STileSelector;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTileSelectionChanged, const FTileHandle &Tile);

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

    void SetTileset(UTileset3D *Tileset3D);

    FDelegateHandle BindToTileSelectionChanged(FOnTileSelectionChanged::FDelegate &&Delegate);
    void RemoveFromTileSelectionChanged(FDelegateHandle Handle);

  private:
    void TileSelectionChanged(const FTileHandle &Tile) const;

    UPROPERTY()
    TObjectPtr<UTileset3D> Tileset;

    TSharedPtr<STileSelector> TileSelector;

    FOnTileSelectionChanged OnTileSelectionChanged;
};
