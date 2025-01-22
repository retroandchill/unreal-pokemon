// "Unreal Pokémon" created by Retro & Chill.


#include "Simple2D/Rendering/SimpleFlipbookSceneProxy.h"
#include "DynamicMeshBuilder.h"
#include "MaterialDomain.h"
#include "Paper2DModule.h"
#include "Simple2D.h"
#include "Materials/MaterialRenderProxy.h"
#include "Simple2D/Rendering/FlipbookTextureOverrideRenderProxy.h"
#include <bit>

DECLARE_CYCLE_STAT(TEXT("Get New Batch Meshes"), STAT_Simple2DRender_GetNewBatchMeshes, STATGROUP_Simple2D);
DECLARE_CYCLE_STAT(TEXT("SpriteProxy GDME"), STAT_Simple2DRenderSceneProxy_GetDynamicMeshElements, STATGROUP_Simple2D);

namespace Simple2D {
    static TAutoConsoleVariable CVarDrawSpritesAsTwoSided(
        TEXT("r.Simple2D.DrawTwoSided"), 1, TEXT("Draw sprites as two sided."));
    static TAutoConsoleVariable CVarDrawSpritesUsingPrebuiltVertexBuffers(
        TEXT("r.Simple2D.UsePrebuiltVertexBuffers"), 1, TEXT("Draw sprites using prebuilt vertex buffers."));

    FSimpleFlipbookSceneProxy::FSimpleFlipbookSceneProxy(const UMeshComponent *InComponent) :
        FPrimitiveSceneProxy(InComponent),
        VertexFactory(InComponent->GetWorld()->GetFeatureLevel()), Owner(InComponent->GetOwner()),
        bCastShadow(InComponent->CastShadow), MaterialRelevance(InComponent->GetMaterialRelevance(GetScene().GetFeatureLevel())),
        CollisionResponse(InComponent->GetCollisionResponseToChannels()), bDrawTwoSided(CVarDrawSpritesAsTwoSided.GetValueOnAnyThread() != 0), bSpritesUseVertexBufferPath(CVarDrawSpritesUsingPrebuiltVertexBuffers.GetValueOnAnyThread() != 0), Material(InComponent->GetMaterial(0)), AlternateMaterial(InComponent->GetMaterial(1)) {
        SetWireframeColor(FLinearColor::White);

        if (Material == nullptr) {
            Material = UMaterial::GetDefaultMaterial(MD_Surface);
        }
        
        if (AlternateMaterial == nullptr) {
            AlternateMaterial = UMaterial::GetDefaultMaterial(MD_Surface);
        }
    }

    FSimpleFlipbookSceneProxy::~FSimpleFlipbookSceneProxy() {
        VertexBuffer.ReleaseResource();
        VertexFactory.ReleaseResource();
    }

    SIZE_T FSimpleFlipbookSceneProxy::GetTypeHash() const {
        static size_t UniquePointer;
        return std::bit_cast<size_t>(std::addressof(UniquePointer));
    }

    void FSimpleFlipbookSceneProxy::SetFlipbookBounds(const FSpriteDrawCallRecord &NewDynamicData) {
        SCOPE_CYCLE_COUNTER(STAT_PaperRender_SetSpriteRT);

        BatchedSections.Reset();
        Vertices.Reset();

        auto &Section = BatchedSections[BatchedSections.AddDefaulted()];
        Section.Material = Material;
        Section.AddVerticesFromDrawCallRecord(NewDynamicData, 0, NewDynamicData.RenderVerts.Num(), Vertices);

        RecreateCachedRenderData(FRHICommandListImmediate::Get());
    }

    void FSimpleFlipbookSceneProxy::GetDynamicMeshElements(const TArray<const FSceneView *> &Views,
        const FSceneViewFamily &ViewFamily, uint32 VisibilityMap, FMeshElementCollector &Collector) const {
        SCOPE_CYCLE_COUNTER(STAT_Simple2DRenderSceneProxy_GetDynamicMeshElements);

        const FEngineShowFlags& EngineShowFlags = ViewFamily.EngineShowFlags;

        // Draw simple collision as wireframe if 'show collision', collision is enabled
        const bool bDrawWireframeCollision = EngineShowFlags.Collision && IsCollisionEnabled();


            for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
            {
                if (VisibilityMap & (1 << ViewIndex))
                {
                    const FSceneView* View = Views[ViewIndex];

                    GetDynamicMeshElementsForView(View, ViewIndex, Collector);
                }
            }

        for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
        {
            if (VisibilityMap & (1 << ViewIndex))
            {

                // Draw bounds
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
                if (EngineShowFlags.Paper2DSprites)
                {
                    RenderBounds(Collector.GetPDI(ViewIndex), EngineShowFlags, GetBounds(), (Owner == nullptr) || IsSelected());
                }
#endif
            }
        }
    }

