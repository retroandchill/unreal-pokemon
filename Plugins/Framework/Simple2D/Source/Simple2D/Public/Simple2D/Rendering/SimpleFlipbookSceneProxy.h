// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "FlipbookRenderSection.h"
#include "FlipbookTextureOverrideRenderProxy.h"
#include "PrimitiveSceneProxy.h"
#include "SimpleFlipbookVertexBuffer.h"
#include "SimpleFlipbookVertexFactory.h"

struct FSimpleFlipbookDrawCall;
struct FSpriteDrawCallRecord;
class UMeshComponent;

namespace Simple2D {
    /**
     *
     */
    class SIMPLE2D_API FSimpleFlipbookSceneProxy : public FPrimitiveSceneProxy {
      public:
        explicit FSimpleFlipbookSceneProxy(const UMeshComponent *InComponent);
        ~FSimpleFlipbookSceneProxy() override;

        UE_NONCOPYABLE(FSimpleFlipbookSceneProxy)

        SIZE_T GetTypeHash() const override;

        void SetFlipbookBounds(const FSimpleFlipbookDrawCall &NewDynamicData);

        void GetDynamicMeshElements(const TArray<const FSceneView *> &Views, const FSceneViewFamily &ViewFamily,
                                    uint32 VisibilityMap, FMeshElementCollector &Collector) const override;
        FPrimitiveViewRelevance GetViewRelevance(const FSceneView *View) const override;
        uint32 GetMemoryFootprint() const override;
        bool CanBeOccluded() const override;
        bool IsUsingDistanceCullFade() const override;
        void CreateRenderThreadResources(FRHICommandListBase &RHICmdList) override;

#if WITH_EDITOR
        void SetTransientTextureOverride_RenderThread(const UTexture *InTextureToModifyOverrideFor,
                                                      const UTexture *InOverrideTexture);
#endif

      protected:
        void GetDynamicMeshElementsForView(const FSceneView *View, int32 ViewIndex,
                                           FMeshElementCollector &Collector) const;

        bool GetMeshElement(FMeshElementCollector &Collector, int32 SectionIndex, uint8 DepthPriorityGroup,
                            bool bIsSelected, FMeshBatch &OutMeshBatch) const;

        void GetNewBatchMeshes(const FSceneView *View, int32 ViewIndex, FMeshElementCollector &Collector) const;
        void GetNewBatchMeshesPrebuilt(const FSceneView *View, int32 ViewIndex, FMeshElementCollector &Collector) const;

        // Call this if you modify BatchedSections or Vertices after the proxy has already been created
        void RecreateCachedRenderData(FRHICommandListBase &RHICmdList);

        FFlipbookTextureOverrideRenderProxy *
        GetCachedMaterialProxyForSection(FMeshElementCollector &Collector, int32 SectionIndex,
                                         FMaterialRenderProxy *ParentMaterialProxy) const;

      private:
        TArray<FFlipbookRenderSection> BatchedSections;
        TArray<FDynamicMeshVertex> Vertices;

        mutable UE::FMutex MaterialTextureOverrideProxiesMutex;
        mutable TArray<TUniquePtr<FFlipbookTextureOverrideRenderProxy>> MaterialTextureOverrideProxies;

        FSimpleFlipbookVertexBuffer VertexBuffer;
        FSimpleFlipbookVertexFactory VertexFactory;

        //
        AActor *Owner;

        uint8 bDrawTwoSided : 1;
        uint8 bCastShadow : 1;
        uint8 bSpritesUseVertexBufferPath : 1;

        // The view relevance for the associated material
        FMaterialRelevance MaterialRelevance;

        // The Collision Response of the component being proxied
        FCollisionResponseContainer CollisionResponse;

#if WITH_EDITOR
        // The texture override list
        FFlipbookRenderSceneProxyTextureOverrideMap TextureOverrideList;
#endif

        UMaterialInterface *Material;
        UMaterialInterface *AlternateMaterial;
    };
} // namespace Simple2D