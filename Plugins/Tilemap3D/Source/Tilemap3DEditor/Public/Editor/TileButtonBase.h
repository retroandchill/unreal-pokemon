// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "TileButtonBase.generated.h"

class ATilemap3D;

/**
 * 
 */
UCLASS(Abstract)
class TILEMAP3DEDITOR_API UTileButtonBase : public UCommonButtonBase {
    GENERATED_BODY()

protected:
    void NativeOnMouseEnter(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent) override;
public:
    UFUNCTION(BlueprintImplementableEvent, Category = Style)
    void Refresh(ATilemap3D* Tilemap, int32 X, int32 Y, int32 Layer = 0);

};
