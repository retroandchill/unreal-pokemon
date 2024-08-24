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
    
public:
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Configuration)
    ATilemap3D* GetTilemapActor() const;

    void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;

private:
    UPROPERTY(EditAnywhere, BlueprintGetter = GetTilemapActor, Category = Configuration)
    TObjectPtr<ATilemap3D> TilemapActor;
    
    UPROPERTY(EditAnywhere, Category = Configuration)
    TObjectPtr<UStaticMesh> SelectedTile;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTilemap3DEditorViewportWidget> Editor;
};
