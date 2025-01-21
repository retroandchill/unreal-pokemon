// "Unreal Pokémon" created by Retro & Chill.


#include "SimpleFlipbookSceneProxy.h"
#include "MaterialDomain.h"
#include <bit>

FSimpleFlipbookSceneProxy::FSimpleFlipbookSceneProxy(const UMeshComponent *InComponent) : FPaperRenderSceneProxy(InComponent) {
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

void FSimpleFlipbookSceneProxy::SetSprite_RenderThread(const FSpriteDrawCallRecord &NewDynamicData, int32 SplitIndex) {
    SCOPE_CYCLE_COUNTER(STAT_PaperRender_SetSpriteRT);

    BatchedSections.Reset();
    Vertices.Reset();

    if (SplitIndex != INDEX_NONE)
    {
        BatchedSections.AddDefaulted();
        BatchedSections.AddDefaulted();

        FSpriteRenderSection& Section = BatchedSections[0];
        Section.Material = Material;
        Section.AddVerticesFromDrawCallRecord(NewDynamicData, 0, SplitIndex, Vertices);

        FSpriteRenderSection& AlternateSection = BatchedSections[1];
        AlternateSection.Material = AlternateMaterial;
        AlternateSection.AddVerticesFromDrawCallRecord(NewDynamicData, SplitIndex, NewDynamicData.RenderVerts.Num() - SplitIndex, Vertices);
    }
    else
    {
        FSpriteRenderSection& Section = BatchedSections[BatchedSections.AddDefaulted()];
        Section.Material = Material;
        Section.AddVerticesFromDrawCallRecord(NewDynamicData, 0, NewDynamicData.RenderVerts.Num(), Vertices);
    }

    RecreateCachedRenderData(FRHICommandListImmediate::Get());
}

SIZE_T FSimpleFlipbookSceneProxy::GetTypeHash() const {
    static size_t UniquePointer;
    return std::bit_cast<size_t>(std::addressof(UniquePointer));
}