// "Unreal Pokémon" created by Retro & Chill.


#include "Simple2D/Rendering/SimpleFlipbookSceneProxy.h"
#include "MaterialDomain.h"
#include "Paper2DModule.h"
#include <bit>

FSimpleFlipbookSceneProxy::FSimpleFlipbookSceneProxy(const UMeshComponent *InComponent) : FPrimitiveSceneProxy(InComponent) {
    Material = InComponent->GetMaterial(0);
    if (Material == nullptr)
    {
        Material = UMaterial::GetDefaultMaterial(MD_Surface);
    }

    AlternateMaterial = InComponent->GetMaterial(1);
    if (AlternateMaterial == nullptr)
    {
        AlternateMaterial = UMaterial::GetDefaultMaterial(MD_Surface);
    }

    MaterialRelevance = InComponent->GetMaterialRelevance(GetScene().GetFeatureLevel());
}

SIZE_T FSimpleFlipbookSceneProxy::GetTypeHash() const {
    static size_t UniquePointer;
    return std::bit_cast<size_t>(std::addressof(UniquePointer));
}