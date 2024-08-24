// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Editor/Blutility/Classes/EditorUtilityWidget.h"
#include "Tilemap3DEditorWidget.generated.h"

class UTilemap3DEditorViewportWidget;
class ATilemap3D;
class UTileButtonBase;

/**
 * 
 */
UCLASS(Abstract)
class TILEMAP3DEDITOR_API UTilemap3DEditorWidget : public UEditorUtilityWidget {
    GENERATED_BODY()

protected:
    void NativePreConstruct() override;

    UFUNCTION(BlueprintImplementableEvent, Category = Placement)
    void SlotTileButton(UTileButtonBase* Button, int32 X, int32 Y);

    UFUNCTION(BlueprintImplementableEvent, Category = Tiles)
    void OnTileButtonClicked(int32 X, int32 Y);

public:
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Configuration)
    ATilemap3D* GetTilemapActor() const;

    void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;

private:
    void UpdateTileButtons();
    void UpdateTileAppearance(int32 X, int32 Y, int32 Layer);
    
    UPROPERTY(EditAnywhere, BlueprintGetter = GetTilemapActor, Category = Configuration,
        meta = (UIMin = 1, ClampMin = 1))
    TObjectPtr<ATilemap3D> TilemapActor;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTilemap3DEditorViewportWidget> Editor;

    UPROPERTY()
    TArray<TObjectPtr<UTileButtonBase>> TileButtons;

	UPROPERTY(EditAnywhere, Category = Style)
	TSubclassOf<UTileButtonBase> TileButtonClass;

    FDelegateHandle MapSizeChangedHandle;
    FDelegateHandle TileChangedHandle;
};
