// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "PaperFlipbookSceneProxy.h"

/**
 * 
 */
class SIMPLE2D_API FSimpleFlipbookSceneProxy : public FPaperRenderSceneProxy {
public:
    explicit FSimpleFlipbookSceneProxy(const UMeshComponent* InComponent);

    void SetSprite_RenderThread(const FSpriteDrawCallRecord& NewDynamicData, int32 SplitIndex);

    SIZE_T GetTypeHash() const override;

private:
    UMaterialInterface* Material;
    UMaterialInterface* AlternateMaterial;
};