    FPrimitiveViewRelevance FSimpleFlipbookSceneProxy::GetViewRelevance(const FSceneView *View) const {
        const FEngineShowFlags& EngineShowFlags = View->Family->EngineShowFlags;

        checkSlow(IsInParallelRenderingThread());

        FPrimitiveViewRelevance Result;
        Result.bDrawRelevance = IsShown(View) && EngineShowFlags.Paper2DSprites;
        Result.bRenderCustomDepth = ShouldRenderCustomDepth();
        Result.bRenderInMainPass = ShouldRenderInMainPass();
        Result.bUsesLightingChannels = GetLightingChannelMask() != GetDefaultLightingChannelMask();
        Result.bShadowRelevance = IsShadowCast(View);
        Result.bTranslucentSelfShadow = bCastVolumetricTranslucentShadow;

        MaterialRelevance.SetPrimitiveViewRelevance(Result);

        Result.bStaticRelevance = false;
        Result.bDynamicRelevance = true;

        if (!EngineShowFlags.Materials )
        {
            Result.bOpaque = true;
        }

        Result.bVelocityRelevance = DrawsVelocity() && Result.bOpaque && Result.bRenderInMainPass;

        return Result;
    }

    uint32 FSimpleFlipbookSceneProxy::GetMemoryFootprint() const {
	    return sizeof(*this) + GetAllocatedSize();
    }

    bool FSimpleFlipbookSceneProxy::CanBeOccluded() const {
	    return !MaterialRelevance.bDisableDepthTest;
    }

    bool FSimpleFlipbookSceneProxy::IsUsingDistanceCullFade() const {
	    return MaterialRelevance.bUsesDistanceCullFade;
    }

    void FSimpleFlipbookSceneProxy::CreateRenderThreadResources(FRHICommandListBase &RHICmdList) {
        if (bSpritesUseVertexBufferPath && (Vertices.Num() > 0))
        {
            VertexBuffer.SetVertices(Vertices);

            // Init the resources
            VertexBuffer.InitResource(RHICmdList);
            VertexFactory.Init(RHICmdList, &VertexBuffer);
        }
    }

#if WITH_EDITOR
    void FSimpleFlipbookSceneProxy::SetTransientTextureOverride_RenderThread(
        const UTexture *InTextureToModifyOverrideFor, const UTexture *InOverrideTexture) {
        if (InOverrideTexture != nullptr)
        {
            TextureOverrideList.FindOrAdd(InTextureToModifyOverrideFor) = InOverrideTexture;
        }
        else
        {
            TextureOverrideList.Remove(InTextureToModifyOverrideFor);
        }
    }
#endif

    void FSimpleFlipbookSceneProxy::GetDynamicMeshElementsForView(const FSceneView *View, int32 ViewIndex,
        FMeshElementCollector &Collector) const {
        if (bSpritesUseVertexBufferPath)
        {
            GetNewBatchMeshesPrebuilt(View, ViewIndex, Collector);
        }
        else
        {
            GetNewBatchMeshes(View, ViewIndex, Collector);
        }
        
    }

