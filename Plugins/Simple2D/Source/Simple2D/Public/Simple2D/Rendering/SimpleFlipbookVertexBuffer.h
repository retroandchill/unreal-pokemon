// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "DynamicMeshBuilder.h"

namespace Simple2D {
    /**
     * 
     */
    class SIMPLE2D_API FSimpleFlipbookVertexBuffer : public FVertexBuffer {

    public:
        FSimpleFlipbookVertexBuffer() = default;
        
        /* Marks this buffer as dynamic, so it gets initialized as so. */
        void SetDynamicUsage(bool bInDynamicUsage);

        /* Initializes the buffers with the given number of vertices to accommodate. */
        void CreateBuffers(FRHICommandListBase& RHICmdList, int32 NumVertices);

        /* Clear all the buffers currently being used. */
        void ReleaseBuffers();

        /* Moves all the PaperVertex data onto the RHI buffers. */
        void CommitVertexData(FRHICommandListBase& RHICmdList);

        void InitRHI(FRHICommandListBase& RHICmdList) override;
        void ReleaseRHI() override;
        void InitResource(FRHICommandListBase& RHICmdList) override;
        void ReleaseResource() override;

        /* True if generating a commit would require a reallocation of the buffers. */
        FORCEINLINE bool CommitRequiresBufferRecreation() const { return NumAllocatedVertices != Vertices.Num(); }

        /* Checks if the buffer has been initialized. */
        FORCEINLINE bool IsFullyInitialized() const { return NumAllocatedVertices > 0; }

        /* Obtain the index buffer initialized for this buffer. */
        FORCEINLINE const FIndexBuffer* GetIndexPtr() const { return &IndexBuffer; }

        void SetVertices(TArray<FDynamicMeshVertex> NewVertices) {
            Vertices = std::move(NewVertices);
        }

    private:
        friend class FSimpleFlipbookVertexFactory;
        
        //Buffers
        FVertexBuffer PositionBuffer;
        FVertexBuffer TangentBuffer;
        FVertexBuffer TexCoordBuffer;
        FVertexBuffer ColorBuffer;
        FIndexBuffer IndexBuffer;

        //SRVs for Manual Fetch on platforms that support it
        FShaderResourceViewRHIRef TangentBufferSRV;
        FShaderResourceViewRHIRef TexCoordBufferSRV;
        FShaderResourceViewRHIRef ColorBufferSRV;
        FShaderResourceViewRHIRef PositionBufferSRV;

        //Vertex data
        TArray<FDynamicMeshVertex> Vertices;

        /* Indicates if this buffer will be configured for dynamic usage. */
        bool bDynamicUsage = true;

        /* Amount of vertices allocated on the vertex buffer. */
        int32 NumAllocatedVertices = 0;
    };
}
