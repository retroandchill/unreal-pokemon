// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Editor/Blutility/Classes/EditorUtilityWidget.h"
#include "Tilemap3DEditorWidget.generated.h"

class UCommonButtonBase;

/**
 * 
 */
UCLASS(Abstract)
class TILEMAP3DEDITOR_API UTilemap3DEditorWidget : public UEditorUtilityWidget {
    GENERATED_BODY()

protected:
    void NativePreConstruct() override;

    UFUNCTION(BlueprintImplementableEvent, Category = Placement)
    void SlotTileButton(UCommonButtonBase* Button, int32 X, int32 Y);

    UFUNCTION(BlueprintImplementableEvent, Category = Tiles)
    void OnTileButtonClicked(int32 X, int32 Y);

private:
    UPROPERTY(EditAnywhere, Category = Configuration, meta = (UIMin = 1, ClampMin = 1))
    int32 SizeX;

    UPROPERTY(EditAnywhere, Category = Configuration, meta = (UIMin = 1, ClampMin = 1))
    int32 SizeY;

    UPROPERTY()
    TArray<TObjectPtr<UCommonButtonBase>> TileButtons;

	UPROPERTY(EditAnywhere, Category = Style)
	TSubclassOf<UCommonButtonBase> TileButtonClass;


};