    bool FSimpleFlipbookSceneProxy::GetMeshElement(FMeshElementCollector &Collector, int32 SectionIndex,
        uint8 DepthPriorityGroup, bool bIsSelected, FMeshBatch &OutMeshBatch) const {
        check(bSpritesUseVertexBufferPath);
        check(SectionIndex < BatchedSections.Num());

        const auto& Section = BatchedSections[SectionIndex];
        if (Section.IsValid())
        {
            checkSlow(VertexBuffer.IsFullyInitialized() && VertexFactory.IsFullyInitialized());

            OutMeshBatch.bCanApplyViewModeOverrides = true;
            OutMeshBatch.bUseWireframeSelectionColoring = bIsSelected;

            OutMeshBatch.LODIndex = 0;
            OutMeshBatch.VertexFactory = &VertexFactory;
            OutMeshBatch.LCI = nullptr;
            OutMeshBatch.ReverseCulling = IsLocalToWorldDeterminantNegative() ? true : false;
            OutMeshBatch.CastShadow = bCastShadow;
            OutMeshBatch.DepthPriorityGroup = DepthPriorityGroup;
            OutMeshBatch.Type = PT_TriangleList;
            OutMeshBatch.bDisableBackfaceCulling = bDrawTwoSided;
            OutMeshBatch.MaterialRenderProxy = GetCachedMaterialProxyForSection(Collector, SectionIndex, Section.Material->GetRenderProxy());

            // Set up the FMeshBatchElement.
            FMeshBatchElement& BatchElement = OutMeshBatch.Elements[0];
            BatchElement.IndexBuffer = VertexBuffer.GetIndexPtr();
            BatchElement.FirstIndex = Section.VertexOffset;
            BatchElement.MinVertexIndex = Section.VertexOffset;
            BatchElement.MaxVertexIndex = Section.VertexOffset + Section.NumVertices;
            BatchElement.NumPrimitives = Section.NumVertices / 3;
            BatchElement.VertexFactoryUserData = VertexFactory.GetUniformBuffer();

            return true;
        }
        
        return false;
    }

    void FSimpleFlipbookSceneProxy::GetNewBatchMeshes(const FSceneView *View, int32 ViewIndex,
        FMeshElementCollector &Collector) const {
        if (BatchedSections.Num() == 0)
	{
		return;
	}

	SCOPE_CYCLE_COUNTER(STAT_Simple2DRender_GetNewBatchMeshes);

	const uint8 DPG = GetDepthPriorityGroup(View);
	const bool bIsWireframeView = View->Family->EngineShowFlags.Wireframe;

	int32 SectionIndex = 0;
	if (Vertices.Num())
	{
		FDynamicMeshBuilder DynamicMeshBuilder(View->GetFeatureLevel());
		DynamicMeshBuilder.AddVertices(Vertices);
		DynamicMeshBuilder.ReserveTriangles(Vertices.Num() / 3);
		for (int32 i = 0; i < Vertices.Num(); i += 3)
		{
			DynamicMeshBuilder.AddTriangle(i, i + 1, i + 2);
		}

		for (const FFlipbookRenderSection& Batch : BatchedSections)
		{
			if (Batch.IsValid())
			{
				FMaterialRenderProxy* ParentMaterialProxy = Batch.Material->GetRenderProxy();

				FDynamicMeshBuilderSettings Settings;
				Settings.bCanApplyViewModeOverrides = true;
				Settings.bUseWireframeSelectionColoring = IsSelected();

				// Implementing our own wireframe coloring as the automatic one (controlled by Mesh.bCanApplyViewModeOverrides) only supports per-FPrimitiveSceneProxy WireframeColor
				if (bIsWireframeView)
				{
					const FLinearColor EffectiveWireframeColor = IsOpaqueBlendMode(*Batch.Material) ? GetWireframeColor() : FLinearColor::Green;

					auto WireframeMaterialInstance = new FColoredMaterialRenderProxy(
						GEngine->WireframeMaterial->GetRenderProxy(),
						GetSelectionColor(EffectiveWireframeColor, IsSelected(), IsHovered(), false)
					);

					Collector.RegisterOneFrameMaterialProxy(WireframeMaterialInstance);

					ParentMaterialProxy = WireframeMaterialInstance;

					Settings.bWireframe = true;
					// We are applying our own wireframe override
					Settings.bCanApplyViewModeOverrides = false;
				}

				// Create a texture override material proxy or make sure our existing one is valid
				auto SectionMaterialProxy = GetCachedMaterialProxyForSection(Collector, SectionIndex, ParentMaterialProxy);

				Settings.CastShadow = bCastShadow;
				Settings.bDisableBackfaceCulling = bDrawTwoSided;

				FDynamicMeshDrawOffset DrawOffset;
				DrawOffset.FirstIndex = Batch.VertexOffset;
				DrawOffset.NumPrimitives = Batch.NumVertices / 3;
				DynamicMeshBuilder.GetMesh(GetLocalToWorld(), SectionMaterialProxy, DPG, Settings, &DrawOffset, ViewIndex, Collector);
			}

			++SectionIndex;
		}
	}
    }

