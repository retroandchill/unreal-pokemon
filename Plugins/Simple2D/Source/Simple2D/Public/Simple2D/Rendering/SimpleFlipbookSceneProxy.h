// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "SpriteDrawCall.h"


/**
 * 
 */
class SIMPLE2D_API FSimpleFlipbookSceneProxy : public FPrimitiveSceneProxy {
public:
    explicit FSimpleFlipbookSceneProxy(const UMeshComponent* InComponent);

    SIZE_T GetTypeHash() const override;

private:

    // The view relevance for the associated material
    FMaterialRelevance MaterialRelevance;
    
    UMaterialInterface* Material;
    UMaterialInterface* AlternateMaterial;
};
