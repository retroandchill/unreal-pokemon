// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "PaperRenderSceneProxy.h"

/**
 * 
 */
class SIMPLE2D_API FSimpleFlipbookSceneProxy : public FPaperRenderSceneProxy {
public:
    explicit FSimpleFlipbookSceneProxy(const UMeshComponent* InComponent);
    
    SIZE_T GetTypeHash() const override;
    
    void SetFlipbookBounds(const FSpriteDrawCallRecord& NewDynamicData);

private:
    
    UMaterialInterface* Material;
    UMaterialInterface* AlternateMaterial;
};