    void FSimpleFlipbookSceneProxy::GetNewBatchMeshesPrebuilt(const FSceneView *View, int32 ViewIndex,
        FMeshElementCollector &Collector) const {
        const uint8 DPG = GetDepthPriorityGroup(View);
        const bool bIsWireframeView = View->Family->EngineShowFlags.Wireframe;

        //Go for each section, creating a batch and collecting it
        for (int32 SectionIndex = 0; SectionIndex < BatchedSections.Num(); SectionIndex++)
        {
            FMeshBatch& Batch = Collector.AllocateMesh();

            if (GetMeshElement(Collector, SectionIndex, DPG, IsSelected(), Batch))
            {
                // Implementing our own wireframe coloring as the automatic one (controlled by Mesh.bCanApplyViewModeOverrides) only supports per-FPrimitiveSceneProxy WireframeColor
                if (bIsWireframeView)
                {
                    const auto& Section = BatchedSections[SectionIndex];
                    const FLinearColor EffectiveWireframeColor = IsOpaqueBlendMode(*Section.Material) ? GetWireframeColor() : FLinearColor::Green;

                    auto WireframeMaterialInstance = new FColoredMaterialRenderProxy(
                        GEngine->WireframeMaterial->GetRenderProxy(),
                        GetSelectionColor(EffectiveWireframeColor, IsSelected(), IsHovered(), false)
                    );

                    Collector.RegisterOneFrameMaterialProxy(WireframeMaterialInstance);

                    // We are applying our own wireframe override
                    Batch.bCanApplyViewModeOverrides = false;
                    Batch.bWireframe = true;

                    // Create a texture override material proxy and register it as a dynamic resource so that it won't be deleted until the rendering thread has finished with it
                    Batch.MaterialRenderProxy = WireframeMaterialInstance;
                }

                Collector.AddMesh(ViewIndex, Batch);
            }
        }
    }

    void FSimpleFlipbookSceneProxy::RecreateCachedRenderData(FRHICommandListBase &RHICmdList) {
        int32 BatchIndex = 0;
        for (auto& Proxy : MaterialTextureOverrideProxies)
        {
            if ((Proxy != nullptr) && BatchedSections.IsValidIndex(BatchIndex))
            {
                const auto& Section = BatchedSections[BatchIndex];
                Proxy->Reinitialize(Section.Material->GetRenderProxy(), Section.BaseTexture, Section.AdditionalTextures);
            }
            ++BatchIndex;
        }

        if (bSpritesUseVertexBufferPath && (Vertices.Num() > 0))
        {
            VertexBuffer.SetVertices(Vertices);

            //We want the proxy to update the buffer, if it has been already initialized
            if (VertexBuffer.IsFullyInitialized())
            {
                const bool bFactoryRequiresReInitialization = VertexBuffer.CommitRequiresBufferRecreation();
                VertexBuffer.CommitVertexData(RHICmdList);

                //When the buffer reallocates, the factory needs to bind the buffers and SRV again, we just init again.
                if (bFactoryRequiresReInitialization)
                {
                    VertexFactory.Init(RHICmdList, &VertexBuffer);
                }
            }
            else
            {
                VertexBuffer.InitResource(RHICmdList);
                VertexFactory.Init(RHICmdList, &VertexBuffer);
            }
        }
    }

    FFlipbookTextureOverrideRenderProxy * FSimpleFlipbookSceneProxy::GetCachedMaterialProxyForSection(
        FMeshElementCollector &Collector, int32 SectionIndex, FMaterialRenderProxy *ParentMaterialProxy) const {
        UE::TScopeLock Lock(MaterialTextureOverrideProxiesMutex);

        if (MaterialTextureOverrideProxies.Num() < BatchedSections.Num())
        {
            MaterialTextureOverrideProxies.AddDefaulted(BatchedSections.Num() - MaterialTextureOverrideProxies.Num());
        }

        auto& Result = MaterialTextureOverrideProxies[SectionIndex];
        if (Result == nullptr)
        {
            const auto& Section = BatchedSections[SectionIndex];
#if WITH_EDITOR
            Result = MakeUnique<FFlipbookTextureOverrideRenderProxy>(ParentMaterialProxy, Section.BaseTexture, Section.AdditionalTextures, TextureOverrideList);
#else
            Result = MakeUnique<FFlipbookTextureOverrideRenderProxy>(ParentMaterialProxy, Section.BaseTexture, Section.AdditionalTextures);
#endif
        }

        if (!Result->CheckValidity(ParentMaterialProxy))
        {
            Collector.CacheUniformExpressions(Result.Get(), true);
        }

        return Result.Get();
    }
}